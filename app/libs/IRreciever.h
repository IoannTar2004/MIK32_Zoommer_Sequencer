#ifndef IRRECIVER
#define IRRECIVER

void ir_set_pin(uint8_t pin_num);
int ir_decode(uint32_t* data);

#endif