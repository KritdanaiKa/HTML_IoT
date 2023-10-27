#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const int Pin = D6;
bool Status = false;

DHT dht14(D4, DHT11);
ESP8266WebServer server(80);

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial. print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {
  String html = "<meta charset=\"UTF-8\">";
  html += "<button class='btn success'><a href=\"/toggle\">LED</a></button>";
  html += "\tStatus : ";
  html += (Status ? "ON" : "OFF");
  html += "<br><table border='1'>";
  html += "<tr><th>Humidity</th>";
  html += "<th>Temperature</th>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>"+ String(dht14.readHumidity()) + " %</td>";
  html += "<td>"+ String(dht14.readTemperature()) + " C</td>";
  html += "</tr>";
  html += "</table>";
  server.send(200, "text/html; charset=UTF-8", html);
}


void handleToggle() {
  Status = !Status;
  digitalWrite(Pin, Status ? HIGH : LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(31250);
  init_wifi("Miss Shadopay","krittscabello");
  pinMode(Pin, OUTPUT);
  dht14.begin();
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  delay(1000);
}
