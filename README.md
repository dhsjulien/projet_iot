# projet_iot

__Etudiants : LADOUCEUR Salomon, DEBLOUX Mansour, RIEHL Louise, DAHOMAIS Julien__  

## Introduction

## Qu'est-ce qu'un accéléromètre et quelles sont ses principales utilisations ?

Un accéléromètre est un appareil qui mesure son propre mouvement dans l'espace. Il permet de déterminer l'angle de déviation par rapport à la verticale ou les vibrations et les chocs de l'objet auquel il est rattaché. Il existe deux types principaux d'accéléromètres :
l'accéléromètre capacitif, donc le fonctionnement est le suivant : un poids repose sur un ressort qui est lui même attaché au condensateur ; lorsque le poids se déplace sous l'influence de la force, il y a aura un changement de distance entre la masse et le condensateur.  Il est, la plupart du temps, composé en réalité de trois capteurs, soit un pour chacune des directions (X, Y et Z). On retrouve ce type d'accélérateur, peu précis, dans les téléphones (pour déterminer le mode paysage ou portrait) et les voitures (pour déclencher l'airbag en cas de fort freinage).
l'accéléromètre piézoélectrique, dont le fonctionnement est le suivant : certains cristaux ou métaux (tels que le quartz ou le sel de Seignette se chargent électriquement lorsqu'ils sont soumis à une déformation et génèrent donc une tension électrique d'une certaine valeur. On retrouve ce type d'accélérateur, beaucoup plus précis que l'accéléromètre capacitif, dans les machines industrielles afin de mesurer les vibrations et chocs et ainsi déterminer l'usure ou les défauts de l'appareil.

Le projet vise à faire fonctionner un accéléromètre et à pouvoir récupérer les données sur la plateforme IoT ThingsBoard. Les étapes du projet sont visibles sur le schéma "image.webpp". 
Dans un premier temps on fait le montage qui permet de relier l'ESP32 à l'accéléromètre. Ensuite on s'occupe de la partie programmation.  
Une fois ces deux étapes faites, il faut suivre l'ordre des étapes qu'il y a sur le schéma.  

__Etape 1__ : téléverser le programme écrit sur l'IDE d'Arduino vers l'ESP32.  
__Etape 2__ : ESP32 va envoyer ses instructions à l'accéléromètre et lui demander de lui envoyer les informations qui nous intérèsse.  
__Etape 3__ : l'Accélérometre envoie ses informations que l'on a spécifié dans le code  
__Etape 4__ : ESP32 utilise le wifi pour envoyer les informations reçus à la plateforme Thingsboard    

## Explications des branchements
  
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

## Axes d'amélioration

Par la suite, notre maquette pourrait évoluer en quelque chose de plus complet. Avec un peu de créativité, on peut imaginer des fonctionnalités telles qu’un système d’enregistrement de « token » automatique, afin de rendre la connexion avec l’outil ThingsBoard plus simple. On peut aussi imaginer un système de sauvegarde des données transmises dans une base de données, pour anticiper un problème de connexion pour une courte durée (une perte de connexion, par exemple).
De manière générale, l’accéléromètre joue un rôle important dans certains domaines, tels que le domaine médical ou des loisirs (sports, divertissements).  Il est toujours possible d’en améliorer son utilité et sa précision afin de réaliser de nouvelles choses avec cet outil. Des capteurs plus précis pourraient aider à capter des signaux vitaux, ou alors à mieux réagir en cas de problème de sécurité routière. Du côté des loisirs, la Réalité Virtuelle et les outils gyroscopiques en bénéficierait également. Les possibilités ne cessent de grandir.
