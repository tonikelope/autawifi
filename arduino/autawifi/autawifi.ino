#include <ESP8266WiFi.h>

const char * ssid = "YOUR SSID"; 
const char * password = "YOUR PASSWORD"; 

int sensorValue = 0;

const int led = 2; //Blue on board LED is on PIN2.  Active LOW.  Blinks it between reads
const int bell = D6; //IMPORTANT. YOU NEED A PULLUP

const char * host = "192.168.1.5"; //Your PC, Laptop, RaspberryPI, etc running HTTP SERVER for receive notification
const int httpPort = 7123;
const char * url = "/";

void setup() { // This is run once.
    
    pinMode(led, OUTPUT); // set up the onbaord LED pin as an output.
    pinMode(bell, INPUT_PULLUP); //IMPORTANTE EL PULLUP 
    delay(500);    
    
    Serial.begin(115200);

    // wait until serial port opens for native USB devices
    while (!Serial) {
        delay(10);
    }

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println();
    
    Serial.println("WiFi connected");
    
    Serial.println(ssid);
    
    Serial.println(WiFi.localIP());

    for (int x = 0; x < 10; x++) { 

        digitalWrite(led, !digitalRead(led));

        delay(500);
    } 
}

void loop() { 
   
    sensorValue = digitalRead(bell);
    digitalWrite(led, !digitalRead(led));
    Serial.println("Waiting for DoorBell....");
    Serial.println(sensorValue);
    delay(50);

    if (sensorValue == LOW) {
      
        Serial.println("----------Doorbell Dectected----------");

        dingdong();

        do
        {   
            digitalWrite(led, !digitalRead(led));
            Serial.println("Doorbell in process...");
            delay(500);
            sensorValue = digitalRead(bell);
        }while(sensorValue == LOW);
    }
}

void dingdong() {
    WiFiClient client;
    
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }
  
    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    
    delay(500); // Delay for web traffic; maybe not required. 
    
    // Read all the lines of the reply from server and print them to Serial
    while (client.available()) {
        Serial.println(client.readStringUntil('\r'));
    }
}
