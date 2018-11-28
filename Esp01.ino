#include <ESP8266WiFi.h>
 
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define HTTP_PORT 22222
#define GPIO_2 2

WiFiServer server(HTTP_PORT);

void setup() { 
  pinMode(GPIO_2, OUTPUT);
  digitalWrite(GPIO_2, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
    delay(500);


  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  client.flush();

  bool enabled;
  if (req.indexOf("/gpio/off") != -1)
  {
    enabled = false;
  } 
  else if (req.indexOf("/gpio/on") != -1) 
  {
    enabled = true;
  }
  else
  {
    client.stop();
    return;
  }
 
  digitalWrite(GPIO_2, enabled);

  client.flush();

 String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nEnabled: ";
  s += (enabled) ? "on" : "off";
  s += "</html>\n";
 
  client.print(s);
  
  delay(1); 
}
