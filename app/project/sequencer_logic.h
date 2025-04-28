#ifndef SEQUENCER_LOGIC
#define SEQUENCER_LOGIC

typedef struct {
    uint8_t note;
    uint16_t freq;
    float duration;
} sequence;

void sequncer_init();
void ir_change_page();
void print_last_note();
void ir_press_plus_minus(uint8_t plus);
void ir_press_up_down(uint8_t up);
void ir_press_next_prev(uint8_t next);
void ir_press_numbers_init(uint32_t first);

inline sequence* get_sequencer();
inline uint8_t get_play();
inline void switch_play();
void reset_play();

#endif