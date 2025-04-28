#ifndef SEQUENCER_LOGIC
#define SEQUENCER_LOGIC

void sequncer_init();
void ir_change_page();
void ir_press_plus_minus(uint8_t plus);
void ir_press_up_down(uint8_t up);
void ir_press_next_prev(uint8_t next);
void ir_press_numbers_init(uint32_t first);

inline uint8_t get_play();
inline void set_play(uint8_t _play);

#endif