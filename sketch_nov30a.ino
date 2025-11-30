#include <WiFi.h>

const char* ssid = "jnv";
const char* password = "12345678";

WiFiServer server(80);

int led = 2;  // LED pin

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) delay(1);
  String req = client.readStringUntil('\r');
  client.flush();

  if (req.indexOf("/on") != -1) digitalWrite(led, HIGH);
  if (req.indexOf("/off") != -1) digitalWrite(led, LOW);

  // Webpage with colored buttons
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<html>");
  client.println("<head><title>ESP32 LED Control</title></head>");
  client.println("<body style='text-align:center; font-family: Arial;'>");
  client.println("<h1>ESP32 LED Control</h1>");

  // Green ON button
  client.println("<a href='/on'><button style='padding:35px 70px; font-size:20px; background-color:green; color:white; border:none; border-radius:10px;'>ON</button></a>");
  client.println("<br><br>");

  // Red OFF button
  client.println("<a href='/off'><button style='padding:35px 70px; font-size:20px; background-color:red; color:white; border:none; border-radius:10px;'>OFF</button></a>");

  client.println("</body></html>");
}
