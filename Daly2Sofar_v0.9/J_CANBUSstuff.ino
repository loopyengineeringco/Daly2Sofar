
void sendCanTXFrame(byte dataArray[],int dataLength) {
  tx_frame.FIR.B.FF = CAN_frame_std;
  uint16_t combinedId = 0;                  // Storage var
  uint16_t x1 = (uint16_t)dataArray[0]<<8;  // MSB
  uint16_t x0 = (uint16_t)dataArray[1];     // LSB
  combinedId = x1 | x0;
  tx_frame.MsgID = combinedId; // concatenated address from the 1st 2 values in the data array.
  tx_frame.FIR.B.DLC = dataLength;
  tx_frame.data.u8[0] = dataArray[2];
  tx_frame.data.u8[1] = dataArray[3];
  tx_frame.data.u8[2] = dataArray[4];
  tx_frame.data.u8[3] = dataArray[5];
  tx_frame.data.u8[4] = dataArray[6];
  tx_frame.data.u8[5] = dataArray[7];
  tx_frame.data.u8[6] = dataArray[8];
  tx_frame.data.u8[7] = dataArray[9];
  ESP32Can.CANWriteFrame(&tx_frame);
  CANOnline = true;
  delay(5); // just to make sure it gets sent in peace. TRY INCREASING THIS
}


void checkForRX() {
  CAN_frame_t rx_frame;
  if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
    if (rx_frame.FIR.B.FF == CAN_frame_std) {
      printf("New standard frame");
    } else {
      printf("New extended frame");
    }

    if (rx_frame.FIR.B.RTR == CAN_RTR) {
      printf(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    } else {
      printf(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
      for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
        printf("0x%02X ", rx_frame.data.u8[i]);
      }
      printf("\n");
    }
  }  
}
