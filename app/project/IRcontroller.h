#ifndef IRCONTROLLER
#define IRCONTROLLER

typedef enum {
    BUTTON_CH_MINUS    = 0xFFA25D, // CH-
    BUTTON_CH          = 0xFF629D, // CH
    BUTTON_CH_PLUS     = 0xFFE21D, // CH+
    BUTTON_PREV        = 0xFF22DD, // |<<
    BUTTON_NEXT        = 0xFF02FD, // >>|
    BUTTON_PLAY_PAUSE  = 0xFFC23D, // PLAY/PAUSE
    BUTTON_MINUS       = 0xFFE01F, // -
    BUTTON_PLUS        = 0xFFA857, // +
    BUTTON_EQ          = 0xFF906F, // EQ
    BUTTON_100_PLUS    = 0xFF9867, // 100+
    BUTTON_200_PLUS    = 0xFFB04F, // 200+
    BUTTON_0           = 0xFF6897, // 0
    BUTTON_1           = 0xFF30CF, // 1
    BUTTON_2           = 0xFF18E7, // 2
    BUTTON_3           = 0xFF7A85, // 3
    BUTTON_4           = 0xFF10EF, // 4
    BUTTON_5           = 0xFF38C7, // 5
    BUTTON_6           = 0xFF5AA5, // 6
    BUTTON_7           = 0xFF42BD, // 7
    BUTTON_8           = 0xFF4AB5, // 8
    BUTTON_9           = 0xFF52AD  // 9
} ir_button;

void contoller_init(uint8_t pin);
void controller_decode();

#endif