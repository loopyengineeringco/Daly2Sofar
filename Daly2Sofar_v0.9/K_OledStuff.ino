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

  if (BMSOnline) {
    display.drawBitmap(99, 0, dalyOn, 29, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else {
    display.drawBitmap(99, 0, dalyOff, 32, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }

  if (CANOnline) {
    display.drawBitmap(99, 14, canOn, 29, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else {
    display.drawBitmap(99, 14, canOff, 32, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }

  if (WiFiStatus) {
    display.drawBitmap(99, 28, wifiOn, 29, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else {
    display.drawBitmap(99, 28, wifiOff, 32, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }

  if (MQTTStatus){
    display.drawBitmap(99, 42, mqttOn, 29, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  } else {
    display.drawBitmap(99, 42, mqttOff, 29, 13, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  }

  display.display();
}
