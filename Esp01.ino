#include <ESP8266WiFi.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define HTTP_PORT 8266
#define GPIO 0
#define CLRF "\r\n"
#define COMMA ","
#define TAB "    "
#define W(s) \
  String("\"") + s + "\""
#define HTTP_OK(content_type,connection, ...) \
  String("HTTP/1.1 200 OK" CLRF \
         "Content-Type: " content_type CLRF \
         "Connection: " connection CLRF \
         __VA_ARGS__ CLRF)


WiFiServer server(HTTP_PORT);

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA );
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
    delay(500);


  server.begin();
  pinMode(GPIO, OUTPUT);
  Serial.println("#init completed");
}

bool state;
WiFiClient client;
void loop() {
  client = server.available();
  if (!client) {
    delay(1);
    return;
  }

  while (!client.available()) {
    delay(10);
  }
  String r = client.readStringUntil('\r');
  if (pras(r))
  {
    String boolalpha = (state ? "true" : "false") ;
    String html = HTTP_OK("application/json", "Closed") +
                  "{" + CLRF +
                  TAB + "\"value\": " + boolalpha + CLRF +
                  "}";
    Serial.println("\"value\":" + boolalpha);
    client.print(html);
  }
  else if (prai(r))
  {
    String html = HTTP_OK("application/json", "Closed") +
                  "{" + CLRF +
                  TAB + "\"boot mode\": " + ESP.getBootMode() + COMMA + CLRF +
                  TAB + "\"sdk version\": " + W(ESP.getSdkVersion()) + COMMA + CLRF +
                  TAB + "\"boot version\": " + W(ESP.getBootVersion()) + COMMA + CLRF +
                  TAB + "\"chip id\": " + W(ESP.getChipId()) + COMMA + CLRF +
                  TAB + "\"flash chip size\": " + ESP.getFlashChipSize() + COMMA + CLRF +
                  TAB + "\"flash chip real size\": " + ESP.getFlashChipRealSize() + COMMA + CLRF +
                  TAB + "\"flash chip size by chip id\": " + ESP.getFlashChipSizeByChipId() + COMMA + CLRF +
                  TAB + "\"flash chip id\": " + W(ESP.getFlashChipId()) + COMMA + CLRF +
                  TAB + "\"free heap\": " + ESP.getFreeHeap() + CLRF +
                  "}";
    client.print(html);
  }
  else
    client.flush();
  delay(1);
}

//Process Request About Info
bool prai(String request)
{
  if (match(request, "info"))
  {
    return true;
  }
  return false;
}


//Process Request About State
bool pras(String request)
{
  if (match(request, "toggle"))
  {
    state = !state;
    digitalWrite(GPIO, state);
    goto OK;
  }
  if (match(request, "state"))
  {
    goto OK;
  }
  if (match(request, "on"))
  {
    state = true;
    digitalWrite(GPIO, state);
    goto OK;
  }
  if (match(request, "off"))
  {
    state = false;
    digitalWrite(GPIO, state);
    goto OK;
  }

  return false;

OK:
  Serial.println("Request: " + request);
  return true;
}

bool match(String req, String target) {
  req.toLowerCase();
  return req.indexOf("/" + target) != -1;
}
