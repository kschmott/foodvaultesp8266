#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager



void setup() {
    Serial.begin(9600);

    // Initialize Wi-Fi manager
    WiFiManager wifiManager;
    // Uncomment to reset saved settings
    //wifiManager.resetSettings();

    // Set AP timeout to 120 seconds. If no client connects within this time, ESP will try to connect to previously saved WiFi credentials
    wifiManager.setTimeout(120);

    if (!wifiManager.autoConnect("ESP-01")) {
        Serial.println("Failed to connect and hit timeout");
        // Reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);
    }

    Serial.println("Connected to Wi-Fi");
}

void loop() {
  
    if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the command
    Serial.printf("Recieved Command %s\r\n", command);

    if (command == "LIST") {
      fetchList();
    }else if (command.startsWith("DELETE")){
      command.replace("DELETE", "");
      deleteItem(command);
    }else if(command.startsWith("PUT")){
      String box = command.substring(3,4);
      String id = command.substring(4);
      updateBoxStatus(id, box);
    }
   
  }
}
void deleteItem(String id){
  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();
  HTTPClient http;
  http.begin(httpsClient, "https://eecs373foodvault.vercel.app/api/foodOrders?id=" + id); 
  int httpCode = http.DELETE();
   if (httpCode > 0) { // Check for the returning code
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Error on HTTP request");
  }

}
void updateBoxStatus(String id, String box){
  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();
  HTTPClient http;
  http.begin(httpsClient, "https://eecs373foodvault.vercel.app/api/foodOrders?id=" + id + "&box=" + box); 
  int httpCode = http.PUT(nullptr, 0);
   if (httpCode > 0) { // Check for the returning code
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Error on HTTP request");
  }
}

void fetchList(){
  WiFiClientSecure httpsClient;
  httpsClient.setInsecure();
  HTTPClient http;
   http.begin(httpsClient, "https://eecs373foodvault.vercel.app/api/foodOrders?mode=string"); 
   int httpCode = http.GET();
   if (httpCode > 0) { // Check for the returning code
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Error on HTTP request");
  }

}
