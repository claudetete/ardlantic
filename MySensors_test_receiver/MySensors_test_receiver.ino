/* test/example of NRF24 with MySensors library - receiver */
/* need of two arduino (here Pro Micro from SparkFun) one transmitter other receiver */

/* Enable debug prints to serial monitor */
#define MY_DEBUG
/* #define MY_DEBUG_VERBOSE_RF24 */

/* Enable and select radio type attached (NRF24) */
#define MY_RADIO_NRF24

/* Enable repeater functionality for this node */
/* #define MY_REPEATER_FEATURE */


#define MY_BAUD_RATE 9600
/* #define MY_RF24_CE_PIN 9 */
/* #define MY_RF24_CS_PIN 10 */

/* node id used for gateway */
#define MY_NODE_ID 1

#include <SPI.h>
#include <MySensors.h>

/* The RX LED has a defined on Arduino pin of Pro Micro (SparkFun) */
#define IRLED 6


int flag = 0;

void before()
{
  /* This pipes to the serial monitor through USB */
  /* Serial.begin(9600); */
}

void setup()
{
  /* Set IR LED as an output */
  pinMode(IRLED, OUTPUT);
}

void presentation()
{
  /* Send the sketch version information to the gateway and Controller */
  sendSketchInfo("NRF24_test_receiver2", "1.0");

  /* Register all sensors to gw (they will be created as child devices) */
  present(63, S_BINARY);
}

void loop()
{
  if (flag)
  {
    digitalWrite(IRLED, LOW);
  }
  else
  {
    digitalWrite(IRLED, HIGH);
  }
  /* wait in millisecond second */
  wait(100);
}

void receive(const MyMessage &message)
{
  /* We only expect one type of message from controller. But we better check anyway. */
  if (message.type == V_STATUS && message.sensor == 63)
  {
    Serial.println("Message received!");
    /* Change relay state */
    if (message.getBool())
    {
      flag = flag ? 0 : 1;
      Serial.println("with right value");
    }
    else
    {
      flag = 0;
    }
  }
}
