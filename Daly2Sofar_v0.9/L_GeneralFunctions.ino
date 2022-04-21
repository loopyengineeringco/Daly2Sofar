void getEverythingFromDaly() {  
  // Grab values from BMS
  bms.getPackMeasurements(volts, amps, percentage);
  bms.getPackTemp(temp);
  bms.getMinMaxCellVoltage(minCellVoltage, minCellNumber, maxCellVoltage, maxCellNumber);
  
  if (volts == 0) {
    Serial.println("No connection with BMS. Check TX/RX polarity, and that BMS is awake...");
    oledMessage("No BMS found.", "Check TX/RX", "polarity,", "Make sure", "BMS is awake.");
    //BMSOnline = false;
    //bms.Init();
    BMSOnline = true;
  } else {
    watts = amps*volts;
    cellImbalance = (maxCellVoltage-minCellVoltage)*1000;
    oledMessage("SOC: "+String(percentage)+"%", "Volts: "+String(volts)+"V", "Amps: "+String(amps)+"A", "Min/Max: "+String(minCellVoltage)+"/"+String(maxCellVoltage), "Temp: "+String(temp));
    BMSOnline = true;
    // Print above to Serial
    Serial.printf("V: %4.1f, I: %4.1f, \%:%4.1f\n",volts, amps, percentage);
    
    // Now the same thing, but for temperature
    Serial.printf("Temp: %d\n",temp);
    
    // And again, for min/max cell voltages
    Serial.printf("Max Cell: Cell #%d at %4.3f\n",maxCellNumber,maxCellVoltage);
    Serial.printf("Min Cell: Cell #%d at %4.3f\n",minCellNumber,minCellVoltage);    
  }
}


void updateTXFrames() {
  // SOC. 1:1 scaling, 2 bytes - easy.
  CANData355[2] = (int)percentage;

  // Charge voltage
  uint16_t uint16chargevolts = batteryChargeVoltage*10;
  CANData351[2] = uint16chargevolts;        // Arduino/C is big endian. So needs to shift the opposite way of the SMA protocol.
  CANData351[3] = uint16chargevolts>>8;
  
  // High res SOC: 0.01 scale, 2 bytes. rescale, then bitshift.
  uint16_t uint16soc = percentage*100;
  CANData355[6] = uint16soc;        // Arduino/C is big endian. So needs to shift the opposite way of the SMA protocol.
  CANData355[7] = uint16soc>>8;

  // Voltage. 0.01 scale, over 2 bytes SIGNED. Not easy. First rescale, then bitshift
  int16_t int16volts = volts*100;
  CANData356[2] = int16volts;       // Arduino/C is big endian. So needs to shift the opposite way of the SMA protocol.
  CANData356[3] = int16volts>>8;

  // Current. 0.1 scale, over 2 bytes SIGNED. Not easy. First rescale, then bitshift
  int16_t int16amps = amps*10;
  CANData356[4] = int16amps;        // Arduino/C is big endian. So needs to shift the opposite way of the SMA protocol.
  CANData356[5] = int16amps>>8;

  // Battery temp. 0.1 scale, 2 bytes SIGNED. Not easy. First rescale, then bitshift
  int16_t int16temp = temp*10;
  CANData356[6] = int16temp;        // Arduino/C is big endian. So needs to shift the opposite way of the SMA protocol.
  CANData356[7] = int16temp>>8;
  
  // Battery capacity = fixed value from setup section
  CANData35F[7] = batteryCapacity;    // Arduino/C is big endian. So needs to shift the opposite way of the SMA protocol.
  CANData35F[6] = batteryCapacity>>8; // Currently reporting 51200ah :-D obvs wrong...
}




