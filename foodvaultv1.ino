#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager

const int buttonPin = 12; // the number of the pushbutton pin
int buttonState;            // the current reading from the input pin
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;  

void setup() {
   pinMode(buttonPin, INPUT);
    Serial.begin(115200);

    // Initialize Wi-Fi manager
    WiFiManager wifiManager;
    // Uncomment to reset saved settings
    //wifiManager.resetSettings();

    // Set AP timeout to 120 seconds. If no client connects within this time, ESP will try to connect to previously saved WiFi credentials
    wifiManager.setTimeout(120);

    if (!wifiManager.autoConnect("NodeMCU AP")) {
        Serial.println("Failed to connect and hit timeout");
        // Reset and try again, or maybe put it to deep sleep
        ESP.reset();
        delay(1000);
    }

    Serial.println("Connected to Wi-Fi");
}

void loop() {
    // Your code here for what happens after connection
      int reading = digitalRead(buttonPin);
  // read the state of the pushbutton value:
// If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
         WiFiClientSecure httpsClient; //Declare object of class WiFiClient
          httpsClient.setInsecure(); // the ESP8266 does not have the certificate to verify the server's identity

          HTTPClient http;
        if(http.begin(httpsClient, "https://eecs373foodvault.vercel.app/api/temp")){
          int httpCode = http.DELETE();
            Serial.printf("[HTTPS] DELETE... code: %d\n", httpCode);
        }
      }
    }
  // check if the pushbutton is pressed. If it is, the buttonState is LOW:
  
}
lastButtonState = reading;


}
