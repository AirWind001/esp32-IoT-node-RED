#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi credentials
const char* ssid = "TP-Link_GGWP";
const char* password = "Dodecahedron!";

// LED pins (active low)
const int redPin = D1;
const int greenPin = D2;
const int bluePin = D3;

// ESP8266 web server
ESP8266WebServer server(80);

// Function prototypes
void setupWiFi();
void setColor(int redValue, int greenValue, int blueValue);
void handlePattern1();
void handleRainbowWave();
void handleStaticColor();
void pattern1();
void rainbowWave();
void staticColor(int red, int green, int blue);

void setup() {
  Serial.begin(115200);

  // Configure LED pins as outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Set LEDs off initially
  setColor(1, 1, 1);

  // Connect to Wi-Fi
  setupWiFi();

  // Define server routes
  server.on("/pattern1", handlePattern1);
  server.on("/rainbowWave", handleRainbowWave);
  server.on("/staticColor", handleStaticColor);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void setupWiFi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setColor(int redValue, int greenValue, int blueValue) {
  digitalWrite(redPin, redValue);
  digitalWrite(greenPin, greenValue);
  digitalWrite(bluePin, blueValue);
}

void handlePattern1() {
  pattern1();
  server.send(200, "text/plain", "Pattern 1 executed");
}

void handleRainbowWave() {
  rainbowWave();
  server.send(200, "text/plain", "Rainbow wave executed");
}

void handleStaticColor() {
  String red = server.arg("red");
  String green = server.arg("green");
  String blue = server.arg("blue");

  staticColor(red.toInt(), green.toInt(), blue.toInt());
  server.send(200, "text/plain", "Static color set");
}

void pattern1() {
  Serial.println("Pattern 1: Red -> Green -> Blue");
  setColor(0, 1, 1); // Red
  delay(500);
  setColor(1, 0, 1); // Green
  delay(500);
  setColor(1, 1, 0); // Blue
  delay(500);
}

void rainbowWave() {
  Serial.println("Rainbow wave pattern");
  for (int i = 0; i < 256; i++) {
    int red = sin(i * 0.024) * 127 + 128;
    int green = sin(i * 0.024 + 2.094) * 127 + 128;
    int blue = sin(i * 0.024 + 4.188) * 127 + 128;
    setColor(!red, !green, !blue); // Convert to active low
    delay(30);
  }
}

void staticColor(int red, int green, int blue) {
  Serial.println("Static color");
  setColor(!red, !green, !blue); // Convert to active low
}

