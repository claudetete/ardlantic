/* InfraRed Driver: send raw command */

#ifndef ARDLANTIC_IRD_H
#define ARDLANTIC_IRD_H

#include <Arduino.h>

/* Send raw infra red signal defined in a BUFFER of command duration of space or mark */
/* in BUFFER array, even index are mark and odd index ar space                        */
/* LENGTH is size of BUFFER array and HZ is modulation frequence in kHz               */
void IRD_sendRaw(const uint16_t buffer[], uint8_t length, uint8_t khz);

#endif /* ARDLANTIC_IRD_H */
