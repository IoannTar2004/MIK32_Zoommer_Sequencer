#include "timer32.h"
#include "mik32_hal.h"
#include "utils/pins.h"
#include "epic.h"
#include "utils/riscv_irq.h"

#define NULL (void*) (0)
#define EPIC_LINE_TIMER(timer) EPIC_LINE_##timer##_S

static uint8_t channel;
static TIMER32_TypeDef* pwm_timer;
static TIMER32_TypeDef* duration_timer;
static uint8_t stop = 0;

static uint8_t get_epic_line_timer() {
    if (duration_timer == TIMER32_0) return EPIC_LINE_TIMER32_0_S;
    if (duration_timer == TIMER32_1) return EPIC_LINE_TIMER32_1_S;
    if (duration_timer == TIMER32_2) return EPIC_LINE_TIMER32_2_S;

    return -1;
}

uint8_t get_stop_flag() {
    return stop;
}

void reset_stop_flag() {
    stop = 0;
}

static void duration_trap_handler() {
    uint8_t timer = get_epic_line_timer();

    if (EPIC->RAW_STATUS & (1 << timer)) {
        pwm_timer->CHANNELS[channel].OCR = 0;
        duration_timer->INT_MASK = ~(TIMER32_INT_OVERFLOW_M);
        duration_timer->INT_CLEAR = 0xFFFFFFFF;
        duration_timer->ENABLE = 0;
        stop = 1;

        EPIC->CLEAR = timer;
    }
}

void tone_init(uint8_t pin_num, TIMER32_TypeDef* _pwm_timer, TIMER32_TypeDef* _duration_timer) {
    channel = get_pin(pin_num).pwm_channel;
    pwm_timer = _pwm_timer;
    duration_timer = _duration_timer;

    pin_mode_init(pin_num, HAL_GPIO_MODE_TIMER_SERIAL, HAL_GPIO_PULL_NONE, HAL_GPIO_DS_2MA);

    PM->CLK_APB_M_SET = PM_CLOCK_APB_M_EPIC_M;
    EPIC->MASK_LEVEL_SET = 1 << get_epic_line_timer();
    riscv_irq_set_handler(RISCV_IRQ_MEI, duration_trap_handler);
    riscv_irq_enable(RISCV_IRQ_MEI);
    riscv_irq_global_enable();
}

void tone(uint32_t freq, float ms) {
    if (pwm_timer->CHANNELS[channel].OCR == 0) {
        uint32_t period_ticks = (uint32_t) (OSC_SYSTEM_VALUE / freq);
        pwm_timer->TOP = period_ticks;
        pwm_timer->CHANNELS[channel].OCR = period_ticks / 2;

        duration_timer->TOP = OSC_SYSTEM_VALUE / 1000 * ms;
        duration_timer->INT_MASK = TIMER32_INT_OVERFLOW_M;
        duration_timer->ENABLE = 1;
    }
}