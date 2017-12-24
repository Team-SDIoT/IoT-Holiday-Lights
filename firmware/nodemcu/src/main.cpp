/*

Created by: Ayan Pahwa for SDIoT
December, 2017
http://sdiot.in

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h> // For MQTT transactions 
#include <Adafruit_NeoPixel.h> // For controlling neopixel LEDs

#define SERIAL_DEBUG   1 // 1 to enable serial debugging, 0 otherwise
#define NEOPIXEL       1 // 1 if using Neopixel LEDs
#define ANALOG_RGB     0 // 1 if using analog RGB LED strip

#define NEOPIXEL_PIN   D8 // Pin on which Neopicel LED strip is connected
#define NUMPIXELS      85 // Number of Neopixels LEDs in strip

#define RED            D2
#define GREEN          D5 // Pin mapping for Analog RGB LED strip
#define BLUE           D6 


// Variable to hold network parameters
const char* ssid = ".........";
const char* password = "........";
const char* mqtt_server = "***.***.***.***"; //Address or IP
const char* TOPIC = "cafeteria/trees"; // MQTT topic

// Neopixel class instance
#if NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
#endif

// WiFi and PubSub Class Instances
WiFiClient espClient;
PubSubClient client(espClient);

// Declare  functions
void setup_wifi(); // To connect nodemcu board to WiFi Network
void rainbow(uint8_t wait); // Rainbow animation
void colorWipe(uint32_t c, uint8_t wait); // ColorWipe animation
uint32_t Wheel(byte WheelPos); 
void theaterChase(uint32_t c, uint8_t wait); // TheaterChase animation
void callback(char* , byte* , unsigned int); // MQTT callback function

// Program starts here
void setup() {

  #if SERIAL_DEBUG
  Serial.begin(115200); // For Serial debugging
  #endif

  #if ANALOG_RGB
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT); // Declare pins as output for PWM
  pinMode(BLUE, OUTPUT);
  #endif

  setup_wifi(); // Start by connecting to WiFi
  client.setServer(mqtt_server, 1883); // Set MQTT broker IP/Hostname and Port
  client.setCallback(callback);
  strip.begin(); // Initialize Neopixel strip

}

void setup_wifi() {

  delay(5); // Small delay before starting WiFi activity

  #if SERIAL_DEBUG
  Serial.println("Connecting to ");
  Serial.println(ssid);
  #endif

  WiFi.begin(ssid, password); // This will conenct ESP to WiFi with given SSID and PASSWORD

  while (WiFi.status() != WL_CONNECTED) {

    delay(500); // Retry connection after 500 ms

    #if SERIAL_DEBUG
    Serial.print(".");
    #endif

}

  #if SERIAL_DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Prints IP Address on serial console
  #endif

}

// MQTT callback function, executed whenever a payload is received on topic node is subscribed to
void callback(char* topic, byte* payload, unsigned int length) {

  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);


  if(message == "color-wipe") {

    #if SERIAL_DEBUG
    Serial.println("color-wipe");
    #endif

    colorWipe(strip.Color(255, 0, 0), 6);
    colorWipe(strip.Color(0, 0, 0), 6);

  }

  if(message == "theater-chase") {

    #if SERIAL_DEBUG
    Serial.println("theatre-chase");
    #endif

    colorWipe(strip.Color(255, 0, 0), 6);
    colorWipe(strip.Color(0, 0, 0), 6);
    delay(100);

    colorWipe(strip.Color(0, 255, 0), 6);
    colorWipe(strip.Color(0, 0, 0), 6);
    delay(100);

    colorWipe(strip.Color(0, 0, 255), 6);
    colorWipe(strip.Color(0, 0, 0), 6);
    delay(100);
  
    colorWipe(strip.Color(255, 105, 180), 6);
    colorWipe(strip.Color(0, 0, 0), 8);
    delay(100);
  
    colorWipe(strip.Color(255, 0, 255), 6);
    colorWipe(strip.Color(0, 0, 0), 6);
    delay(100);
  
    colorWipe(strip.Color(0, 139, 139), 6);
    colorWipe(strip.Color(0, 0, 0), 6);
    delay(100);

  }


  if(message == "rainbow") {

    #if SERIAL_DEBUG
    Serial.println("rainbow"); 
    #endif

    rainbow(20);

  }

  else
  {

    #if SERIAL_DEBUG
    Serial.println("color"); 
    #endif 
  
    //Parsing String to fetch RGB values independently in each variable:
    int c1 = message.indexOf(',');
    int c2 = message.indexOf(',',c1+1); // RGB values are sent in comma seperated order RR,GG,BB

    int red = message.toInt();
    int green = message.substring(c1+1).toInt(); // Value sent is 8-bit but ESP can do 10 bits PWM hence x By 4
    int blue =  message.substring(c2+1).toInt();

    //Generating PWM corresponding to the RGB values
    #if SERIAL_DEBUG
    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
    #endif

   #if NEOPIXEL
   for(int i=0;i<=NUMPIXELS;i++){

      strip.setPixelColor(i, red, green, blue); // Write data to Neopixel stri[]
      strip.show();
      delay(1);

   }
   #endif

  #if ANALOG_RGB 
  analogWrite(RED,red);
  analogWrite(GREEN,green); // Write data on analog RGB LED strip
  analogWrite(BLUE,blue);
  #endif

  delay(1); // Short delay for smoother animation

 }
}


// If MQTT connection breaks, reconnect to the broker
void reconnect() {

  // Loop until we're reconnected
  String clientId = "ESP8266Client-";
  clientId += String(random(0xffff), HEX); // Generate a random client id, required for MQTT broker

  while (!client.connected()) {
    
    #if SERIAL_DEBUG
    Serial.print("Attempting MQTT connection..."); 
    #endif
    
    if (client.connect(clientId.c_str())) {

      #if SERIAL_DEBUG
      Serial.println("connected"); // Attempt to connect
      #endif
      client.subscribe(TOPIC); // Subscribe to topic defined above

    }

    else {

      #if SERIAL_DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif
      // Wait 5 seconds before retrying
      delay(5000);

    }
  }
}

void loop() {

  if (!client.connected()) {

    reconnect(); // Reconnect to MQTT broker if connection breaks

  }

  client.loop();

}

void colorWipe(uint32_t c, uint8_t wait) {
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {

    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {

  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
  
      }
    }
  }
}

void rainbow(uint8_t wait) {

  uint16_t i, j;

  for(j=0; j<256; j++) {

    for(i=0; i<strip.numPixels(); i++) {

      strip.setPixelColor(i, Wheel((i+j) & 255));
    }

    strip.show();
    delay(wait);
  
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {

  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);

  }

  if(WheelPos < 170) {

    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);

  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);

}
