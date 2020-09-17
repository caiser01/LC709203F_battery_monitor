#include <Wire.h>
#include "LC709203F.h"

// commResult.registerData contains raw data read from or
// written to the chip
// For a read, commResult.commStatus indicates whether 
// a valid CRC was received or not
// For a write commResult.commStatus indicates whether
// the input data was within range of valid data for
// the given register
LC709203F::I2C_Comms commResult;

// An object for communicating with the LC709203F IC
LC709203F batteryMonitor;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // LC709203F initialization
  
  // APA value must be determined from the data given in
  // Figure 12, Table 7, and Table 8 in the datasheet.
  // The value below is for a typical 2000mAh LiPo battery
  batteryMonitor.setAPA(0x2D);

  // Battery profile depends on battery capacity. See Table 8.
  batteryMonitor.setBatteryProfile(0);

  // Temperature is set by I2C
  batteryMonitor.setStatusBit(I2C_MODE);

  // Set initial temperature to 20 degrees C
  batteryMonitor.setCellTemperature(TEMPERATURE_OFFSET + 200);
}

void loop() {

  // Read and display some basic stats about the battery
  
  commResult = batteryMonitor.getCellTemperature();
  // Temperature reading is in 0.1 degree Celsius increments
  // Divide by 10 to get whole degrees
  float battTemp = (commResult.registerData - TEMPERATURE_OFFSET) / 10.0;

  commResult = batteryMonitor.getCellVoltage();
  int battVolt = commResult.registerData;

  commResult = batteryMonitor.getRSOC();
  int RSOC = commResult.registerData;
  
  Serial.println("Battery stats");
  
  Serial.print("Cell Temperature (degC): ");
  Serial.println(battTemp);
  
  Serial.print("Cell Voltage (mV): ");
  Serial.println(battVolt);
  
  Serial.print("State of charge (%): ");
  Serial.println(RSOC);
  
  Serial.println();
  
  delay(1000);
}
