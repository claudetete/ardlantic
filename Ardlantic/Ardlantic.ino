/* Dallas DS18B20 used is waterproof one from: */
/* https://www.aliexpress.com/item/Free-Shipping-1PCS-Digital-Temperature-Temp-Sensor-Probe-DS18B20-For-Thermometer-1m-Waterproof-100CM/32522322459.html */
/*  - Red    = VCC */
/*  - Yellow = Data */
/*  - Black  = GND */

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
#define MY_NODE_ID 44

/* MySensors will override usual function of a sketch, it for a node it need a gateway to start */
#include <MySensors.h>

#include <DallasTemperature.h>
#include <OneWire.h>

/* Sensor id to present and finally received */
#define SENSOR_ID_SEND              (0)
#define SENSOR_ID_SET_ION           (1)
#define SENSOR_ID_SET_STARTUP       (2)
#define SENSOR_ID_SET_SWING         (3)
#define SENSOR_ID_SET_HOUR          (4)
#define SENSOR_ID_SET_FAN_MODE      (5)
#define SENSOR_ID_SET_FLOW_MODE     (6)
#define SENSOR_ID_SET_FAN_MAX       (7)
#define SENSOR_ID_SET_TEMP_RELATIVE (8)
#define SENSOR_ID_SET_TEMPERATURE   (9)
#define SENSOR_ID_SET_POWER_OFF     (10)
#define SENSOR_ID_CLEAR             (11)
#define SENSOR_ID_GET_HVAC_ON_OFF   (12)
#define SENSOR_ID_GET_TEMPERATURE   (13)

/* PIN number of temperature sensor */
#define PIN_ID_TEMPERATURE_SENSOR (3)

/* PIN number of HVAC ON/OFF sensor */
#define PIN_ID_HVAC_ON_OFF_SENSOR          (A2)
#define HVAC_ON_OFF_HYSTERESIS_VOLTAGE_MIN (2)
#define HVAC_ON_OFF_HYSTERESIS_VOLTAGE_MAX (4)

/* 1 second in milliseconds (1000) */
#define POOLING_TIME (1000)

/* instance a IR message*/
IRP_Message* FrameIR;

/* flag indicate that HVAC is ON */
boolean IsHvacPowerOn;

/* declare messages about HVAC */
MyMessage MessageSetHour(SENSOR_ID_SET_HOUR, V_PERCENTAGE);
MyMessage MessageSetFanMode(SENSOR_ID_SET_FAN_MODE, V_PERCENTAGE);
MyMessage MessageSetFlowMode(SENSOR_ID_SET_FLOW_MODE, V_PERCENTAGE);
MyMessage MessageSetTemperature(SENSOR_ID_SET_TEMPERATURE, V_PERCENTAGE);
MyMessage MessageSetTempRelative(SENSOR_ID_SET_TEMP_RELATIVE, V_PERCENTAGE);
MyMessage MessageGetHvacOnOff(SENSOR_ID_GET_HVAC_ON_OFF, V_STATUS);
MyMessage MessageGetTemperature(SENSOR_ID_GET_HVAC_ON_OFF, V_TEMP);

/* Setup a oneWire instance to communicate with any OneWire devices */
OneWire OneWire(PIN_ID_TEMPERATURE_SENSOR);
/* Pass the OneWire reference to Dallas Temperature library */
DallasTemperature TemperatureSensors(&OneWire);
/* arrays to hold device address */
DeviceAddress HvacTemperature;

void before()
{
  /* everything before MySensors execution */
  FrameIR = new IRP_Message();

  /* Initialize ON/OFF sensor */
  pinMode(PIN_ID_HVAC_ON_OFF_SENSOR, INPUT);
  IsHvacPowerOn = false;

  /* Startup up the OneWire library */
  TemperatureSensors.begin();
}

void setup()
{
  if (!TemperatureSensors.getAddress(HvacTemperature, 0)) Serial.println("Unable to find address for Device 0");
  /* requestTemperatures() method will not block thread */
  TemperatureSensors.setWaitForConversion(false);

  /* everything to init after MySensors init */
	request(SENSOR_ID_SET_HOUR, V_DIMMER);
	request(SENSOR_ID_SET_FAN_MODE, V_DIMMER);
	request(SENSOR_ID_SET_FLOW_MODE, V_DIMMER);
  /* use V_DIMMER with V_PERCENTAGE to be able to retrieve a value */
  /* V_TEMP can not receive but only send temperature values */
  request(SENSOR_ID_SET_TEMPERATURE, V_DIMMER);
  request(SENSOR_ID_SET_TEMP_RELATIVE, V_DIMMER);
}

void presentation()
{
  /* Send the sketch version information to the gateway and Node */
  sendSketchInfo("AC Cat Room", "1.0");

  /* everything to present each sensors/actuators on this node to the gateway (so domotic box will register it) */
  present(SENSOR_ID_SEND, S_BINARY);
  present(SENSOR_ID_SET_ION, S_BINARY);
  present(SENSOR_ID_SET_STARTUP, S_BINARY);
  present(SENSOR_ID_SET_POWER_OFF, S_BINARY);
  present(SENSOR_ID_SET_SWING, S_BINARY);
  present(SENSOR_ID_SET_HOUR, S_DIMMER);
  present(SENSOR_ID_SET_FAN_MODE, S_WATER);
  present(SENSOR_ID_SET_FLOW_MODE, S_DIMMER);
  present(SENSOR_ID_SET_FAN_MAX, S_BINARY);
  present(SENSOR_ID_SET_TEMP_RELATIVE, S_DIMMER);
  present(SENSOR_ID_SET_TEMPERATURE, S_DIMMER);
  present(SENSOR_ID_CLEAR, S_BINARY);
}

void loop()
{
  static bool isFirstRun = true;

  if (isFirstRun)
  {
    isFirstRun = false;
    send(MessageSetHour.set((int16_t)0));
    send(MessageSetFanMode.set((int16_t)0));
    send(MessageSetFlowMode.set((int16_t)0));
    send(MessageSetTemperature.set((int16_t)0));
    send(MessageSetTempRelative.set((int16_t)0));
  }

  /* read voltage using voltage divider */
  /* divide by 11 because with 1MΩ and 100KΩ */
  /* which means protection about overvoltage (max 55V = 5V * 11) */
  float power_voltage = ((float)analogRead(PIN_ID_HVAC_ON_OFF_SENSOR) * (5.0 / 1023.0)) * 11;

  /* detect with hysteresis to avoid unwanted switching */
  if (power_voltage <= HVAC_ON_OFF_HYSTERESIS_VOLTAGE_MIN)
  {
    IsHvacPowerOn = false;
    Serial.print("Not Power: ");
    Serial.println(power_voltage);
  }
  else if (power_voltage >= HVAC_ON_OFF_HYSTERESIS_VOLTAGE_MAX)
  {
    IsHvacPowerOn = true;
    Serial.print("Power: ");
    Serial.println(power_voltage);
  }
  send(MessageGetHvacOnOff.set(IsHvacPowerOn ? "1" : "0"));

  /* Fetch temperatures from Dallas sensors */
  TemperatureSensors.requestTemperatures();

  /* query conversion time and sleep until conversion completed */
  int16_t conversionTime = TemperatureSensors.millisToWaitForConversion(TemperatureSensors.getResolution());
  /* sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater) */
  wait(conversionTime);

  /* Fetch and round temperature to one decimal */
  float temperature = static_cast<float>(static_cast<int>(TemperatureSensors.getTempC(HvacTemperature) * 10.)) / 10.;

  /* Send in read temperature */
  send(MessageGetTemperature.set(temperature, 1));
  Serial.print("Temp C: ");
  Serial.print(temperature);
  Serial.println();

  wait(POOLING_TIME);
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
    case SENSOR_ID_SET_ION:
      FrameIR->setIon(message.getBool());
      Serial.println("##### ION");
      break;

    case SENSOR_ID_SET_STARTUP:
      FrameIR->setStartUp(message.getBool() && !IsHvacPowerOn);
      Serial.println("##### POWER ON");
      break;

    case SENSOR_ID_SET_POWER_OFF:
      FrameIR->setPowerOff(message.getBool());
      Serial.println("##### POWER OFF");
      break;

    case SENSOR_ID_SET_SWING:
      FrameIR->setSwing(message.getBool());
      Serial.println("##### SWING");
      break;

    case SENSOR_ID_SET_HOUR:
      FrameIR->setHour(message.getByte());
      hour = atoi(message.data);
      send(MessageSetHour.set(hour));
      Serial.print("##### HOUR: ");
      Serial.println(hour);
      break;

    case SENSOR_ID_SET_FAN_MODE:
      FrameIR->setFanMode((IRP_FanMode_t)message.getByte());
      mode = atoi(message.data);
      send(MessageSetFanMode.set(mode));
      Serial.print("##### FAN: ");
      Serial.println(mode);
      break;

    case SENSOR_ID_SET_FLOW_MODE:
      FrameIR->setFlowMode((IRP_FlowMode_t)message.getByte());
      mode = message.getByte();
      /*send(MessageSetFlowMode.set(mode));*/
      Serial.print("##### FLOW: ");
      Serial.println(mode);
      break;

    case SENSOR_ID_SET_FAN_MAX:
      FrameIR->setFanMax(message.getBool());
      Serial.println("##### FAN MAX");
      break;

    case SENSOR_ID_SET_TEMP_RELATIVE:
      FrameIR->setTemperatureRelative(message.getByte());
      relative = message.getByte();
      send(MessageSetTempRelative.set(relative));
      Serial.print("##### TEMP RELATIVE: ");
      Serial.println(relative);
      break;

    case SENSOR_ID_SET_TEMPERATURE:
      FrameIR->setTemperature(atoi(message.data));
      temperature = atoi(message.data);
      send(MessageSetTemperature.set(temperature));
      Serial.print("##### TEMPERATURE: ");
      Serial.println(temperature);
      break;

    case SENSOR_ID_SEND:
      FrameIR->setStartUp(!IsHvacPowerOn);
      FrameIR->send();
      Serial.println("##### SEND");
      /* TODO send message to indicate IR was emit */
      break;

    case SENSOR_ID_CLEAR:
      FrameIR->clear();
      Serial.println("##### CLEAR");
      break;

    default:
      Serial.println("Message received with unknown sensor ID.");
      break;
  }

  Serial.println("### Message processed!");
}
