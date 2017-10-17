/* InfraRed Protocol: Implement AC protocol */

#ifndef ARDLANTIC_IRP_H
#define ARDLANTIC_IRP_H

#include <Arduino.h>

/* number of bytes for data part */
#define IRP_FRAME_DATA_BYTE_NB (13)
/* number of command for whole frame */
/* 2 command for header + 1 pause + 13 bytes of data (13 * 8 bits * 2 to have data + pause for each bit) */
/* +-----------++-------+-------+-------+--------+-------+--------+-------+-----+--------+-------+ */
/* | infra red || ON    | OFF   | ON    | OFF    | ON    | OFF    | ON    | ... | OFF    | ON    | */
/* | led state ||       |       |       |        |       |        |       |     |        |       | */
/* |-----------++-------+-------+-------+--------+-------+--------+-------+-----+--------+-------| */
/* | command   || START | FIRST | PAUSE | BIT0   | PAUSE | BIT1   | PAUSE | ... | BIT103 | PAUSE | */
/* |-----------++-------+-------+-------+--------+-------+--------+-------+-----+--------+-------| */
/* | example of|| 3750  | 1950  | 400   | 550 or | 400   | 550 or | 400   | ... | 550 or | 400   | */
/* | duration  ||       |       |       | 1450   |       | 1450   |       |     | 1450   |       | */
/* +-----------++-------+-------+-------+--------+-------+--------+-------+-----+--------+-------+ */
#define IRP_FRAME_COMMAND_NB (2 + 1 + (IRP_FRAME_DATA_BYTE_NB * 8 * 2))


typedef enum
{
  IRP_FAN_MODE_AUTO   = 2,
  IRP_FAN_MODE_LOW    = 3,
  IRP_FAN_MODE_MIDDLE = 5,
  IRP_FAN_MODE_HIGH   = 7
} IRP_FanMode_t;

typedef enum
{
  IRP_FLOW_MODE_AUTO = 0,
  IRP_FLOW_MODE_HOT  = 1,
  IRP_FLOW_MODE_COLD = 2,
  IRP_FLOW_MODE_WET  = 3
} IRP_FlowMode_t;

#ifdef __cplusplus
class IRP_Message
{
public:
  /* Constructors */
  IRP_Message();

  void clear(void);

  void send(void);

  void setIon(bool isEnableIon);
  void setStartUp(bool isStartUp);
  void setSwing(bool isEnableSwing);
  void setHour(uint8_t hour);
  void setFanMode(IRP_FanMode_t fanMode);
  bool setFlowMode(IRP_FlowMode_t flowMode);
  void setFanMax(bool isFanMax);
  void setTemperatureRelative(int16_t temperature);
  void setTemperature(uint8_t temperature);

private:
  /* buffer of data in IR frame built byte by byte */
  uint8_t _data[IRP_FRAME_DATA_BYTE_NB];
  /* buffer of frame with duration of each command (including header + data) */
  uint16_t _buffer[IRP_FRAME_COMMAND_NB];

  void setParity(void);
} __attribute__((packed));
#endif /* __cplusplus */

#endif /* ARDLANTIC_IRP_H */
