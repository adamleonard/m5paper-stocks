#include <HTTPClient.h>
#include <M5EPD.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#include "credentials.h"

M5EPD_Canvas canvas(&M5.EPD);

const int screenWidth = 540;

//WiFiClientSecure wifiClient;
//HttpClient httpClient = HttpClient(wifiClient, API_HOST, API_PORT);

void setup() {
  Serial.println("Preparing display");
  M5.begin(); 
  M5.EPD.SetRotation(90);
  M5.RTC.begin();
  canvas.createCanvas(screenWidth, 960);

  const int margin = 20;
  int yOffset = margin;
    
  Serial.println("Joining WiFi network");
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Waiting to join WiFi network..");
  }
  
  Serial.print("Sending stocks API request: ");
  Serial.println(API_HOST);

  HTTPClient http;
  http.begin(API_HOST);
  
  int responseStatusCode = http.GET();
  String response = http.getString();
  if (responseStatusCode >= 400) {
    Serial.print("Got bad response: ");
    Serial.println(responseStatusCode);
    Serial.println(response);
    return;
  }
  Serial.println(responseStatusCode);
  Serial.println(response);

  Serial.println("Parsing JSON");
  JSONVar responseObject = JSON.parse(response);
  if (JSON.typeof(responseObject) == "undefined") {
    Serial.println("Failed to parse API response");
    return;
  }

  JSONVar stocks = responseObject["stocks"];
  Serial.print("Displaying ");
  Serial.print(stocks.length());
  Serial.println(" stocks");

  M5.EPD.Clear(true);
  
  canvas.setTextSize(6);
  canvas.drawString("Stocks", margin, yOffset);
  yOffset += 60;
  canvas.drawLine(0, yOffset, screenWidth, yOffset, 1, 255);
  yOffset += 20;
  
  for (int i = 0; i < stocks.length(); ++i) {
    JSONVar stock = stocks[i];
    
    canvas.setTextSize(4);
    canvas.drawString(stock["symbol"], margin, yOffset);
    
    char priceString[16];
    dtostrf(stock["price"], 8, 2, priceString);
    int priceWidth = canvas.textWidth(priceString);
    canvas.drawString(priceString, screenWidth - margin - priceWidth, yOffset);
    yOffset += 40;
    
    canvas.setTextSize(2);
    canvas.drawString(stock["company_name"], margin, yOffset);

    double percentChangeNumber = stock["percent_change"];
    char percentChangeString[16];
    dtostrf(percentChangeNumber, 4, 2, percentChangeString);
    if (percentChangeNumber > 0) {
      memmove(percentChangeString + 1, percentChangeString, strlen(percentChangeString) + 1);
      percentChangeString[0] = '+';
    }
    strcat(percentChangeString, "%");
    int percentChangeWidth = canvas.textWidth(percentChangeString);
    canvas.drawString(percentChangeString, screenWidth - margin - percentChangeWidth, yOffset);
    yOffset += 20;
    
    canvas.drawLine(0, yOffset, screenWidth, yOffset, 1, 255);
    yOffset += 20;
  }

  canvas.setTextSize(2);
  canvas.drawString(responseObject["last_updated"], margin, yOffset);

  canvas.pushCanvas(0, 0, UPDATE_MODE_GL16);
  WiFi.disconnect(true);

  // Give time for the display to settle
  delay(1000);
  
  M5.shutdown(900);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
