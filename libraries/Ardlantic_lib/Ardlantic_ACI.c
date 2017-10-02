#include "Ardlantic_ACI.h"

typedef struct
{
  unsigned char byte;
  unsigned char bit;
  unsigned char mask;
  unsigned char value;
} aci_type_ir_frame_byte;

#define aci_IR_SPARE_NB (10)

#define aci_IR_HEADER_BYTE_NB (4)


/* InfraRed frame ION */
#define aci_IR_ION_BYTE (11)
#define aci_IR_ION_BIT  (2)
#define aci_IR_ION_MASK (0x1)
#define aci_IR_ION_OFF  (0)
#define aci_IR_ION_ON   (1)

/* InfraRed frame StartUp */
#define aci_IR_STARTUP_BYTE1 (5)
#define aci_IR_STARTUP_BIT1  (5)
#define aci_IR_STARTUP_MASK1 (0x1)
#define aci_IR_STARTUP_BYTE2 (10)
#define aci_IR_STARTUP_BIT2  (2)
#define aci_IR_STARTUP_MASK2 (0x1)
#define aci_IR_STARTUP_START   (0)
#define aci_IR_STARTUP_STARTED (1)

/* InfraRed frame Swing */
#define aci_IR_SWING_BYTE1 (8)
#define aci_IR_SWING_BIT1  (0)
#define aci_IR_SWING_MASK1 (0x7)
#define aci_IR_SWING_BYTE2 (10)
#define aci_IR_SWING_BIT2  (1)
#define aci_IR_SWING_MASK2 (0x1)
#define aci_IR_SWING_OFF1 (0)
#define aci_IR_SWING_ON1  (7)
#define aci_IR_SWING_OFF2 (0)
#define aci_IR_SWING_ON2  (1)

/* InfraRed frame Hour */
#define aci_IR_HOUR_BYTE (7)
#define aci_IR_HOUR_BIT  (0)
#define aci_IR_HOUR_MASK (0x1F)
/* from 1 to 24 (01h to 00h) */
#define aci_IR_HOUR_01H (1)
#define aci_IR_HOUR_02H (2)
/* ... */
#define aci_IR_HOUR_23H (23)
#define aci_IR_HOUR_00H (24)

/* InfraRed frame Fan */
#define aci_IR_FAN_BYTE (6)
#define aci_IR_FAN_BIT  (4)
#define aci_IR_FAN_MASK (0xF)
#define aci_IR_FAN_NB   (4)
#define aci_RF_FAN_AUTO    (0)
#define aci_IR_FAN_AUTO    (2)
#define aci_RF_FAN_LOW     (1)
#define aci_IR_FAN_LOW     (3)
#define aci_RF_FAN_MIDDLE  (2)
#define aci_IR_FAN_MIDDLE  (5)
#define aci_RF_FAN_HIGH    (3)
#define aci_IR_FAN_HIGH    (7)

/* InfraRed frame Mode */
#define aci_IR_MODE_BYTE (6)
#define aci_IR_MODE_BIT  (0)
#define aci_IR_MODE_MASK (0x3)
#define aci_IR_MODE_NB   (4)
#define aci_IR_MODE_AUTO (0)
#define aci_IR_MODE_HOT  (1)
#define aci_IR_MODE_COLD (2)
#define aci_IR_MODE_WET  (3)

/* InfraRed frame Max/Ion/Timer */
#define aci_IR_MAX_BYTE (5)
#define aci_IR_MAX_BIT  (4)
#define aci_IR_MAX_MASK (0x1)
#define aci_IR_MAX_OFF (1)
#define aci_IR_MAX_ON  (0)

/* InfraRed frame Temp auto */
#define aci_IR_TEMPAUTO_BYTE        (4)
#define aci_IR_TEMPAUTO_BIT         (4)
#define aci_IR_TEMPAUTO_MASK        (0xF)
#define aci_IR_TEMPAUTO_SIGNED_MASK (0x8)
#define aci_IR_TEMPAUTO_NUM_MAX     (7)

/* InfraRed frame Temperature */
#define aci_IR_TEMPERATURE_BYTE   (4)
#define aci_IR_TEMPERATURE_BIT    (0)
#define aci_IR_TEMPERATURE_MASK   (0xF)
#define aci_IR_TEMPERATURE_OFFSET (17)
#define aci_IR_TEMPERATURE_MIN    (18)
#define aci_IR_TEMPERATURE_MAX    (32)

/* InfraRed Mode from RF Mode */
int aci_tab_ir_fan_rf[aci_IR_FAN_NB] =
{
  aci_IR_FAN_AUTO,             /* aci_RF_FAN_AUTO */
  aci_IR_FAN_LOW,              /* aci_RF_FAN_LOW */
  aci_IR_FAN_MIDDLE,           /* aci_RF_FAN_MIDDLE */
  aci_IR_FAN_HIGH              /* aci_RF_FAN_HIGH */
};

aci_type_ir_frame_byte aci_tab_ir_spare_init[aci_IR_SPARE_NB] =
{
  /* SPARE0 */
  {
    5,                          /* byte */
    0,                          /* bit */
    0xF,                        /* mask */
    0x1                         /* value */
  },
  /* SPARE1 */
  {
    5,                          /* byte */
    6,                          /* bit */
    0x1,                        /* mask */
    0x0                         /* value */
  },
  /* SPARE2 */
  {
    6,                          /* byte */
    2,                          /* bit */
    0x3,                        /* mask */
    0x0                         /* value */
  },
  /* SPARE3 */
  {
    8,                          /* byte */
    3,                          /* bit */
    0x1F,                       /* mask */
    0x01                        /* value */
  },
  /* SPARE4 */
  {
    9,                          /* byte */
    0,                          /* bit */
    0xFF,                       /* mask */
    0x80                        /* value */
  },
  /* SPARE5 */
  {
    10,                         /* byte */
    0,                          /* bit */
    0x1,                        /* mask */
    0x0                         /* value */
  },
  /* SPARE6 */
  {
    10,                         /* byte */
    3,                          /* bit */
    0x1F,                       /* mask */
    0x00                        /* value */
  },
  /* SPARE7 */
  {
    11,                         /* byte */
    0,                          /* bit */
    0x3,                        /* mask */
    0x0                         /* value */
  },
  /* SPARE8 */
  {
    11,                         /* byte */
    3,                          /* bit */
    0x1F,                       /* mask */
    0x1E                        /* value */
  },
  /* SPARE9 */
  {
    12,                         /* byte */
    0,                          /* bit */
    0xF,                        /* mask */
    0x1                         /* value */
  }
};

aci_type_ir_frame_byte aci_tab_ir_header[aci_IR_HEADER_BYTE_NB] =
{
  /* HEADER0 */
  {
    0,                          /* byte */
    0,                          /* bit */
    0xFF,                       /* mask */
    0xAA                        /* value */
  },
  /* HEADER1 */
  {
    1,                          /* byte */
    0,                          /* bit */
    0xFF,                       /* mask */
    0x5A                        /* value */
  },
  /* HEADER2 */
  {
    2,                          /* byte */
    0,                          /* bit */
    0xFF,                       /* mask */
    0xCF                        /* value */
  },
  /* HEADER3 */
  {
    3,                          /* byte */
    0,                          /* bit */
    0xFF,                       /* mask */
    0x10                        /* value */
  }
};

/* Decoder of RF */
int ACI_decodeRfIon(const unsigned int input[])
{

}

int ACI_decodeRfOn(const unsigned int input[])
{

}

int ACI_decodeRfSwing(const unsigned int input[])
{

}

int ACI_decodeRfTimer(const unsigned int input[])
{

}

int ACI_decodeRfHour(const unsigned int input[])
{

}

int ACI_decodeRfFan(const unsigned int input[])
{

}

int ACI_decodeRfMode(const unsigned int input[])
{

}

int ACI_decodeRfMax(const unsigned int input[])
{

}

int ACI_decodeRfTemperatureAuto(const unsigned int input[])
{

}

int ACI_decodeRfTemperature(const unsigned int input[])
{

}


/* encoder of IR */
void ACI_encodeIrInit(unsigned char output[])
{
  int index = 0;

  /* reset whole output buffer */
  for (index = 0; index < ACI_IR_FRAME_DATA_BYTE_NB; index++)
  {
    output[index] = 0;
  }

  /* init hard coded or unused value in frame */
  for (index = 0; index < aci_IR_SPARE_NB; index++)
  {
    output[aci_tab_ir_spare_init[index].byte] |=
      ((aci_tab_ir_spare_init[index].value & aci_tab_ir_spare_init[index].mask) << aci_tab_ir_spare_init[index].bit);
  }
}

void ACI_encodeIrHeader(unsigned char output[])
{
  int index = 0;

  /* init header value in frame */
  for (index = 0; index < aci_IR_HEADER_BYTE_NB; index++)
  {
    output[aci_tab_ir_header[index].byte] |=
      ((aci_tab_ir_header[index].value & aci_tab_ir_header[index].mask) << aci_tab_ir_header[index].bit);
  }
}

void ACI_encodeIrIon(unsigned char output[], int value)
{
  if (value)
  {
    /* ION ON */
    output[aci_IR_ION_BYTE] =
      ((output[aci_IR_ION_BYTE] & ~aci_IR_ION_MASK) |
       (unsigned char)((aci_IR_ION_ON << aci_IR_ION_BIT) & aci_IR_ION_MASK));

    output[aci_IR_MAX_BYTE] =
      ((output[aci_IR_MAX_BYTE] & ~aci_IR_MAX_MASK) |
       (unsigned char)((aci_IR_MAX_ON << aci_IR_MAX_BIT) & aci_IR_MAX_MASK));
  }
  else
  {
    /* ION OFF */
    output[aci_IR_ION_BYTE] =
      ((output[aci_IR_ION_BYTE] & ~aci_IR_ION_MASK) |
       (unsigned char)((aci_IR_ION_OFF << aci_IR_ION_BIT) & aci_IR_ION_MASK));

    output[aci_IR_MAX_BYTE] =
      ((output[aci_IR_MAX_BYTE] & ~aci_IR_MAX_MASK) |
       (unsigned char)((aci_IR_MAX_OFF << aci_IR_MAX_BIT) & aci_IR_MAX_MASK));
  }
}

void ACI_encodeIrStartUp(unsigned char output[], int value)
{
  if (value)
  {
    /* Start Up */
    output[aci_IR_STARTUP_BYTE1] =
      ((output[aci_IR_STARTUP_BYTE1] & ~aci_IR_STARTUP_MASK1) |
       (unsigned char)((aci_IR_STARTUP_START << aci_IR_STARTUP_BIT1) & aci_IR_STARTUP_MASK1));

    output[aci_IR_STARTUP_BYTE2] =
      ((output[aci_IR_STARTUP_BYTE2] & ~aci_IR_STARTUP_MASK2) |
       (unsigned char)((aci_IR_STARTUP_START << aci_IR_STARTUP_BIT2) & aci_IR_STARTUP_MASK2));
  }
  else
  {
    /* Already started */
    output[aci_IR_STARTUP_BYTE1] =
      ((output[aci_IR_STARTUP_BYTE1] & ~aci_IR_STARTUP_MASK1) |
       (unsigned char)((aci_IR_STARTUP_STARTED << aci_IR_STARTUP_BIT1) & aci_IR_STARTUP_MASK1));

    output[aci_IR_STARTUP_BYTE2] =
      ((output[aci_IR_STARTUP_BYTE2] & ~aci_IR_STARTUP_MASK2) |
       (unsigned char)((aci_IR_STARTUP_STARTED << aci_IR_STARTUP_BIT2) & aci_IR_STARTUP_MASK2));
  }
}

void ACI_encodeIrSwing(unsigned char output[], int value)
{
  if (value)
  {
    /* Swing ON */
    output[aci_IR_SWING_BYTE1] =
      ((output[aci_IR_SWING_BYTE1] & ~aci_IR_SWING_MASK1) |
       (unsigned char)((aci_IR_SWING_ON1 << aci_IR_SWING_BIT1) & aci_IR_SWING_MASK1));

    output[aci_IR_SWING_BYTE2] =
      ((output[aci_IR_SWING_BYTE2] & ~aci_IR_SWING_MASK2) |
       (unsigned char)((aci_IR_SWING_ON2 << aci_IR_SWING_BIT2) & aci_IR_SWING_MASK2));
  }
  else
  {
    /* Swing OFF */
    output[aci_IR_SWING_BYTE1] =
      ((output[aci_IR_SWING_BYTE1] & ~aci_IR_SWING_MASK1) |
       (unsigned char)((aci_IR_SWING_OFF1 << aci_IR_SWING_BIT1) & aci_IR_SWING_MASK1));

    output[aci_IR_SWING_BYTE2] =
      ((output[aci_IR_SWING_BYTE2] & ~aci_IR_SWING_MASK2) |
       (unsigned char)((aci_IR_SWING_OFF2 << aci_IR_SWING_BIT2) & aci_IR_SWING_MASK2));
  }
}

void ACI_encodeIrTimer(unsigned char output[], int value)
{
  /* TODO, but domotic box can do the same */
}

void ACI_encodeIrHour(unsigned char output[], int value)
{
  if (value)
  {
    /* on 24h but from 01h to 23h */
    output[aci_IR_HOUR_BYTE] =
      ((output[aci_IR_HOUR_BYTE] & ~aci_IR_HOUR_MASK) |
       (unsigned char)((value << aci_IR_HOUR_BIT) & aci_IR_HOUR_MASK));
  }
  else
  {
    /* On 24 but only midnight (00h) */
    output[aci_IR_HOUR_BYTE] =
      ((output[aci_IR_HOUR_BYTE] & ~aci_IR_HOUR_MASK) |
       (unsigned char)((aci_IR_HOUR_00H << aci_IR_HOUR_BIT) & aci_IR_HOUR_MASK));
  }
}

void ACI_encodeIrFan(unsigned char output[], int value)
{
  /* only valid value of fan from RF [0; 4[ */
  if ((value >= 0) && (value < aci_IR_FAN_NB))
  {
    /* Write Fan */
    output[aci_IR_FAN_BYTE] =
      ((output[aci_IR_FAN_BYTE] & ~aci_IR_FAN_MASK) |
       (unsigned char)((aci_tab_ir_fan_rf[value] << aci_IR_FAN_BIT) & aci_IR_FAN_MASK));
  }
  else
  {
    /* Write Fan Auto when not valid */
    output[aci_IR_FAN_BYTE] =
      ((output[aci_IR_FAN_BYTE] & ~aci_IR_FAN_MASK) |
       (unsigned char)((aci_IR_FAN_AUTO << aci_IR_FAN_BIT) & aci_IR_FAN_MASK));
  }
}

int ACI_encodeIrMode(unsigned char output[], int value)
{
  /* only valid value of fan from RF [0; 4[ */
  if ((value >= 0) && (value < aci_IR_MODE_NB))
  {
    /* Write Mode */
    output[aci_IR_MODE_BYTE] =
      ((output[aci_IR_MODE_BYTE] & ~aci_IR_MODE_MASK) |
       (unsigned char)((value << aci_IR_MODE_BIT) & aci_IR_MODE_MASK));
  }
  else
  {
    /* Write Mode Auto when not valid */
    output[aci_IR_MODE_BYTE] =
      ((output[aci_IR_MODE_BYTE] & ~aci_IR_MODE_MASK) |
       (unsigned char)((aci_IR_MODE_AUTO << aci_IR_MODE_BIT) & aci_IR_MODE_MASK));
  }

  /* return 0 to use temperature offset and 1 to use temperature */
  return (^value);
}

void ACI_encodeIrMax(unsigned char output[], int value)
{
  if (value)
  {
    /* Max ON */
    output[aci_IR_MAX_BYTE] =
      ((output[aci_IR_MAX_BYTE] & ~aci_IR_MAX_MASK) |
       (unsigned char)((aci_IR_MAX_ON << aci_IR_MAX_BIT) & aci_IR_MAX_MASK));
  }
  else
  {
    /* Max OFF */
    output[aci_IR_MAX_BYTE] =
      ((output[aci_IR_MAX_BYTE] & ~aci_IR_MAX_MASK) |
       (unsigned char)((aci_IR_MAX_OFF << aci_IR_MAX_BIT) & aci_IR_MAX_MASK));
  }
}

void ACI_encodeIrTemperatureAuto(unsigned char output[], float value)
{
  int value_computed = 0;

  /* when negative value */
  if (value < 0)
  {
    /* add signed value */
    value_computed = aci_IR_TEMPAUTO_SIGNED_MASK;
    /* keep only numerical value without signed */
    value *= -1;
  }
  else
  {
    /* nothing to do: positive value */
  }

  /* limit numerical value maximum */
  if (value > aci_IR_TEMPAUTO_NUM_MAX)
  {
    /* cannot exceed maximum */
    value = aci_IR_TEMPAUTO_NUM_MAX;
  }
  else
  {
    /* nothing to do: numerical value so start from 0 until max */
  }

  /* set numerical value */
  value_computed |= (int)value;

  /* set value in output buffer */
  output[aci_IR_TEMPAUTO_BYTE] =
    ((output[aci_IR_TEMPAUTO_BYTE] & ~aci_IR_TEMPAUTO_MASK) |
     (unsigned char)((value_computed << aci_IR_TEMPAUTO_BIT) & aci_IR_TEMPAUTO_MASK));
}

void ACI_encodeIrTemperature(unsigned char output[], float value)
{
  int value_computed = 0;

  if (!value)
  {
    /* temperature not used = 0 */
    value_computed = 0;
  }
  else
  {
    /* temperature is used */
    if (value > aci_IR_TEMPERATURE_MAX)
    {
      value_computed = aci_IR_TEMPERATURE_MAX - aci_IR_TEMPERATURE_OFFSET;
    }
    else if (value < aci_IR_TEMPERATURE_MIN)
    {
      value_computed = aci_IR_TEMPERATURE_MIN - aci_IR_TEMPERATURE_OFFSET;
    }
    else
    {
      /* nothing to do: value in bounds */
    }
  }

  /* set value in output buffer */
  output[aci_IR_TEMPERATURE_BYTE] =
    ((output[aci_IR_TEMPERATURE_BYTE] & ~aci_IR_TEMPERATURE_MASK) |
     (unsigned char)((value_computed << aci_IR_TEMPERATURE_BIT) & aci_IR_TEMPERATURE_MASK));
}

void ACI_encodeIrParity(unsigned char output[])
{

}
