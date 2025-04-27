#include <inttypes.h>
#include "project/display.h"

static uint8_t bpm = 120;

void sequencer_init() {
    print_bpm(bpm);
}
