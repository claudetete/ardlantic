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

/* Power ON */
#define irp_POWER_ON_BYTE    (5)
#define irp_POWER_ON_BIT     (5)
#define irp_POWER_ON_MASK    (0x20)
#define irp_POWER_ON_START   (0)
#define irp_POWER_ON_STARTED (1)

/* Switch ON->OFF, OFF->, and flow mode */
#define irp_SWITCH_POWER_OR_MODE_BYTE   (10)
#define irp_SWITCH_POWER_OR_MODE_BIT    (2)
#define irp_SWITCH_POWER_OR_MODE_MASK   (0x04)
#define irp_SWITCH_POWER_OR_MODE_SWITCH (0)
#define irp_SWITCH_POWER_OR_MODE_KEEP   (1)

/* Swing */
#define irp_SWING_PART1_BYTE (8)
#define irp_SWING_PART1_BIT  (0)
#define irp_SWING_PART1_MASK (0x07)
#define irp_SWING_PART1_OFF  (0)
#define irp_SWING_PART1_ON   (7)
#define irp_SWING_PART2_BYTE (10)
#define irp_SWING_PART2_BIT  (1)
#define irp_SWING_PART2_MASK (0x02)
#define irp_SWING_PART2_OFF  (0)
#define irp_SWING_PART2_ON   (1)

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

/* Fan Max/Ion/Power OFF */
#define irp_FAN_MAX_OR_ION_OR_POWER_OFF_BYTE (5)
#define irp_FAN_MAX_OR_ION_OR_POWER_OFF_BIT  (4)
#define irp_FAN_MAX_OR_ION_OR_POWER_OFF_MASK (0x10)
#define irp_FAN_MAX_OR_ION_OR_POWER_OFF_TRUE  (0)
#define irp_FAN_MAX_OR_ION_OR_POWER_OFF_FALSE (1)

/* Fan Max */
#define irp_FAN_MAX_PART1_BYTE (5)
#define irp_FAN_MAX_PART1_BIT  (6)
#define irp_FAN_MAX_PART1_MASK (0x40)
#define irp_FAN_MAX_PART2_BYTE (10)
#define irp_FAN_MAX_PART2_BIT  (0)
#define irp_FAN_MAX_PART2_MASK (0x01)
#define irp_FAN_MAX_OFF  (0)
#define irp_FAN_MAX_ON   (1)

/* Temp relative */
#define irp_TEMP_RELATIVE_BYTE        (4)
#define irp_TEMP_RELATIVE_BIT         (4)
#define irp_TEMP_RELATIVE_MASK        (0xF0)
#define irp_TEMP_RELATIVE_SIGNED_MASK (0x8)
#define irp_TEMP_RELATIVE_NEGATIVE_2  (10)
#define irp_TEMP_RELATIVE_NEGATIVE_1  (9)
#define irp_TEMP_RELATIVE_0           (0)
#define irp_TEMP_RELATIVE_POSITIVE_1  (1)
#define irp_TEMP_RELATIVE_POSITIVE_2  (2)

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


/****** MACRO ******/
#define irp_SET_BITS(NAME, value)                                   \
  (_data[irp_##NAME##_BYTE] =                                       \
   /* keep only bits not used and */                                \
   /* and set only bits used */                                     \
   ((_data[irp_##NAME##_BYTE] & ~irp_##NAME##_MASK) |               \
    (uint8_t)((value << irp_##NAME##_BIT) & irp_##NAME##_MASK)))

#define irp_GET_BITS(NAME)                                              \
    ((_data[irp_##NAME##_BYTE] & irp_##NAME##_MASK) >> irp_##NAME##_BIT)


/****** TYPE ******/


/****** DATA ******/
const uint8_t irp_tab_init_data[IRP_FRAME_DATA_BYTE_NB] =
{
  0xAA, /*  byte0 */
  0x5A, /*  byte1 */
  0xCF, /*  byte2 */
  0x10, /*  byte3 */
  0x01, /*  byte4 */
  0x21, /*  byte5 */
  0x20, /*  byte6 */
  0x01, /*  byte7 */
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
  /*for (data_byte = IRP_FRAME_DATA_BYTE_NB - 1; data_byte >= 0; data_byte--)*/
  for (data_byte = 0; data_byte < IRP_FRAME_DATA_BYTE_NB; data_byte++)
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
    irp_SET_BITS(ION, irp_ION_ON);
    irp_SET_BITS(FAN_MAX_OR_ION_OR_POWER_OFF, irp_FAN_MAX_OR_ION_OR_POWER_OFF_TRUE);
    /* FAN MAX PART should be reseted */
    irp_SET_BITS(FAN_MAX_PART1, irp_FAN_MAX_OFF);
    irp_SET_BITS(FAN_MAX_PART2, irp_FAN_MAX_OFF);
  }
  else
  {
    /* ION OFF */
    irp_SET_BITS(ION, irp_ION_OFF);
    irp_SET_BITS(FAN_MAX_OR_ION_OR_POWER_OFF, irp_FAN_MAX_OR_ION_OR_POWER_OFF_FALSE);
  }
}

void IRP_Message::setStartUp(bool isStartUp)
{
  if (isStartUp)
  {
    /* Power ON */
    irp_SET_BITS(POWER_ON, irp_POWER_ON_START);
    /* Switch power from OFF to ON */
    irp_SET_BITS(SWITCH_POWER_OR_MODE, irp_SWITCH_POWER_OR_MODE_SWITCH);
  }
  else
  {
    /* Power already ON */
    irp_SET_BITS(POWER_ON, irp_POWER_ON_STARTED);
    /* Power not switched */
    irp_SET_BITS(SWITCH_POWER_OR_MODE, irp_SWITCH_POWER_OR_MODE_KEEP);
  }
}

void IRP_Message::setPowerOff(bool isPowerOff)
{
  if (isPowerOff)
  {
    /* Switch power from ON to OFF */
    irp_SET_BITS(SWITCH_POWER_OR_MODE, irp_SWITCH_POWER_OR_MODE_SWITCH);
    irp_SET_BITS(FAN_MAX_OR_ION_OR_POWER_OFF, irp_FAN_MAX_OR_ION_OR_POWER_OFF_TRUE);
  }
  else
  {
    /* power already OFF */
    irp_SET_BITS(SWITCH_POWER_OR_MODE, irp_SWITCH_POWER_OR_MODE_KEEP);
    irp_SET_BITS(FAN_MAX_OR_ION_OR_POWER_OFF, irp_FAN_MAX_OR_ION_OR_POWER_OFF_FALSE);
  }
}

void IRP_Message::setSwing(bool isEnableSwing)
{
  if (isEnableSwing)
  {
    /* Switch Swing to ON */
    irp_SET_BITS(SWING_PART1, irp_SWING_PART1_ON);
    irp_SET_BITS(SWING_PART2, irp_SWING_PART2_ON);
    /* FAN MAX PART should be reseted */
    irp_SET_BITS(FAN_MAX_PART1, irp_FAN_MAX_OFF);
    irp_SET_BITS(FAN_MAX_PART2, irp_FAN_MAX_OFF);
  }
  else
  {
    /* Switch Swing to OFF */
    irp_SET_BITS(SWING_PART1, irp_SWING_PART1_OFF);
    irp_SET_BITS(SWING_PART2, irp_SWING_PART2_OFF);
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
    irp_SET_BITS(HOUR, hour);
  }
  else
  {
    /* midnight 00h has value 24 */
    irp_SET_BITS(HOUR, irp_HOUR_00H);
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
  irp_SET_BITS(FAN_MODE, fanMode);
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
  irp_SET_BITS(FLOW_MODE, flowMode);
  /* Switch mode */
  irp_SET_BITS(SWITCH_POWER_OR_MODE, irp_SWITCH_POWER_OR_MODE_SWITCH);

  /* return 0 to use temperature offset, use temperature otherwise */
  return ((bool)(!flowMode));
}

void IRP_Message::setFanMax(bool isFanMax)
{
  if (isFanMax)
  {
    /* Fan Max ON */
    irp_SET_BITS(FAN_MAX_OR_ION_OR_POWER_OFF, irp_FAN_MAX_OR_ION_OR_POWER_OFF_TRUE);
    if ((irp_GET_BITS(ION) == irp_ION_OFF) &&
        (irp_GET_BITS(SWING_PART1) == irp_SWING_PART1_OFF))
    {
      irp_SET_BITS(FAN_MAX_PART1, irp_FAN_MAX_ON);
      irp_SET_BITS(FAN_MAX_PART2, irp_FAN_MAX_ON);
    }
    else
    {
      irp_SET_BITS(FAN_MAX_PART1, irp_FAN_MAX_OFF);
      irp_SET_BITS(FAN_MAX_PART2, irp_FAN_MAX_OFF);
    }
    /* Fan Max reset hour to 0 */
    irp_SET_BITS(HOUR, 0);
  }
  else
  {
    /* Fan Max OFF */
    irp_SET_BITS(FAN_MAX_OR_ION_OR_POWER_OFF, irp_FAN_MAX_OR_ION_OR_POWER_OFF_FALSE);
    irp_SET_BITS(FAN_MAX_PART1, irp_FAN_MAX_OFF);
    irp_SET_BITS(FAN_MAX_PART2, irp_FAN_MAX_OFF);
  }
}

void IRP_Message::setTemperatureRelative(uint8_t temperature)
{
  uint8_t temperature_computed = irp_TEMP_RELATIVE_0;

  /* when in range */
  switch (temperature)
  {
    case irp_TEMP_RELATIVE_NEGATIVE_2:
    case irp_TEMP_RELATIVE_NEGATIVE_1:
    case irp_TEMP_RELATIVE_0:
    case irp_TEMP_RELATIVE_POSITIVE_1:
    case irp_TEMP_RELATIVE_POSITIVE_2:
      temperature_computed = temperature;
      break;

    default:
      /* when out of range, use null value */
      temperature_computed = irp_TEMP_RELATIVE_0;
      break;
  }

  /* set value in output buffer */
  irp_SET_BITS(TEMP_RELATIVE, temperature_computed);
}

void IRP_Message::setTemperature(uint8_t temperature)
{
  uint8_t temperature_computed = 0;

  /* do not touch when temperature is 0, it means "not used" */
  if (temperature != 0)
  {
      /* temperature is used */
      if (temperature > irp_TEMPERATURE_MAX)
      {
          temperature = irp_TEMPERATURE_MAX;
      }
      else if ((temperature < irp_TEMPERATURE_MIN) && (temperature != 0))
      {
          temperature = irp_TEMPERATURE_MIN;
      }
      else
      {
          /* nothing to do: temperature in bounds */
      }
      temperature_computed = temperature - irp_TEMPERATURE_OFFSET;
  }

  /* set temperature in output buffer */
  irp_SET_BITS(TEMPERATURE, temperature_computed);
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
