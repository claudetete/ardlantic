#include <Arduino.h>

#include <Ardlantic_IRD.h>

/* The RX LED has a defined Arduino pin */
#define RXLED 17


/* Values for remote control of Atlantic AMC12XPM.UI */
/* Header start value, duration in microsecond */
#define AC_START (3750)
/* Header first value, duration in microsecond */
#define AC_FIRST (1950)
/* space/pause between command (except header start), duration in microsecond */
#define AC_PAUSE (400)
/* Command bit 1, duration in microsecond */
#define AC_BIT1  (1450)
/* Command bit 0, duration in microsecond */
#define AC_BIT0  (550)

/* number of bytes for data part */
#define AC_VALUE_BYTE_NB (13)
/* number of command for whole frame */
/* 2 command for header + 1 pause + 13 bytes of data (13 * 8 bits * 2 to have data + pause for each bit) */
#define AC_RX_COMMAND_NB (2 + 1 + (AC_VALUE_BYTE_NB * 8 * 2))

/* buffer of RX command in microseconds */
unsigned int RX_buffer[AC_RX_COMMAND_NB];

const char RX_value[AC_VALUE_BYTE_NB] = {
  /* fanauto hot 26 */
  0xC1, 0xF0, 0x04, 0x80, 0x08, 0x03, 0x21, 0x31, 0x09, 0x10, 0xCF, 0x5A, 0xAA
};

void setup()
{
  int bit_pos = 0;
  int word_pos = 0;
  int command_pos = 0;

  // Set RX LED as an output
  pinMode(RXLED, OUTPUT);

  //This pipes to the serial monitor
  Serial.begin(9600);

  /* init rx command buffer for header */
  RX_buffer[command_pos] = AC_START;
  command_pos++;
  RX_buffer[command_pos] = AC_FIRST;
  command_pos++;
  RX_buffer[command_pos] = AC_PAUSE;
  command_pos++;

  /* fill rx command buffer with data part from last word to first (msb to lsb) */
  for (word_pos = AC_VALUE_BYTE_NB - 1; word_pos >= 0; word_pos--)
  {
    /* in bit order lsb to msb */
    for (bit_pos = 0; bit_pos < 8; bit_pos++)
    {
      /* data */
      if ((RX_value[word_pos] >> bit_pos) & 0x1)
      {
        RX_buffer[command_pos] = AC_BIT1;
      }
      else
      {
        RX_buffer[command_pos] = AC_BIT0;
      }
      command_pos++;

      /* space/pause */
      RX_buffer[command_pos] = AC_PAUSE;
      command_pos++;
    }
  }
}

void loop()
{
  // Print "Hello World" to the Serial Monitor
  Serial.println("Hello world");

  // set the LED on
  digitalWrite(RXLED, LOW);
  // wait
  delay(100);
  // set the LED off
  digitalWrite(RXLED, HIGH);
  // wait
  delay(100);
  // set the LED on
  digitalWrite(RXLED, LOW);
  // wait
  delay(1000);
  // set the LED off
  digitalWrite(RXLED, HIGH);
  /* try send IR raw */
  IRD_sendRaw(RX_buffer, AC_RX_COMMAND_NB, 38);
  // wait
  delay(1000);
}
