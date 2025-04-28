#include "mik32_memory_map.h"
#include "scr1_timer.h"
#include "timer32.h"
#include "utils/delays.h"

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
