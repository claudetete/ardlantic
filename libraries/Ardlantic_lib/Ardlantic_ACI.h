/* Air Conditioner Interface: Implementation of IR protocol about Atlantic AMC12XPM.UI remote control */

#ifndef ARDLANTIC_ACI_H
#define ARDLANTIC_ACI_H

#define ACI_IR_FRAME_DATA_BYTE_NB (13)


/* Decoder of RF */
int ACI_decodeRfIon(const unsigned int input[]);
int ACI_decodeRfStartUp(const unsigned int input[]);
int ACI_decodeRfSwing(const unsigned int input[]);
int ACI_decodeRfTimer(const unsigned int input[]);
int ACI_decodeRfHour(const unsigned int input[]);
int ACI_decodeRfFan(const unsigned int input[]);
int ACI_decodeRfMode(const unsigned int input[]);
int ACI_decodeRfMax(const unsigned int input[]);
int ACI_decodeRfTemperatureAuto(const unsigned int input[]);
int ACI_decodeRfTemperature(const unsigned int input[]);

/* encoder of IR */
void ACI_encodeIrInit(unsigned char output[]);
void ACI_encodeIrHeader(unsigned char output[]);
void ACI_encodeIrIon(unsigned char output[], int value);
void ACI_encodeIrStartUp(unsigned char output[], int value);
void ACI_encodeIrSwing(unsigned char output[], int value);
void ACI_encodeIrTimer(unsigned char output[], int value);
void ACI_encodeIrHour(unsigned char output[], int value);
void ACI_encodeIrFan(unsigned char output[], int value);
int ACI_encodeIrMode(unsigned char output[], int value);
void ACI_encodeIrMax(unsigned char output[], int value);
void ACI_encodeIrTemperatureAuto(unsigned char output[], float value);
void ACI_encodeIrTemperature(unsigned char output[], float value);
void ACI_encodeIrParity(unsigned char output[]);


#endif /* ARDLANTIC_ACI_H */
