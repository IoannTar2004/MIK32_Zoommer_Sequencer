#include "libs/tone.h"
#include "project/sequencer_logic.h"
#include "xprintf.h"
#include "utils/pins.h"
#include "utils/delays.h"
#include "project/IRcontroller.h"

#define LED 20

void sequencer_runner_init(uint8_t zoomer_pin_num) {

    tone_init(zoomer_pin_num);
    pin_mode(LED, __OUTPUT);
}

void sequencer_play() {
    if (!get_play()) {
        digital_write(LED, LOW);
        return;
    }

    digital_write(LED, HIGH);
    volatile sequence* sequencer = get_sequencer();
    for (int j = 0; j < get_bpm() / 30; j++) {
        for (int i = 0; i < 16; i++) {
            tone(sequencer[i].freq, sequencer[i].duration, controller_decode);
            if (!get_play)
                break;
        }
    }
    reset_play();
}