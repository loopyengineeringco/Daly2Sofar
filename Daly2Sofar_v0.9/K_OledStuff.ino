void oledMessage(String line1, String line2, String line3, String line4, String line5) {//bool WiFiStatus, bool MQTTStatus) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0,0);
  if (line1 != "NULL") {
    display.println(line1);
    oledLine1 = line1;
  } else {
    display.println(oledLine1);
  }
  
  display.setCursor(0,12);
  if (line2 != "NULL") {
    display.println(line2);
    oledLine2 = line2;
  } else {
    display.println(oledLine2);
  }
  
  display.setCursor(0,24);
  if (line3 != "NULL") {
    display.println(line3);
    oledLine3 = line3;
  } else {
    display.println(oledLine3);
  }
  
  display.setCursor(0,36);
  if (line4 != "NULL") {
    display.println(line4);
    oledLine4 = line4;
  } else {
    display.println(oledLine4);
  }

  display.setCursor(0,48);
  if (line4 != "NULL") {
    display.println(line5);
    oledLine4 = line5;
  } else {
    display.println(oledLine5);
  }

  if (WiFiStatus) {
    display.drawBitmap(96, 0, wifiOn, 32, 14, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else {
    display.drawBitmap(96, 0, wifiOff, 32, 14, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }

  if (MQTTStatus){
    display.drawBitmap(96, 16, mqttOn, 32, 14, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else {
    display.drawBitmap(96, 16, mqttOff, 32, 14, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }

  display.display();
}

