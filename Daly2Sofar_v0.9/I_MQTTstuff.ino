
void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        WiFiStatus = true;
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        WiFiStatus = false;
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  MQTTStatus = true;
  xTimerStart(mqttPublishXTimer, 0);
  //mqttPublisherTimer->Start();
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  xTimerStop(mqttPublishXTimer, 0);
  MQTTStatus = false;
  //mqttPublisherTimer->Stop();
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}


void sendMQTTData() {
  // Update all parameters and send to MQTT.
  String state = "{";
  
  if (!( state == "{")) { state += ","; }
  state += "\"soc\":"+String(percentage);

  if (!( state == "{")) { state += ","; }
  state += "\"voltage\":"+String(volts);

  if (!( state == "{")) { state += ","; }
  state += "\"current\":"+String(amps);

  if (!( state == "{")) { state += ","; }
  state += "\"power\":"+String(watts);

  if (!( state == "{")) { state += ","; }
  state += "\"temp\":"+String(temp);

  if (!( state == "{")) { state += ","; }
  state += "\"lowestcell\":"+String(minCellVoltage);
  
  if (!( state == "{")) { state += ","; }
  state += "\"highestcell\":"+String(maxCellVoltage);

  if (!( state == "{")) { state += ","; }
  state += "\"cellimbalance\":"+String(cellImbalance);  // Cell imbalance is sent in Millivolts
      
  state = state+"}";
  
  //Prefixt the mqtt topic name with deviceName.
  String topic (deviceName);
  topic += "/state";

  uint16_t packetIdPub2 = mqttClient.publish(const_cast<char*>(topic.c_str()), 2, true, const_cast<char*>(state.c_str()));
  Serial.println("Publishing at QoS 2");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(state);

  xTimerStart(mqttPublishXTimer, 0);
}
