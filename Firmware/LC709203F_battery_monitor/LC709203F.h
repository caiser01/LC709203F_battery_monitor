/**************************************************************************
 *                                                                         *
 * LC709203F* Driver for Arduino                                           *
 *                                                                         *
 * Christopher A. Isert                                                    *
 *                                                                         *
 ***************************************************************************/

#include <Arduino.h>
#include <Wire.h>

#ifndef _LC709203F_h
#define _LC709203F_h

#define LC709203F_ADDRESS                 0x0B

// Command codes
#define LC709203F_BEFORE_RSOC             0x04
#define LC709203F_THERMISTOR_B            0x06
#define LC709203F_INITIAL_RSOC            0x07
#define LC709203F_CELL_TEMPERATURE        0x08
#define LC709203F_CELL_VOLTAGE            0x09
#define LC709203F_CURRENT_DIRECTION       0x0A
#define LC709203F_APA                     0x0B
#define LC709203F_APT                     0x0C
#define LC709203F_RSOC                    0x0D
#define LC709203F_ITE                     0x0F
#define LC709203F_IC_VERSION              0x11
#define LC709203F_CHANGE_OF_PARAM         0x12
#define LC709203F_ALARM_LOW_RSOC          0x13
#define LC709203F_ALARM_LOW_CELL_VOLTAGE  0x14
#define LC709203F_IC_POWER_MODE           0x15
#define LC709203F_STATUS_BIT              0x16
#define LC709203F_NUMBER_OF_PARAM         0x1A

// Data word used to initialize RSOC
#define INIT_RSOC                         0xAA55

// Current direction modes
#define CURRENT_DIRECTION_AUTO            0x0000
#define CURRENT_DIRECTION_CHARGE          0x0001
#define CURRENT_DIRECTION_DISCHARGE       0xFFFF

// Power modes
#define OPERATIONAL                       0x0001
#define SLEEP                             0x0002

// Status bit
#define I2C_MODE                          0x0000
#define THERMISTOR_MODE                   0x0001

// Cell temperature offset
#define TEMPERATURE_OFFSET                2732

#define LC709203F_RESPONSE_LENGTH         3       // 2 bytes of data + 1 byte of CRC

// Needed for CRC calculation
#define LC709203F_READ_ADDR               0x17
#define LC709203F_WRITE_ADDR              0x16
#define LC709203F_READ_LENGTH             6
#define LC709203F_WRITE_LENGTH            5

class LC709203F {

  public:
    struct I2C_Comms {
      int registerData = 0;
      bool commStatus = true;
    };
    void initRSOCsampledvoltage();
    I2C_Comms getThermistorB();
    I2C_Comms setThermistorB(int thermistor_const);
    void initRSOC();
    I2C_Comms getCellTemperature();
    I2C_Comms setCellTemperature(int cellTemp);
    I2C_Comms getCellVoltage();
    I2C_Comms getCurrentDirection();
    I2C_Comms setCurrentDirection(int currDir);
    I2C_Comms getAPA();
    I2C_Comms setAPA(int APA);
    I2C_Comms getAPT();
    I2C_Comms setAPT(int APT);
    I2C_Comms getRSOC();
    I2C_Comms getITE();
    I2C_Comms getICVersion();
    I2C_Comms getBatteryProfile();
    I2C_Comms setBatteryProfile(int battProf);
    I2C_Comms getAlarmLowRSOC();
    I2C_Comms setAlarmLowRSOC(int lowRSOCthresh);
    I2C_Comms getAlarmLowCellVoltage();
    I2C_Comms setAlarmLowCellVoltage(int lowCellVoltThresh);
    I2C_Comms getICPowerMode();
    I2C_Comms setICPowerMode(int powerMode);
    I2C_Comms getStatusBit();
    I2C_Comms setStatusBit(int statusBit);
    I2C_Comms getNumberofParam();

  private:
    I2C_Comms simpleWrite(unsigned char commandCode, int dataToWrite);
    I2C_Comms simpleRead(unsigned char commandCode);
    void writeRegister(unsigned char commandCode, I2C_Comms commData);
    void readRegister(unsigned char commandCode, I2C_Comms &commData);
    unsigned char calcCRC8(const unsigned char *vptr, int len);
};

#endif
