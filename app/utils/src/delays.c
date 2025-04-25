#include "mik32_memory_map.h"
#include "scr1_timer.h"

#define SCR1_TIMER_GET_TIME()                                                  \
   (((uint64_t)(SCR1_TIMER->MTIMEH) << 32) | (SCR1_TIMER->MTIME))

#define SYSTEM_FREQ_HZ 32000000UL

__attribute__((section(".ram_text"))) 
void delay(uint32_t ms) {
    uint64_t end_mtimer = SCR1_TIMER_GET_TIME() + ms * (SYSTEM_FREQ_HZ / 1000);
    while (SCR1_TIMER_GET_TIME() < end_mtimer);
}

__attribute__((section(".ram_text"))) 
void delayMicroseconds(uint32_t us) {
    uint64_t end_mtimer = SCR1_TIMER_GET_TIME() + us * (SYSTEM_FREQ_HZ / 1000000);
    while (SCR1_TIMER_GET_TIME() < end_mtimer);
}