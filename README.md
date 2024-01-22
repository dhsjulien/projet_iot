# projet_iot

__Etudiants : LADOUCEUR Salomon, DEBLOUX Mansour, RIEHL Louise, DAHOMAIS Julien__  

Le projet vise à faire fonctionner un accéléromètre et à pouvoir récupérer les données sur une plateforme iot, sur "image.webpp" il y a le schéma qui montre toutes les étapes du projet.  
Dans un premier temps on fait le montage qui permet de relier l'ESP32 à l'accéléromètre. Ensuite on s'occupe de la partie programmation.  
Une fois ces deux étapes faites, il faut suivre l'ordre des étapes qu'il y a sur le shéma.  

__Etape 1__ : téléverser le programme écrit sur l'IDE d'Arduino vers l'ESP32.  
__Etape 2__ : ESP32 va envoyer ses instructions à l'accéléromètre et lui demander de lui envoyer les informations qui nous intérèsse.  
__Etape 3__ : l'Accélérometre envoie ses informations que l'on a spécifié dans le code  
__Etape 4__ : ESP32 utilise le wifi pour envoyer les informations reçus à la plateforme Thingsboard    

Pour le branchement (voir branchement.png) :  
On branche 4 fils qui vont de l'accéléromètre aux broches de l'ESP32 :  
Le premier permet l'alimentation en 3V3 sur l'ESP32 et est branché à VCC sur l'accéléromètre   
Le second est relié à la masse, le GND de l'accéléromètre sur le GND de l'ESP32    
Le troisième est relié au SCL à la broche 22 de l'accélérometre, cette broche permet de synchroniser la transmission de données entre deux appareils, elle génère une horloge qui indique le rythme auquel les bits sont lu ou écrits sur la ligne SDA  
Le quatrième est relié au SDA à la broche 21, SDA permet de transmettre les données entre les deux appareils.  

Pour la partie programmation se référer à "projet_esp32_acceleromater_hor_ver.ino :  

//Cette partie là sert à inclure les bibliothèques qu'on va utiliser, Wire sert à la communication entre ESP32 et l'accéléromètre  
#include <Wire.h>  
#include <MPU6050.h>  //lib de l'accélérometre  
#include <WiFi.h>  
#include <PubSubClient.h>    

//les constantes qu'on va utiliser
const char* ssid = "nom_du_réseau";  //nom du réseau  
const char* password = "mot_de_passe_du_réseau"; //mdp  
const char* thingsboardServer = "thingsboard.cloud"; //plateforme sur laquelle envoyer le code  
const char* token = "em2WdOkLcDM3AM03H2bf"; //token qu'on a généré sur thingsboard  
const char* vert = "Portrait"; //Indique que l'accélérometre est en mode portrait  
const char* hor = "Paysage";  ////Indique que l'accélérometre est en mode paysage  

//permet de créer un client sur thingsboard  
WiFiClient espClient;  
PubSubClient client(espClient);  

// Objet qui va nous permettre d'utiliser la librairie de l'accélérometre  
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


//fonction pour se reconnecter si ça ne marche pas  
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


Une fois que cela est fait on va se créer un compte sur la plateforme iot thingsboard (thingsboard.png) et on va créer un "device" qui va permettre de récolter les données JSON qui arrive en temps réelle et on va les stocker dans un dashboard (voir dashboard.png)  
Parmis ces données on retrouve x, y et z ainsi que la rotation actuelle de l'appareil  









