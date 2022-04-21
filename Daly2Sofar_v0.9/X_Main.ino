void setup() {
  Serial.begin(115200);
  Serial.println("Daly2Sofar-UARTtoCANBUS v1");
  
  // Set CAN pins and baudrate
  CAN_cfg.speed=CAN_SPEED_500KBPS;
  CAN_cfg.tx_pin_id = GPIO_NUM_5;
  CAN_cfg.rx_pin_id = GPIO_NUM_4;
  
  // Create a queue for CAN receiving */
  CAN_cfg.rx_queue = xQueueCreate(rx_queue_size,sizeof(CAN_frame_t));
  
  // Initialize CAN Module
  ESP32Can.CANInit();
  
  // Initialise the Daly comms
  bms.Init();

  Wire.begin();   // sda= GPIO_21 /scl= GPIO_22
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize OLED with the I2C addr 0x3C (for the 64x48)
  display.clearDisplay();
  display.display();
  oledMessage("Hi", "Ola!", "Hello", "Labas", "Zdravstvuyte");
  delay(500);

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
  mqttPublishXTimer = xTimerCreate("mqttPubTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(sendMQTTData));

  WiFi.onEvent(WiFiEvent);

  mqttClient.setCredentials(mqttUser, mqttPassword);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}


void loop() {
  unsigned long currentMillisUARTCAN = millis();
  if (currentMillisUARTCAN - previousMillisUARTCAN >= intervalUARTCAN) {
    previousMillisUARTCAN = currentMillisUARTCAN;
    
    getEverythingFromDaly();
  
    if (BMSOnline) {
      updateTXFrames();
  
      sendCanTXFrame(CANData351, 8); // 0x0351 - 8 bytes
      sendCanTXFrame(CANData355, 6); // 0x0355 - 6 bytes
      sendCanTXFrame(CANData356, 6); // 0x0356 - 6 bytes
      sendCanTXFrame(CANData35A, 8); // 0x035A - 8 bytes
      sendCanTXFrame(CANData35B, 2); // 0x035B - 2 bytes
      sendCanTXFrame(CANData35E, 8); // 0x035E - 8 bytes
      sendCanTXFrame(CANData35F, 8); // 0x035F - 8 bytes
    }
  }
}
