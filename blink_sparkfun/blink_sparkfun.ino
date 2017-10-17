/* Pro Micro Test Code
   by: Nathan Seidle
   modified by: Jim Lindblom
   SparkFun Electronics
   date: September 16, 2013
   license: Public Domain - please use this code however you'd like.
   It's provided as a learning tool.

   This code is provided to show how to control the SparkFun
   ProMicro's TX and RX LEDs within a sketch. It also serves
   to explain the difference between Serial.print() and
   Serial1.print().
*/

// The RX LED has a defined Arduino pin
int RXLED = 17;
// The TX LED was not so lucky, we'll need to use pre-defined
// macros (TXLED1, TXLED0) to control that.
// (We could use the same macros for the RX LED too -- RXLED1,
//  and RXLED0.)

void setup()
{
  // Set RX LED as an output
  pinMode(RXLED, OUTPUT);
  // TX LED is set as an output behind the scenes

  //This pipes to the serial monitor
  Serial.begin(9600);
  //This is the UART, pipes to sensors attached to board
  Serial1.begin(9600);
}

void loop()
{
  // Print "Hello World" to the Serial Monitor
  Serial.println("Hello world");
  // Print "Hello!" over hardware UART
  Serial1.println("Hello!");

  // set the LED on
  digitalWrite(RXLED, LOW);
  //TX LED is not tied to a normally controlled pin
  TXLED0;
  // wait for a second
  delay(1000);

  // set the LED off
  digitalWrite(RXLED, HIGH);
  TXLED1;
  // wait for a second
  delay(1000);
}
