/*
    Minimal hardware layer for the Alvik carrier (STM32F411, HAL).

    Plain C port of the pin handling in Arduino_AlvikCarrier's
    DCmotor (dcmotor.cpp), Encoder (encoder.cpp) and HAL_custom_init.h.
*/

#include "alvik.h"
#include "pinout_definitions.h"
#include "robot_definitions.h"


// HAL port/pin equivalents of the Arduino pin names in pinout_definitions.h
#define MOTORS_ENABLE_PORT      GPIOD           // PD2
#define MOTORS_ENABLE_PIN       GPIO_PIN_2

#define ENC_LEFT_PORT           GPIOC           // PC6, PC7 -> TIM3
#define ENC_LEFT_PINS           (GPIO_PIN_6 | GPIO_PIN_7)
#define ENC_RIGHT_PORT          GPIOA           // PA0, PA1 -> TIM5
#define ENC_RIGHT_PINS          (GPIO_PIN_0 | GPIO_PIN_1)

typedef struct {
    GPIO_TypeDef * port;
    uint16_t pin;
} pin_t;

typedef struct {
    pin_t fwd;
    pin_t rev;
} motor_pins_t;

// Motor driver inputs
#define PIN_LEFT_A      { GPIOA, GPIO_PIN_3 }       // PA3
#define PIN_LEFT_B      { GPIOB, GPIO_PIN_3 }       // PB3
#define PIN_RIGHT_A     { GPIOA, GPIO_PIN_2 }       // PA2
#define PIN_RIGHT_B     { GPIOA, GPIO_PIN_15 }      // PA15

// *_FLIP swaps the A/B inputs, as in the DCmotor constructor
#if MOTOR_LEFT_FLIP
#define LEFT_PINS       { .fwd = PIN_LEFT_B, .rev = PIN_LEFT_A }
#else
#define LEFT_PINS       { .fwd = PIN_LEFT_A, .rev = PIN_LEFT_B }
#endif

#if MOTOR_RIGHT_FLIP
#define RIGHT_PINS      { .fwd = PIN_RIGHT_B, .rev = PIN_RIGHT_A }
#else
#define RIGHT_PINS      { .fwd = PIN_RIGHT_A, .rev = PIN_RIGHT_B }
#endif

static const motor_pins_t motor_pins[MOTOR_COUNT] = {
    [MOTOR_LEFT]  = LEFT_PINS,
    [MOTOR_RIGHT] = RIGHT_PINS,
};

// Speed is measured over this many ticks (same window as the Arduino library)
#define SPEED_TICKS             ((uint32_t)(ALVIK_TICK_HZ * MOTOR_CONTROL_PERIOD))
#define RPM_PER_COUNT           ((float)(60.0 / (MOTOR_CPR * MOTOR_GEAR_RATIO * MOTOR_CONTROL_PERIOD)))

static TIM_HandleTypeDef htim_enc[MOTOR_COUNT];
static TIM_HandleTypeDef * tick_htim;
static void (* volatile tick_callback)(void);

static volatile float rpm[MOTOR_COUNT];
static int16_t last_count[MOTOR_COUNT];
static uint32_t speed_ticks;


/***************************************************************/
/*                            Init                             */
/***************************************************************/

static void output_init(GPIO_TypeDef * port, uint16_t pin){
    GPIO_InitTypeDef gpio = {0};
    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
    gpio.Pin = pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &gpio);
}

static void encoder_timer_init(TIM_HandleTypeDef * htim, TIM_TypeDef * tim, bool flip){
    htim->Instance = tim;
    htim->Init.Prescaler = 0;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = 65535;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    TIM_Encoder_InitTypeDef cfg = {0};
    cfg.EncoderMode = TIM_ENCODERMODE_TI2;
    cfg.IC1Polarity = flip ? TIM_ICPOLARITY_FALLING : TIM_ICPOLARITY_RISING;
    cfg.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    cfg.IC1Prescaler = TIM_ICPSC_DIV1;
    cfg.IC1Filter = 0;
    cfg.IC2Polarity = TIM_ICPOLARITY_RISING;
    cfg.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    cfg.IC2Prescaler = TIM_ICPSC_DIV1;
    cfg.IC2Filter = 0;

    HAL_TIM_Encoder_Init(htim, &cfg);
    HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
    htim->Instance->CNT = 0;
}

void alvik_init(TIM_HandleTypeDef * tick_timer){
    GPIO_InitTypeDef gpio = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_TIM3_CLK_ENABLE();
    __HAL_RCC_TIM5_CLK_ENABLE();

    // Motors: plain outputs, all off
    output_init(MOTORS_ENABLE_PORT, MOTORS_ENABLE_PIN);
    for (int m = 0; m < MOTOR_COUNT; m++){
        output_init(motor_pins[m].fwd.port, motor_pins[m].fwd.pin);
        output_init(motor_pins[m].rev.port, motor_pins[m].rev.pin);
    }

    // Encoders: left PC6/PC7 -> TIM3, right PA0/PA1 -> TIM5 (both AF2)
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    gpio.Pin = ENC_LEFT_PINS;
    gpio.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(ENC_LEFT_PORT, &gpio);
    gpio.Pin = ENC_RIGHT_PINS;
    gpio.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(ENC_RIGHT_PORT, &gpio);

    encoder_timer_init(&htim_enc[MOTOR_LEFT], ENC_LEFT_TIMER, ENC_LEFT_FLIP);
    encoder_timer_init(&htim_enc[MOTOR_RIGHT], ENC_RIGHT_TIMER, ENC_RIGHT_FLIP);
    for (int m = 0; m < MOTOR_COUNT; m++){
        last_count[m] = 0;
        rpm[m] = 0.0f;
    }

    HAL_GPIO_WritePin(MOTORS_ENABLE_PORT, MOTORS_ENABLE_PIN, GPIO_PIN_SET);

    // Tick timer: measures speed, then calls the user callback
    tick_htim = tick_timer;
    HAL_TIM_Base_Start_IT(tick_htim);
}


/***************************************************************/
/*                          Motors                             */
/***************************************************************/

void motor_set(motor_id_t motor, bool on){
    if (motor >= MOTOR_COUNT){
        return;
    }
    HAL_GPIO_WritePin(motor_pins[motor].fwd.port, motor_pins[motor].fwd.pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(motor_pins[motor].rev.port, motor_pins[motor].rev.pin, GPIO_PIN_RESET);
}

float motor_get_rpm(motor_id_t motor){
    if (motor >= MOTOR_COUNT){
        return 0.0f;
    }
    return rpm[motor];
}


/***************************************************************/
/*                           Timer                             */
/***************************************************************/

void timer_start(void (*callback)(void)){
    tick_callback = callback;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
    if (htim != tick_htim){
        return;
    }

    if (++speed_ticks >= SPEED_TICKS){
        speed_ticks = 0;
        for (int m = 0; m < MOTOR_COUNT; m++){
            int16_t count = (int16_t)__HAL_TIM_GET_COUNTER(&htim_enc[m]);   // cast gives the sign
            rpm[m] = (int16_t)(count - last_count[m]) * RPM_PER_COUNT;      // int16 wrap-safe
            last_count[m] = count;
        }
    }

    if (tick_callback){
        tick_callback();
    }
}
