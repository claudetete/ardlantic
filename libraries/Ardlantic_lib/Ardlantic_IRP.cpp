/* Infra Red Protocol layer using Infra Red Driver */
/* about Atlantic AMC12XPM.UI remote control */

#include <Arduino.h>

#include "Ardlantic_IRD.h"

#include "Ardlantic_IRP.h"


/****** SYMBOL ******/
/* IR Command duration for Altantic AMC12XPM.UI remote control */
#define irp_COMMAND_DURATION_START (3750)
#define irp_COMMAND_DURATION_FIRST (1950)
#define irp_COMMAND_DURATION_PAUSE (400)
#define irp_COMMAND_DURATION_BIT_0 (550)
#define irp_COMMAND_DURATION_BIT_1 (1450)

/* ION part */
#define irp_ION_BYTE (11)
#define irp_ION_BIT  (2)
#define irp_ION_MASK (0x04)
#define irp_ION_OFF  (0)
#define irp_ION_ON   (1)

/* StartUp */
#define irp_STARTUP_BYTE1 (5)
#define irp_STARTUP_BIT1  (5)
#define irp_STARTUP_MASK1 (0x20)
#define irp_STARTUP_BYTE2 (10)
#define irp_STARTUP_BIT2  (2)
#define irp_STARTUP_MASK2 (0x04)
#define irp_STARTUP_START   (0)
#define irp_STARTUP_STARTED (1)

/* Swing */
#define irp_SWING_BYTE1 (8)
#define irp_SWING_BIT1  (0)
#define irp_SWING_MASK1 (0x07)
#define irp_SWING_BYTE2 (10)
#define irp_SWING_BIT2  (1)
#define irp_SWING_MASK2 (0x02)
#define irp_SWING_OFF1 (0)
#define irp_SWING_ON1  (7)
#define irp_SWING_OFF2 (0)
#define irp_SWING_ON2  (1)

/* Hour */
#define irp_HOUR_BYTE (7)
#define irp_HOUR_BIT  (0)
#define irp_HOUR_MASK (0x1F)
/* from 1 to 23 about 01h to 23h, but 24 for 00h */
#define irp_HOUR_00H (24)

/* Fan Mode */
#define irp_FAN_MODE_BYTE (6)
#define irp_FAN_MODE_BIT  (4)
#define irp_FAN_MODE_MASK (0xF0)

/* Flow Mode */
#define irp_FLOW_MODE_BYTE (6)
#define irp_FLOW_MODE_BIT  (0)
#define irp_FLOW_MODE_MASK (0x03)

/* Max/Ion/Timer */
#define irp_FAN_MAX_BYTE (5)
#define irp_FAN_MAX_BIT  (4)
#define irp_FAN_MAX_MASK (0x10)
#define irp_FAN_MAX_OFF (1)
#define irp_FAN_MAX_ON  (0)

/* Temp relative */
#define irp_TEMP_RELATIVE_BYTE        (4)
#define irp_TEMP_RELATIVE_BIT         (4)
#define irp_TEMP_RELATIVE_MASK        (0xF0)
#define irp_TEMP_RELATIVE_SIGNED_MASK (0x8)
#define irp_TEMP_RELATIVE_NUM_MAX     (7)

/* Temperature */
#define irp_TEMPERATURE_BYTE   (4)
#define irp_TEMPERATURE_BIT    (0)
#define irp_TEMPERATURE_MASK   (0x0F)
#define irp_TEMPERATURE_OFFSET (17)
#define irp_TEMPERATURE_MIN    (18)
#define irp_TEMPERATURE_MAX    (32)

/* Parity */
#define irp_PARITY_BYTE      (12)
#define irp_PARITY_BIT_START (4)
#define irp_PARITY_BIT_MASK  (0xF0)
#define irp_PARITY_BIT_NB    (4)


/****** TYPE ******/


/****** DATA ******/
const uint8_t irp_tab_init_data[IRP_FRAME_DATA_BYTE_NB] =
{
  0xAA, /*  byte0 */
  0x5A, /*  byte1 */
  0xCF, /*  byte2 */
  0x10, /*  byte3 */
  0x00, /*  byte4 */
  0x01, /*  byte5 */
  0x00, /*  byte6 */
  0x00, /*  byte7 */
  0x08, /*  byte8 */
  0x80, /*  byte9 */
  0x00, /* byte10 */
  0xF0, /* byte11 */
  0x01  /* byte12 */
};


/****** PUBLIC METHOD ******/
IRP_Message::IRP_Message()
{
  this->clear();
}

void IRP_Message::clear(void)
{
  int index = 0;

  /* reset whole output buffer with init values */
  for (index = 0; index < IRP_FRAME_DATA_BYTE_NB; index++)
  {
    _data[index] = irp_tab_init_data[index];
  }
}

void IRP_Message::send(void)
{
  int data_bit      = 0;
  int data_byte     = 0;
  int command_index = 0;

  Serial.println("Compute IR frame parity...");
  /* compute and set parity on data part to have a valid frame */
  this->setParity();

  Serial.println("Build IR frame duration...");
  /* init rx command buffer for header */
  _buffer[command_index] = irp_COMMAND_DURATION_START;
  command_index++;
  _buffer[command_index] = irp_COMMAND_DURATION_FIRST;
  command_index++;
  _buffer[command_index] = irp_COMMAND_DURATION_PAUSE;
  command_index++;

  /* fill rx command buffer with data part from last word to first (msb to lsb) */
  for (data_byte = IRP_FRAME_DATA_BYTE_NB - 1; data_byte >= 0; data_byte--)
  {
    /* in bit order lsb to msb about a byte */
    for (data_bit = 0; data_bit < 8; data_bit++)
    {
      /* data */
      if ((_data[data_byte] >> data_bit) & 0x1)
      {
        _buffer[command_index] = irp_COMMAND_DURATION_BIT_1;
      }
      else
      {
        _buffer[command_index] = irp_COMMAND_DURATION_BIT_0;
      }
      command_index++;

      /* space/pause */
      _buffer[command_index] = irp_COMMAND_DURATION_PAUSE;
      command_index++;
    }
  }

  /* send IR frame */
  Serial.println("Send IR frame...");
  IRD_sendRaw(_buffer, (uint8_t)IRP_FRAME_COMMAND_NB, (uint8_t)38);
  Serial.println("Send IR frame...Done");
}

void IRP_Message::setIon(bool isEnableIon)
{
  if (isEnableIon)
  {
    /* ION ON */
    _data[irp_ION_BYTE] =
      ((_data[irp_ION_BYTE] & ~irp_ION_MASK) |                  /* keep only bits not used and */
       (uint8_t)((irp_ION_ON << irp_ION_BIT) & irp_ION_MASK)); /* set only bits used */

    /* FAN MAX is set to ON when ION is ON */
    _data[irp_FAN_MAX_BYTE] =
      ((_data[irp_FAN_MAX_BYTE] & ~irp_FAN_MAX_MASK) |                      /* keep only bits not used and */
       (uint8_t)((irp_FAN_MAX_ON << irp_FAN_MAX_BIT) & irp_FAN_MAX_MASK)); /* set only bits used */
  }
  else
  {
    /* ION OFF */
    _data[irp_ION_BYTE] =
      ((_data[irp_ION_BYTE] & ~irp_ION_MASK) |                   /* keep only bits not used and */
       (uint8_t)((irp_ION_OFF << irp_ION_BIT) & irp_ION_MASK)); /* set only bits used */

    /* FAN MAX is set to OFF when ION is OFF */
    _data[irp_FAN_MAX_BYTE] =
      ((_data[irp_FAN_MAX_BYTE] & ~irp_FAN_MAX_MASK) |                       /* keep only bits not used and */
       (uint8_t)((irp_FAN_MAX_OFF << irp_FAN_MAX_BIT) & irp_FAN_MAX_MASK)); /* set only bits used */
  }
}

void IRP_Message::setStartUp(bool isStartUp)
{
  if (isStartUp)
  {
    /* To start up*/

    /* Start Up bit1 */
    _data[irp_STARTUP_BYTE1] =
      ((_data[irp_STARTUP_BYTE1] & ~irp_STARTUP_MASK1) |
       (uint8_t)((irp_STARTUP_START << irp_STARTUP_BIT1) & irp_STARTUP_MASK1));

    /* Start Up bit2 */
    _data[irp_STARTUP_BYTE2] =
      ((_data[irp_STARTUP_BYTE2] & ~irp_STARTUP_MASK2) |
       (uint8_t)((irp_STARTUP_START << irp_STARTUP_BIT2) & irp_STARTUP_MASK2));
  }
  else
  {
    /* Already started */

    /* Start Up bit1 */
    _data[irp_STARTUP_BYTE1] =
      ((_data[irp_STARTUP_BYTE1] & ~irp_STARTUP_MASK1) |
       (uint8_t)((irp_STARTUP_STARTED << irp_STARTUP_BIT1) & irp_STARTUP_MASK1));
    /* Start Up bit1 */
    _data[irp_STARTUP_BYTE2] =
      ((_data[irp_STARTUP_BYTE2] & ~irp_STARTUP_MASK2) |
       (uint8_t)((irp_STARTUP_STARTED << irp_STARTUP_BIT2) & irp_STARTUP_MASK2));
  }
}

void IRP_Message::setSwing(bool isEnableSwing)
{
  if (isEnableSwing)
  {
    /* Switch Swing to ON */

    /* Swing bit1 */
    _data[irp_SWING_BYTE1] =
      ((_data[irp_SWING_BYTE1] & ~irp_SWING_MASK1) |
       (uint8_t)((irp_SWING_ON1 << irp_SWING_BIT1) & irp_SWING_MASK1));

    /* Swing bit2 */
    _data[irp_SWING_BYTE2] =
      ((_data[irp_SWING_BYTE2] & ~irp_SWING_MASK2) |
       (uint8_t)((irp_SWING_ON2 << irp_SWING_BIT2) & irp_SWING_MASK2));
  }
  else
  {
    /* Switch Swing to OFF */

    /* Swing bit1 */
    _data[irp_SWING_BYTE1] =
      ((_data[irp_SWING_BYTE1] & ~irp_SWING_MASK1) |
       (uint8_t)((irp_SWING_OFF1 << irp_SWING_BIT1) & irp_SWING_MASK1));

    /* Swing bit2 */
    _data[irp_SWING_BYTE2] =
      ((_data[irp_SWING_BYTE2] & ~irp_SWING_MASK2) |
       (uint8_t)((irp_SWING_OFF2 << irp_SWING_BIT2) & irp_SWING_MASK2));
  }
}

void IRP_Message::setHour(uint8_t hour)
{
  if (hour > 24)
  {
    hour = 0;
  }
  else
  {
    /* hour is OK: nothing to do */
  }

  if (hour)
  {
    /* from 01h to 23h, same value from 1 to 23 */
    _data[irp_HOUR_BYTE] =
      ((_data[irp_HOUR_BYTE] & ~irp_HOUR_MASK) |
       (uint8_t)((hour << irp_HOUR_BIT) & irp_HOUR_MASK));
  }
  else
  {
    /* midnight 00h has value 24 */
    _data[irp_HOUR_BYTE] =
      ((_data[irp_HOUR_BYTE] & ~irp_HOUR_MASK) |
       (uint8_t)((irp_HOUR_00H << irp_HOUR_BIT) & irp_HOUR_MASK));
  }
}

void IRP_Message::setFanMode(IRP_FanMode_t fanMode)
{
  switch (fanMode)
  {
    /* when enum value is valid keep it */
    case IRP_FAN_MODE_LOW:
    case IRP_FAN_MODE_MIDDLE:
    case IRP_FAN_MODE_HIGH:
    case IRP_FAN_MODE_AUTO:
      break;

    /* when enum value is invalid force it to auto */
    default:
      fanMode = IRP_FAN_MODE_AUTO;
      break;
  }

  /* Fan */
  _data[irp_FAN_MODE_BYTE] =
    ((_data[irp_FAN_MODE_BYTE] & ~irp_FAN_MODE_MASK) |
     (uint8_t)((fanMode << irp_FAN_MODE_BIT) & irp_FAN_MODE_MASK));
}

bool IRP_Message::setFlowMode(IRP_FlowMode_t flowMode)
{
  switch (flowMode)
  {
    /* when enum value is valid keep it */
    case IRP_FLOW_MODE_HOT:
    case IRP_FLOW_MODE_COLD:
    case IRP_FLOW_MODE_WET:
    case IRP_FLOW_MODE_AUTO:
      break;

    /* when enum value is invalid force it to auto */
    default:
      flowMode = IRP_FLOW_MODE_AUTO;
      break;
  }

  /* Flow Mode */
  _data[irp_FLOW_MODE_BYTE] =
    ((_data[irp_FLOW_MODE_BYTE] & ~irp_FLOW_MODE_MASK) |
     (uint8_t)((flowMode << irp_FLOW_MODE_BIT) & irp_FLOW_MODE_MASK));

  /* return 0 to use temperature offset, use temperature otherwise */
  return ((bool)(!flowMode));
}

void IRP_Message::setFanMax(bool isFanMax)
{
  if (isFanMax)
  {
    /* Fan Max ON */

    _data[irp_FAN_MAX_BYTE] =
      ((_data[irp_FAN_MAX_BYTE] & ~irp_FAN_MAX_MASK) |
       (unsigned char)((irp_FAN_MAX_ON << irp_FAN_MAX_BIT) & irp_FAN_MAX_MASK));
  }
  else
  {
    /* Fan Max OFF */

    _data[irp_FAN_MAX_BYTE] =
      ((_data[irp_FAN_MAX_BYTE] & ~irp_FAN_MAX_MASK) |
       (unsigned char)((irp_FAN_MAX_OFF << irp_FAN_MAX_BIT) & irp_FAN_MAX_MASK));
  }
}

void IRP_Message::setTemperatureRelative(int16_t temperature)
{
  uint8_t temperature_computed = 0;

  /* when negative value */
  if (temperature < 0)
  {
    /* add signed value */
    temperature_computed = irp_TEMP_RELATIVE_SIGNED_MASK;
    /* keep only numerical value without signed */
    temperature *= -1;
  }
  else
  {
    /* nothing to do: positive value */
  }

  /* limit numerical value maximum */
  if (temperature > irp_TEMP_RELATIVE_NUM_MAX)
  {
    /* cannot exceed maximum */
    temperature = irp_TEMP_RELATIVE_NUM_MAX;
  }
  else
  {
    /* nothing to do: numerical value so start from 0 until max */
  }

  /* set numerical value */
  temperature_computed |= (uint8_t)temperature;

  /* set value in output buffer */
  _data[irp_TEMP_RELATIVE_BYTE] =
    ((_data[irp_TEMP_RELATIVE_BYTE] & ~irp_TEMP_RELATIVE_MASK) |
     (uint8_t)((temperature_computed << irp_TEMP_RELATIVE_BIT) & irp_TEMP_RELATIVE_MASK));
}

void IRP_Message::setTemperature(uint8_t temperature)
{
  uint8_t temperature_computed = 0;

  if (!temperature)
  {
    /* temperature not used = 0 */
    temperature_computed = 0;
  }
  else
  {
    /* temperature is used */
    if (temperature > irp_TEMPERATURE_MAX)
    {
      temperature_computed = irp_TEMPERATURE_MAX - irp_TEMPERATURE_OFFSET;
    }
    else if (temperature < irp_TEMPERATURE_MIN)
    {
      temperature_computed = irp_TEMPERATURE_MIN - irp_TEMPERATURE_OFFSET;
    }
    else
    {
      /* nothing to do: temperature in bounds */
    }
  }

  /* set temperature in output buffer */
  _data[irp_TEMPERATURE_BYTE] =
    ((_data[irp_TEMPERATURE_BYTE] & ~irp_TEMPERATURE_MASK) |
     (uint8_t)((temperature_computed << irp_TEMPERATURE_BIT) & irp_TEMPERATURE_MASK));
}


/****** PRIVATE METHOD ******/
void IRP_Message::setParity(void)
{
  uint8_t index_bit = 0;
  uint8_t index_byte = 0;
  uint8_t parity_bit = 0;

  /* Parity is done by bit on word of 4 bits */
  /* bit0 of parity word (4bits) is a parity on each bit0 of all other words (4 bits) of frame */
  /* same for bit1 of parity, each bit1 of all other words, etc */

  /* reset PARITY part of frame */
  _data[irp_PARITY_BYTE] = (_data[irp_PARITY_BYTE] & ~irp_PARITY_BIT_MASK);

  /* compute parity for each bit */
  for (index_bit = 0; index_bit < irp_PARITY_BIT_NB; index_bit++)
  {
    /* init computed parity */
    parity_bit = 0;
    /* browse each byte to add bit by word of 4 bits */
    for (index_byte = 0; index_byte < IRP_FRAME_DATA_BYTE_NB; index_byte++)
    {
      /* add current bit position of first 4bits of frame byte */
      parity_bit += ((_data[index_byte] >> index_bit) & 0x1);
      /* add current bit position of second 4bits of frame byte */
      parity_bit += ((_data[index_byte] >> (index_bit + 4)) & 0x1);
    }
    /* compute parity (modulo 2 on sum) */
    parity_bit &= 0x1;

    /* set bit of parity with result */
    _data[irp_PARITY_BYTE] |= (uint8_t)((parity_bit << (irp_PARITY_BIT_START + index_bit)) & irp_PARITY_BIT_MASK);
  }
}
