#ifndef RECEPTION_COMMUNICATION_H
#define RECEPTION_COMMUNICATION_H

#include "../common_code/communication/keys.h"
#include "../common_code/communication/emission.h"
#include "../common_code/communication/lecture_reception.h"

extern callback_t callbacks[KEYS_SIZE];

void init_reception_communication();
void reception_communication(char c);


#endif
