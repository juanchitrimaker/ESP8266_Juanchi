#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define XIVELY_BROKER             "broker.xively.com"
#define XIVELY_BROKER_PORT        443

// Enter your WiFi credentials 
#define WLAN_SSID                 "Wideo"
#define WLAN_PASS                 "churnparamkt<5%"

// Enter your Xively credentials and topic
#define XIVELY_DEVICE_ID          "445fe324-e39a-4939-813b-dd44fda8fb70" 
#define XIVELY_DEVICE_CREDENTIALS "8ZG8Soes0E5zcvbUtwKWNH2E3n+rUMNEqajoiwPVo54=445fe324-e39a-4939-813b-dd44fda8fb70"
#define LED_TOPIC                 "xi/blue/v1/730425e5-2503-4dc1-a78c-b3d170e356b7/d/445fe324-e39a-4939-813b-dd44fda8fb70/potenciometro"

// Only change this if you have a newer certificate fingerprint
const char* fingerprint = "60:1A:9C:B6:DF:9C:AF:F5:00:0C:6B:E4:43:0A:22:FE:45:F8:48:23";

// Setup the MQTT secure client class by passing in the WiFi client and MQTT server and login details.
WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client, XIVELY_BROKER, XIVELY_BROKER_PORT, XIVELY_DEVICE_ID, XIVELY_DEVICE_CREDENTIALS);

// Setup Channel
Adafruit_MQTT_Subscribe led = Adafruit_MQTT_Subscribe(&mqtt, LED_TOPIC);

void MQTT_connect();
void verifySecure();
void blink();

void setup() {
  Serial.begin(9600);

  // Finds the onboard LED and sets it to off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.println(F("Xively MQTTS (SSL/TLS) Example"));

  // Connect to WiFi access point
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  delay(1000);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // verify SSL certificate origin
  verifySecure();
  blink(1);
  mqtt.subscribe(&led);
}

void loop() {
  // Ensure the connection to the MQTT server is alive
  MQTT_connect();

  // Subscribe to channel and apply changes. 
  // Messages in the channel should be 0 or 1
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &led) {
      Serial.print(F("led: "));
      Serial.println((char *)led.lastread);
    }

    if (strcmp((char *)led.lastread, "1") == 0) {
        digitalWrite(LED_BUILTIN, LOW); 
        Serial.println("swithing on");
    }
    if (strcmp((char *)led.lastread, "0") == 0) {
      digitalWrite(LED_BUILTIN, HIGH); 
      Serial.println("switching off");
    }
  }
}


void verifySecure() {

  const char* host = XIVELY_BROKER;

  Serial.print("Connecting to ");
  Serial.println(host);

  if (! client.connect(host, XIVELY_BROKER_PORT)) {
    Serial.println("Connection failed. Halting execution.");
    while(1);
  }

  // Shut down connection if host identity cannot be trusted.
  if (client.verify(fingerprint, host)) {
    Serial.println("Connection secure.");
  } else {
    Serial.println("Connection insecure! Halting execution.");
    while(1);
  }
}

void blink(int times) {
  int count = 0;
  while (count < times) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    count++;
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
}
