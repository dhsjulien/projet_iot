# projet_iot

__Etudiants : LADOUCEUR Salomon, DEBLOUX Mansour, RIEHL Louise, DAHOMAIS Julien__  

## Introduction

Le projet vise à faire fonctionner un accéléromètre et à pouvoir récupérer les données sur la plateforme IoT ThingsBoard. Dans un premier temps on fait le montage qui permet de relier l'ESP32 à l'accéléromètre. Ensuite on s'occupe de la partie programmation. Une fois ces deux étapes faites, il faut suivre l'ordre des étapes qu'il y a sur [le schéma](https://raw.githubusercontent.com/dhsjulien/projet_iot/main/Images/image.webp).  

__Etape 1__ : téléverser le programme écrit sur l'IDE d'Arduino vers l'ESP32.  
__Etape 2__ : ESP32 va envoyer ses instructions à l'accéléromètre et lui demander de lui envoyer les informations qui nous intérèsse.  
__Etape 3__ : l'Accélérometre envoie ses informations que l'on a spécifié dans le code  
__Etape 4__ : ESP32 utilise le wifi pour envoyer les informations reçus à la plateforme Thingsboard    

## Qu'est-ce qu'un accéléromètre et quelles sont ses principales utilisations ?

Un accéléromètre est un appareil qui mesure son propre mouvement dans l'espace. Il permet de déterminer l'angle de déviation par rapport à la verticale ou les vibrations et les chocs de l'objet auquel il est rattaché. Il existe deux types principaux d'accéléromètres :
l'accéléromètre capacitif, donc le fonctionnement est le suivant : un poids repose sur un ressort qui est lui même attaché au condensateur ; lorsque le poids se déplace sous l'influence de la force, il y a aura un changement de distance entre la masse et le condensateur.  Il est, la plupart du temps, composé en réalité de trois capteurs, soit un pour chacune des directions (X, Y et Z). On retrouve ce type d'accélérateur, peu précis, dans les téléphones (pour déterminer le mode paysage ou portrait) et les voitures (pour déclencher l'airbag en cas de fort freinage).
l'accéléromètre piézoélectrique, dont le fonctionnement est le suivant : certains cristaux ou métaux (tels que le quartz ou le sel de Seignette se chargent électriquement lorsqu'ils sont soumis à une déformation et génèrent donc une tension électrique d'une certaine valeur. On retrouve ce type d'accélérateur, beaucoup plus précis que l'accéléromètre capacitif, dans les machines industrielles afin de mesurer les vibrations et chocs et ainsi déterminer l'usure ou les défauts de l'appareil.

## Explications des branchements
  
On branche 4 fils qui vont de l'accéléromètre aux broches de l'ESP32 :  
Le premier permet l'alimentation en 3V3 sur l'ESP32 et est branché à VCC sur l'accéléromètre   
Le second est relié à la masse, le GND de l'accéléromètre sur le GND de l'ESP32    
Le troisième est relié au SCL à la broche 22 de l'accélérometre, cette broche permet de synchroniser la transmission de données entre deux appareils, elle génère une horloge qui indique le rythme auquel les bits sont lu ou écrits sur la ligne SDA  
Le quatrième est relié au SDA à la broche 21, SDA permet de transmettre les données entre les deux appareils.  

Une fois le programme pour ESP32 réalisé, on va se créer un compte sur la plateforme iot thingsboard (thingsboard.png) et créer un "device" qui va permettre de récolter les données JSON générées en temps réel que l'on on va les stocker dans un [dashboard](Images/dashboard.png). Parmi ces données on retrouve x, y et z ainsi que la rotation actuelle de l'appareil.

## Axes d'amélioration

Par la suite, notre maquette pourrait évoluer en quelque chose de plus complet. Avec un peu de créativité, on peut imaginer des fonctionnalités telles qu’un système d’enregistrement de « token » automatique, afin de rendre la connexion avec l’outil ThingsBoard plus simple. On peut aussi imaginer un système de sauvegarde des données transmises dans une base de données, pour anticiper un problème de connexion pour une courte durée (une perte de connexion, par exemple).
De manière générale, l’accéléromètre joue un rôle important dans certains domaines, tels que le domaine médical ou des loisirs (sports, divertissements).  Il est toujours possible d’en améliorer son utilité et sa précision afin de réaliser de nouvelles choses avec cet outil. Des capteurs plus précis pourraient aider à capter des signaux vitaux, ou alors à mieux réagir en cas de problème de sécurité routière. Du côté des loisirs, la Réalité Virtuelle et les outils gyroscopiques en bénéficierait également. Les possibilités ne cessent de grandir.
