//******************************************************************************
// IRremote
// Version 2.0.1 June, 2015
// Copyright 2009 Ken Shirriff
// For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
// Edited by Mitra to add new controller SANYO
//
// Interrupt code based on NECIRrcv by Joe Knapp
// http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
// Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
//
// JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
// LG added by Darryl Smith (based on the JVC protocol)
// Whynter A/C ARC-110WD added by Francesco Meschia
//
// Keep only send raw by Claude Tete (IRD, Infra Red Driver)
//******************************************************************************

#include "Arduino.h"

#include "Ardlantic_IRD.h"

/* each define are only use for arduino pro micro (sparkfun) with pin 9 as IR output */

/* main Arduino clock */
#define ird_SYSCLOCK  F_CPU

#define ird_TIMER_ENABLE_PWM()    (TCCR1A |= _BV(COM1A1))
#define ird_TIMER_DISABLE_PWM()   (TCCR1A &= ~(_BV(COM1A1)))
#define ird_TIMER_ENABLE_INTR()   (TIMSK1 = _BV(OCIE1A))
#define ird_TIMER_DISABLE_INTR()  (TIMSK1 = 0)
#define ird_TIMER_INTR_NAME()     TIMER1_COMPA_vect

#define ird_TIMER_CONFIG_KHZ(val) ({                            \
      const uint16_t pwmval = ird_SYSCLOCK / 2000 / (val);      \
      TCCR1A                = _BV(WGM11);                       \
      TCCR1B                = _BV(WGM13) | _BV(CS10);           \
      ICR1                  = pwmval;                           \
      OCR1A                 = pwmval / 3;                       \
    })

#define ird_TIMER_CONFIG_NORMAL() ({                   \
      TCCR1A = 0;                                      \
      TCCR1B = _BV(WGM12) | _BV(CS10);                 \
      OCR1A  = ird_SYSCLOCK * USECPERTICK / 1000000;   \
      TCNT1  = 0;                                      \
    })

#define ird_TIMER_PWM_PIN  9


void ird_enableIROut(int khz);
void ird_mark(unsigned int time);
void ird_space(unsigned int time);
void ird_custom_delay_usec(unsigned long uSecs);


void IRD_sendRaw(const uint16_t buffer[], uint8_t length, uint8_t khz)
{
  uint8_t buffer_index = 0;

  // Set IR carrier frequency
  ird_enableIROut(khz);

  for (buffer_index = 0; buffer_index < length; buffer_index++)
  {
    if (buffer_index & 1)
    {
      ird_space((unsigned int)buffer[buffer_index]);
    }
    else
    {
      ird_mark((unsigned int)buffer[buffer_index]);
    }
  }

  // Always end with the LED off
  ird_space(0);
}

// Enables IR output.  The khz value controls the modulation frequency in kilohertz.
// The IR output will be on pin 3 (OC2B).
// This routine is designed for 36-40KHz; if you use it for other values, it's up to you
// to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
// TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
// controlling the duty cycle.
// There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
// To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
// A few hours staring at the ATmega documentation and this will all make sense.
// See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.
void ird_enableIROut(int khz)
{
  // Disable the Timer2 Interrupt (which is used for receiving IR)
  //Timer2 Overflow Interrupt
  ird_TIMER_DISABLE_INTR();

  pinMode(ird_TIMER_PWM_PIN, OUTPUT);
  // When not sending PWM, we want it low
  digitalWrite(ird_TIMER_PWM_PIN, LOW);

  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2  = 000: no prescaling
  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  ird_TIMER_CONFIG_KHZ(khz);
}

// Sends an IR mark for the specified number of microseconds.
// The mark output is modulated at the PWM frequency.
void ird_mark(unsigned int time)
{
  // Enable pin 3 PWM output
  ird_TIMER_ENABLE_PWM();

  if (time > 0)
  {
    ird_custom_delay_usec(time);
  }
}

// Leave pin off for time (given in microseconds)
// Sends an IR space for the specified number of microseconds.
// A space is no output, so the PWM output is disabled.
void ird_space(unsigned int time)
{
  // Disable pin 3 PWM output
  ird_TIMER_DISABLE_PWM();

  if (time > 0)
  {
    ird_custom_delay_usec(time);
  }
}

// Custom delay function that circumvents Arduino's delayMicroseconds limit
void ird_custom_delay_usec(unsigned long uSecs)
{
  if (uSecs > 4)
  {
    unsigned long start = micros();
    unsigned long endMicros = start + uSecs - 4;
    if (endMicros < start)
    {
      // Check if overflow
      while (micros() > start)
      {
        // wait until overflow
      }
    }

    while (micros() < endMicros)
    {
      // normal wait
    }
  }
  //else
  //{
  //  __asm__("nop\n\t"); // must have or compiler optimizes out
  //}
}
