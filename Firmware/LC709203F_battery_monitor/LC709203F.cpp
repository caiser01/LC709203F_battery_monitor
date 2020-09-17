/**************************************************************************
 *                                                                         *
 * LC709203F* Driver for Arduino                                           *
 *                                                                         *
 * Christopher A. Isert                                                    *
 *                                                                         *
 ***************************************************************************/

#include "LC709203F.h"

void LC709203F::initRSOCsampledvoltage(){
  simpleWrite(LC709203F_BEFORE_RSOC, INIT_RSOC);
}

LC709203F::I2C_Comms LC709203F::getThermistorB(){
  return simpleRead(LC709203F_THERMISTOR_B);
}

LC709203F::I2C_Comms LC709203F::setThermistorB(int thermistor_const){
  return simpleWrite(LC709203F_THERMISTOR_B, thermistor_const);
}

void LC709203F::initRSOC(){
  simpleWrite(LC709203F_INITIAL_RSOC, INIT_RSOC);
}

LC709203F::I2C_Comms LC709203F::getCellTemperature(){
  return simpleRead(LC709203F_CELL_TEMPERATURE);
}

LC709203F::I2C_Comms LC709203F::setCellTemperature(int cellTemp){
  if ((cellTemp >= 0x09E4) && (cellTemp <= 0x0D04)){
    return simpleWrite(LC709203F_CELL_TEMPERATURE, cellTemp);
  }
  else {
    LC709203F::I2C_Comms commWrite;
    commWrite.registerData = cellTemp;
    commWrite.commStatus = false;
    return commWrite;
  }
}

LC709203F::I2C_Comms LC709203F::getCellVoltage(){
  return simpleRead(LC709203F_CELL_VOLTAGE);
}

LC709203F::I2C_Comms LC709203F::getCurrentDirection(){
  return simpleRead(LC709203F_CURRENT_DIRECTION);
}

LC709203F::I2C_Comms LC709203F::setCurrentDirection(int currDir){
  return simpleWrite(LC709203F_CURRENT_DIRECTION, currDir);
}

LC709203F::I2C_Comms LC709203F::getAPA(){
  return simpleRead(LC709203F_APA);
}

LC709203F::I2C_Comms LC709203F::setAPA(int APA){
  if ((APA >= 0) && (APA <= 0x00FF)){
    return simpleWrite(LC709203F_APA, APA);
  }
  else {
    LC709203F::I2C_Comms commWrite;
    commWrite.registerData = APA;
    commWrite.commStatus = false;
    return commWrite;
  }  
}

LC709203F::I2C_Comms LC709203F::getAPT(){
  return simpleRead(LC709203F_APT);
}

LC709203F::I2C_Comms LC709203F::setAPT(int APT){
  return simpleWrite(LC709203F_APT, APT);
}

LC709203F::I2C_Comms LC709203F::getRSOC(){
  return simpleRead(LC709203F_RSOC);
}

LC709203F::I2C_Comms LC709203F::getITE(){
  return simpleRead(LC709203F_ITE);
}

LC709203F::I2C_Comms LC709203F::getICVersion(){
  return simpleRead(LC709203F_IC_VERSION);
}

LC709203F::I2C_Comms LC709203F::getBatteryProfile(){
  return simpleRead(LC709203F_CHANGE_OF_PARAM);
}

LC709203F::I2C_Comms LC709203F::setBatteryProfile(int battProf){
  return simpleWrite(LC709203F_CHANGE_OF_PARAM, battProf);
}

LC709203F::I2C_Comms LC709203F::getAlarmLowRSOC(){
  return simpleRead(LC709203F_ALARM_LOW_RSOC);
}

LC709203F::I2C_Comms LC709203F::setAlarmLowRSOC(int lowRSOCthresh){
  if ((lowRSOCthresh >= 0) && (lowRSOCthresh <= 0x0064)){
    return simpleWrite(LC709203F_ALARM_LOW_RSOC, lowRSOCthresh);
  }
  else {
    LC709203F::I2C_Comms commWrite;
    commWrite.registerData = lowRSOCthresh;
    commWrite.commStatus = false;
    return commWrite;
  }  
}

LC709203F::I2C_Comms LC709203F::getAlarmLowCellVoltage(){
  return simpleRead(LC709203F_ALARM_LOW_CELL_VOLTAGE);
}

LC709203F::I2C_Comms LC709203F::setAlarmLowCellVoltage(int lowCellVoltThresh){
  return simpleWrite(LC709203F_ALARM_LOW_CELL_VOLTAGE, lowCellVoltThresh);
}

LC709203F::I2C_Comms LC709203F::getICPowerMode(){
  return simpleRead(LC709203F_IC_POWER_MODE);
}

LC709203F::I2C_Comms LC709203F::setICPowerMode(int powerMode){
  return simpleWrite(LC709203F_IC_POWER_MODE, powerMode);
}

LC709203F::I2C_Comms LC709203F::getStatusBit(){
  return simpleRead(LC709203F_STATUS_BIT);
}

LC709203F::I2C_Comms LC709203F::setStatusBit(int statusBit){
  return simpleWrite(LC709203F_STATUS_BIT, statusBit);
}

LC709203F::I2C_Comms LC709203F::getNumberofParam(){
  return simpleRead(LC709203F_NUMBER_OF_PARAM);
}

LC709203F::I2C_Comms LC709203F::simpleWrite(unsigned char commandCode, int dataToWrite){
  LC709203F::I2C_Comms commWrite;
  commWrite.registerData = dataToWrite;
  writeRegister(commandCode, commWrite);
  commWrite.commStatus = true;
  return commWrite;
}

LC709203F::I2C_Comms LC709203F::simpleRead(unsigned char commandCode){
  LC709203F::I2C_Comms commRead;  
  readRegister(commandCode, commRead);  
  return commRead;
}

void LC709203F::writeRegister(unsigned char commandCode, LC709203F::I2C_Comms commWrite){
  unsigned char dataByteLow = (unsigned char)(0x00FF & commWrite.registerData);
  unsigned char dataByteHigh = (unsigned char)((0xFF00 & commWrite.registerData) >> 8);

  // Calculate CRC, chip will not accept data without a valid CRC
  unsigned char message[LC709203F_WRITE_LENGTH] = {LC709203F_WRITE_ADDR, commandCode, dataByteLow, dataByteHigh};
  unsigned char crc = calcCRC8(message, LC709203F_WRITE_LENGTH - 1);

  // Write register data and CRC
  Wire.beginTransmission(LC709203F_ADDRESS);
  Wire.write(commandCode);
  //Wire.write(commWrite.registerData);
  Wire.write(dataByteLow);
  Wire.write(dataByteHigh);
  Wire.write(crc);
  Wire.endTransmission();

  commWrite.commStatus = 0;
}

void LC709203F::readRegister(unsigned char commandCode, LC709203F::I2C_Comms &commRead){
  // Send the command code
  Wire.beginTransmission(LC709203F_ADDRESS);
  Wire.write(commandCode);
  Wire.endTransmission(false); // false because the chip requires a repeated start

  // Receive the register data and CRC
  Wire.requestFrom(LC709203F_ADDRESS, LC709203F_RESPONSE_LENGTH);
  
  unsigned char dataByteLow = Wire.read();
  unsigned char dataByteHigh = Wire.read();

  // Combine upper and lower data bytes
  commRead.registerData = 0xFF00 & (dataByteHigh << 8);
  commRead.registerData |= 0x00FF & dataByteLow;
  
  unsigned char crc = Wire.read();
  
  unsigned char message[LC709203F_READ_LENGTH] = {LC709203F_WRITE_ADDR, commandCode, LC709203F_READ_ADDR, dataByteLow, dataByteHigh, crc};
  crc = calcCRC8(message, LC709203F_READ_LENGTH);

  // True only if CRC is good
  commRead.commStatus = (crc == 0);
}

// https://chromium.googlesource.com/chromiumos/platform/vboot_reference/+/664096bd1a7247b751bf4e564738d53a1e2795c2/firmware/lib/crc8.c

/**
 * Return CRC-8 of the data, using x^8 + x^2 + x + 1 polynomial.  A table-based
 * algorithm would be faster, but for only a few bytes it isn't worth the code
 * size. */
unsigned char LC709203F::calcCRC8(const unsigned char *vptr, int len)
{
  const unsigned char *data = vptr;
  unsigned int crc = 0;
  int i, j;
  for (j = len; j; j--, data++) {
    crc ^= (*data << 8);
    for(i = 8; i; i--) {
      if (crc & 0x8000)
        crc ^= (0x1070 << 3);
      crc <<= 1;
    }
  }
  return (unsigned char)(crc >> 8);
}
