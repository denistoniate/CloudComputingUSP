#include <SPI.h>
#include <Ethernet.h>

// MAC address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// IP adress and subnet mask
IPAddress ip(192,168,2,20);
IPAddress subnet(255, 255, 255, 0);

//Arduino is gonna be a Http client
EthernetClient client;

// Define Http parameters for connection
int    HTTP_PORT   = 1026;
char   HOST_NAME[] = "192.168.2.10";
String PATH_NAME   = "/v2";

// Device ID (example: urn:ngsi-ld:entity:001)
const char *deviceID = "urn:ngsi-ld:entity:001";

int iTemperature, iHumidity;

void setup() {
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, subnet);
  Serial.print("client is at ");
  Serial.println(Ethernet.localIP());

   iTemperature = 0;
   iHumidity = 0;

 }
void loop() {
  /*
  // connect to web server on port HTTP_PORT:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    
    // if connected:
    Serial.println("Connected to server");

    orionGetEntities();
    HttpRead();
    HttpClose();

  } else {// if not connected:
    Serial.println("connection failed");
  }

  delay(1000);*/
  
  // connect to web server on port HTTP_PORT:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    
    // if connected:
    Serial.println("Connected to server");
    
    orionGetEntity(deviceID);
    HttpRead();
    HttpClose();
   
  } else {// if not connected:
    Serial.println("connection failed");
  }

  delay(1000);

  // connect to web server on port HTTP_PORT:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    
    // if connected:
    Serial.println("Connected to server");
    
    orionUpdateHumidity(deviceID, iHumidity);
    HttpClose();
   
  } else {// if not connected:
    Serial.println("connection failed");
  }

  delay(1000);

  // connect to web server on port HTTP_PORT:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    
    // if connected:
    Serial.println("Connected to server");
    
    orionUpdateTemperature(deviceID, iTemperature);
    HttpClose();
   
  } else {// if not connected:
    Serial.println("connection failed");
  }

  delay(1000);

  if(iTemperature > 99) {
    iTemperature = 0;
  }
  else{
    iTemperature = iTemperature + 2;
  }

  if(iHumidity > 99) {
    iHumidity = 0;
  }
  else{
    iHumidity = iHumidity + 3;
  }
  
}

void HttpRead()
{
  while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
}

void HttpClose()
{
    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
}

//Check orion communication and version
void orionVersion()
{
    String HTTP_METHOD = "GET";
    
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + "/version" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME) + ":" + String(HTTP_PORT));
    client.println("Connection: close");
    client.println(); // end HTTP header
}

//Get all the entities available in the server
void orionGetEntities()
{
    String HTTP_METHOD = "GET";

    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + "/v2/entities/" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME) + ":" + String(HTTP_PORT));
    client.println("Accept: application/json");
    client.println("fiware-service: helixiot");
    client.println("fiware-servicepath: /");
    client.println("Connection: close");
    client.println(); // end HTTP header

}

//Get a specific entity available in the server
void orionGetEntity(String entityID)
{
    String HTTP_METHOD = "GET";

    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + "/v2/entities/" + entityID + "/" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME) + ":" + String(HTTP_PORT));
    client.println("Accept: application/json");
    client.println("fiware-service: helixiot");
    client.println("fiware-servicepath: /");
    client.println("Connection: close");
    client.println(); // end HTTP header
}

// Update humidity value in the Helix Sandbox
void orionUpdateHumidity(String entityID, int humidity)
{
    String HTTP_METHOD = "PUT";
    
    client.println(HTTP_METHOD + " " + "/v2/entities/" + entityID + "/attrs/humidity/value" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME) + ":" + String(HTTP_PORT));
    client.println("Content-Type: application/json");
    client.println("fiware-service: helixiot");
    client.println("fiware-servicepath: /");
    client.println("Content-Type: text/plain");
    client.println("Content-Length: 2");
    client.println("");
    client.println(power);
    client.println("Connection: close");
    client.println(); // end HTTP header
}

// Update temperature value in the Helix Sandbox
void orionUpdateTemperature(String entityID, int temperature)
{
    String HTTP_METHOD = "PUT";
    
    client.println(HTTP_METHOD + " " + "/v2/entities/" + entityID + "/attrs/temperature/value" + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME) + ":" + String(HTTP_PORT));
    client.println("Content-Type: application/json");
    client.println("fiware-service: helixiot");
    client.println("fiware-servicepath: /");
    client.println("Content-Type: text/plain");
    client.println("Content-Length: 2");
    client.println("");
    client.println(temperature);
    client.println("Connection: close");
    client.println(); // end HTTP header
}
