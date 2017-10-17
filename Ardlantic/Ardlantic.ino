#include <Arduino.h>

/* Infra Red Protocol */
#include <Ardlantic_IRP.h>

/**
 * MYSENSORS
 **/
/* Enable debug prints to serial monitor */
#define MY_DEBUG
/* Enable and select radio type attached (NRF24) */
#define MY_RADIO_NRF24
/* Enable repeater functionality for this node */
#define MY_REPEATER_FEATURE

/* do not use to high baud rate when not needed */
#define MY_BAUD_RATE 9600
/* CE/CS PIN for NRF24L01+ can be redefined (9 and 10 by default) */
/* #define MY_RF24_CE_PIN 9 */
/* #define MY_RF24_CS_PIN 10 */

/* node id used for gateway (must be uniq) */
/* when not set it leave the gateway to assign an id (do not work always) */
#define MY_NODE_ID 42

/* MySensors will override usual function of a sketch, it for a node it need a gateway to start */
#include <MySensors.h>

/* Sensor id to present and finally received */
#define SENSOR_ID_SEND          (0)
#define SENSOR_ID_ION           (1)
#define SENSOR_ID_STARTUP       (2)
#define SENSOR_ID_SWING         (3)
#define SENSOR_ID_HOUR          (4)
#define SENSOR_ID_FAN_MODE      (5)
#define SENSOR_ID_FLOW_MODE     (6)
#define SENSOR_ID_FAN_MAX       (7)
#define SENSOR_ID_TEMP_RELATIVE (8)
#define SENSOR_ID_TEMPEATURE    (9)

/* instance a IR message*/
IRP_Message* MessageIR;

void before()
{
  /* everything before MySensors execution */
  MessageIR = new IRP_Message();
}

void setup()
{
  /* everything to init after MySensors init */
}

void presentation()
{
  /* Send the sketch version information to the gateway and Node */
  sendSketchInfo("AC living room", "1.0");

  /* everything to present each sensors/actuators on this node to the gateway (so domotic box will register it) */
  present(SENSOR_ID_SEND, S_BINARY);
  present(SENSOR_ID_ION, S_BINARY);
  present(SENSOR_ID_STARTUP, S_BINARY);
  present(SENSOR_ID_SWING, S_BINARY);
  present(SENSOR_ID_HOUR, S_CUSTOM);
  present(SENSOR_ID_FAN_MODE, S_HVAC);
  present(SENSOR_ID_FLOW_MODE, S_HVAC);
  present(SENSOR_ID_FAN_MAX, S_BINARY);
  present(SENSOR_ID_TEMP_RELATIVE, S_TEMP);
  present(SENSOR_ID_TEMPEATURE, S_TEMP);
}

void loop()
{
}

void receive(const MyMessage &message)
{
  Serial.println("Message received!");

  switch(message.sensor)
  {
    case SENSOR_ID_ION:
      MessageIR->setIon(message.getBool());
      break;

    case SENSOR_ID_STARTUP:
      MessageIR->setStartUp(message.getBool());
      break;

    case SENSOR_ID_SWING:
      MessageIR->setSwing(message.getBool());
      break;

    case SENSOR_ID_HOUR:
      MessageIR->setHour(message.getByte());
      break;

    case SENSOR_ID_FAN_MODE:
      MessageIR->setFanMode((IRP_FanMode_t)message.getByte());
      break;

    case SENSOR_ID_FLOW_MODE:
      MessageIR->setFlowMode((IRP_FlowMode_t)message.getByte());
      break;

    case SENSOR_ID_FAN_MAX:
      MessageIR->setFanMax(message.getBool());
      break;

    case SENSOR_ID_TEMP_RELATIVE:
      MessageIR->setTemperatureRelative(message.getInt());
      break;

    case SENSOR_ID_TEMPEATURE:
      MessageIR->setTemperature(message.getByte());
      break;

    case SENSOR_ID_SEND:
      Serial.println("Send IR frame...");
      MessageIR->send();
      /* TODO send message to indicate IR was emit */
      break;

    default:
      Serial.println("Message received with unknown sensor ID.");
      break;
  }
}
