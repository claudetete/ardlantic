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
/* CE/CS PIN for NRF24L01+ can be redefined (9 and 10 by default but use by IR to have PWM) */
#define MY_RF24_CE_PIN 5
#define MY_RF24_CS_PIN 6

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
#define SENSOR_ID_TEMPERATURE   (9)
#define SENSOR_ID_POWER_OFF     (10)
#define SENSOR_ID_CLEAR         (11)
#define SENSOR_ID_HVAC_POWER    (12)

#define PIN_ID_HVAC_POWER         (A2)
#define HVAC_POWER_HYSTERESIS_MIN (2)
#define HVAC_POWER_HYSTERESIS_MAX (4)

/* 1 second in milliseconds (1000) */
#define HVAC_POWER_POOLING_TIME   (1000)

/* instance a IR message*/
IRP_Message* MessageIR;

/* flag indicate that HVAC is ON */
boolean IsHvacPowerOn;

MyMessage MyMessageHour(SENSOR_ID_HOUR, V_PERCENTAGE);
MyMessage MyMessageFanMode(SENSOR_ID_FAN_MODE, V_PERCENTAGE);
MyMessage MyMessageFlowMode(SENSOR_ID_FLOW_MODE, V_PERCENTAGE);
MyMessage MyMessageTemperature(SENSOR_ID_TEMPERATURE, V_PERCENTAGE);
MyMessage MyMessageTempRelative(SENSOR_ID_TEMP_RELATIVE, V_PERCENTAGE);
MyMessage MyMessageHvacPower(SENSOR_ID_HVAC_POWER, V_STATUS);

void before()
{
  /* everything before MySensors execution */
  MessageIR = new IRP_Message();

  pinMode(PIN_ID_HVAC_POWER, INPUT);
  IsHvacPowerOn = false;
}

void setup()
{
  /* everything to init after MySensors init */
	request(SENSOR_ID_HOUR, V_DIMMER);
	request(SENSOR_ID_FAN_MODE, V_DIMMER);
	request(SENSOR_ID_FLOW_MODE, V_DIMMER);
  /* use V_DIMMER with V_PERCENTAGE to be able to retrieve a value */
  /* V_TEMP can not receive but only send temperature values */
  request(SENSOR_ID_TEMPERATURE, V_DIMMER);
  request(SENSOR_ID_TEMP_RELATIVE, V_DIMMER);
}

void presentation()
{
  /* Send the sketch version information to the gateway and Node */
  sendSketchInfo("AC First Floor", "1.0");

  /* everything to present each sensors/actuators on this node to the gateway (so domotic box will register it) */
  present(SENSOR_ID_SEND, S_BINARY);
  present(SENSOR_ID_ION, S_BINARY);
  present(SENSOR_ID_STARTUP, S_BINARY);
  present(SENSOR_ID_POWER_OFF, S_BINARY);
  present(SENSOR_ID_SWING, S_BINARY);
  present(SENSOR_ID_HOUR, S_DIMMER);
  present(SENSOR_ID_FAN_MODE, S_WATER);
  present(SENSOR_ID_FLOW_MODE, S_DIMMER);
  present(SENSOR_ID_FAN_MAX, S_BINARY);
  present(SENSOR_ID_TEMP_RELATIVE, S_DIMMER);
  present(SENSOR_ID_TEMPERATURE, S_DIMMER);
  present(SENSOR_ID_CLEAR, S_BINARY);
}

void loop()
{
  static bool isFirstRun = true;

  if (isFirstRun)
  {
    isFirstRun = false;
    send(MyMessageHour.set((int16_t)0));
    send(MyMessageFanMode.set((int16_t)0));
    send(MyMessageFlowMode.set((int16_t)0));
    send(MyMessageTemperature.set((int16_t)0));
    send(MyMessageTempRelative.set((int16_t)0));
  }

  /* read voltage using voltage divider */
  /* divide by 11 because with 1MΩ and 100KΩ */
  /* which means protection about overvoltage (max 55V = 5V * 11) */
  float power_voltage = ((float)analogRead(PIN_ID_HVAC_POWER) * (5.0 / 1023.0)) * 11;

  /* detect with hysteresis to avoid unwanted switching */
  if (power_voltage <= HVAC_POWER_HYSTERESIS_MIN)
  {
    IsHvacPowerOn = false;
    Serial.print("Not Power: ");
    Serial.println(power_voltage);
  }
  else if (power_voltage >= HVAC_POWER_HYSTERESIS_MAX)
  {
    IsHvacPowerOn = true;
    Serial.print("Power: ");
    Serial.println(power_voltage);
  }
  send(MyMessageHvacPower.set(IsHvacPowerOn ? "1" : "0"));

  wait(HVAC_POWER_POOLING_TIME);
}

void receive(const MyMessage &message)
{
  int hour;
  int mode;
  int relative;
  int temperature;
  Serial.println("Message received!");

  switch(message.sensor)
  {
    case SENSOR_ID_ION:
      MessageIR->setIon(message.getBool());
      Serial.println("##### ION");
      break;

    case SENSOR_ID_STARTUP:
      MessageIR->setStartUp(message.getBool() && !IsHvacPowerOn);
      Serial.println("##### POWER ON");
      break;

    case SENSOR_ID_POWER_OFF:
      MessageIR->setPowerOff(message.getBool());
      Serial.println("##### POWER OFF");
      break;

    case SENSOR_ID_SWING:
      MessageIR->setSwing(message.getBool());
      Serial.println("##### SWING");
      break;

    case SENSOR_ID_HOUR:
      MessageIR->setHour(message.getByte());
      hour = atoi(message.data);
      send(MyMessageHour.set(hour));
      Serial.print("##### HOUR: ");
      Serial.println(hour);
      break;

    case SENSOR_ID_FAN_MODE:
      MessageIR->setFanMode((IRP_FanMode_t)message.getByte());
      mode = atoi(message.data);
      send(MyMessageFanMode.set(mode));
      Serial.print("##### FAN: ");
      Serial.println(mode);
      break;

    case SENSOR_ID_FLOW_MODE:
      MessageIR->setFlowMode((IRP_FlowMode_t)message.getByte());
      mode = message.getByte();
      /*send(MyMessageFlowMode.set(mode));*/
      Serial.print("##### FLOW: ");
      Serial.println(mode);
      break;

    case SENSOR_ID_FAN_MAX:
      MessageIR->setFanMax(message.getBool());
      Serial.println("##### FAN MAX");
      break;

    case SENSOR_ID_TEMP_RELATIVE:
      MessageIR->setTemperatureRelative(message.getByte());
      relative = message.getByte();
      send(MyMessageTempRelative.set(relative));
      Serial.print("##### TEMP RELATIVE: ");
      Serial.println(relative);
      break;

    case SENSOR_ID_TEMPERATURE:
      MessageIR->setTemperature(atoi(message.data));
      temperature = atoi(message.data);
      send(MyMessageTemperature.set(temperature));
      Serial.print("##### TEMPERATURE: ");
      Serial.println(temperature);
      break;

    case SENSOR_ID_SEND:
      MessageIR->setStartUp(!IsHvacPowerOn);
      MessageIR->send();
      Serial.println("##### SEND");
      /* TODO send message to indicate IR was emit */
      break;

    case SENSOR_ID_CLEAR:
      MessageIR->clear();
      Serial.println("##### CLEAR");
      break;

    default:
      Serial.println("Message received with unknown sensor ID.");
      break;
  }

  Serial.println("### Message processed!");
}
