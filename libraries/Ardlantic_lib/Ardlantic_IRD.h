/* InfraRed Driver: send raw command */

#ifndef ARDLANTIC_IRD_H
#define ARDLANTIC_IRD_H


/* Send raw infra red signal defined in a BUFFER of command duration of space or mark */
/* in BUFFER array, even index are mark and odd index ar space                        */
/* LENGTH is size of BUFFER array and HZ is modulation frequence in kHz               */
void IRD_sendRaw(const unsigned int buffer[], unsigned int length, unsigned int khz);

#endif /* ARDLANTIC_IRD_H */
