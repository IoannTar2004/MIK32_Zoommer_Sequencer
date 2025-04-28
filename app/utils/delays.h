#ifndef DELAYS
#define DELAYS

#define SCR1_TIMER_GET_TIME()                                                  \
   (((uint64_t)(SCR1_TIMER->MTIMEH) << 32) | (SCR1_TIMER->MTIME))

#define SYSTEM_FREQ_HZ 32000000UL

void delay(uint32_t ms);
void delayMicroseconds(uint32_t ms);

#endif