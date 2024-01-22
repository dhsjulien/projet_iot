#include <Wire.h>
#include <MPU6050.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TestWifi";
const char* password = "wifitest";
const char* thingsboardServer = "thingsboard.cloud";
const char* token = "em2WdOkLcDM3AM03H2bf";
const char* vert = "Portrait";
const char* hor = "Paysage";


WiFiClient espClient;
PubSubClient client(espClient);
MPU6050 accel;

void setup() {
  Serial.begin(115200);


  // pour se co en wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connecté au réseau WiFi");

  // Configurer le serveur ThingsBoard
  client.setServer(thingsboardServer, 1883);

  // Initialiser l'accéléromètre
  Wire.begin();
  accel.initialize();
}

void loop() {
  

  
  //Pour se co à thingsboard
  if (!client.connected()) {
    reconnect();
  }

  // Pour avoir les données de l'accéléromètre
  int16_t ax, ay, az;
  accel.getAcceleration(&ax, &ay, &az);
  


  // Transformer les données en JSON
  String payload = "{";
  payload += "\"x\":" + String(ax);
  payload += ", \"y\":" + String(ay);
  payload += ", \"z\":" + String(az);
  
  //permet de déterminer la rotation de l'accéléromètre
  if(abs(ax)>abs(ay)){
    if(ax > 0){
      payload += ", \"Rotation\":" + String(hor);
    }
    else{
      payload += ", \"Rotation\":" + String(hor);
    }
  }
  else{
    if(ay > 0){
      payload += ", \"Rotation\":" + String(vert);
    }
    else{
      payload += ", \"Rotation\":" + String(vert);
    }
  }
 
  payload += "}";

  // Publier les données sur Thingsboard
  client.publish("v1/devices/me/telemetry", payload.c_str());

  // 1 seconde entre chaque données envoyé
  delay(1000);
  

}

void reconnect() {
  // Boucle jusqu'à ce que nous soyons reconnectés à ThingsBoard
  while (!client.connected()) {
    Serial.print("Tentative de connexion à ThingsBoard...");
    
    // Pour se connecter à l'aide du Token
    if (client.connect("ESP32_Device_ID", token, NULL)) {
      Serial.println("Connecté!");
    } else {
      Serial.print("Échec, rc=");
      Serial.print(client.state());
      Serial.println(" Réessayez dans 5 secondes...");
      delay(5000);
    }
  }
}