/**
 * @file    main.cpp
 * @author  Thomas Baudry
 * @brief   Utilisation de la classe myStone et myTemp
 * @version 1.0
 * @date    2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * 
 *     Historique des versions
 *         Version         Date     Auteur      Description
 *           1.0        22/04/22     TB       Première version
 * 
 *     platform = espressif32
 *     board = esp32doit-devkit-v1
 *     framework = arduino
 *     lib_deps = 
 *     Autres libairies (à copier dans le répertoire lib)
 *  
 *     GPIO utilisés pour le ESP32
 *         I2C
 *             SCL     :   GPIO22
 *             SDA     :   GPIO21 
 *             VCC     :   3.3v
 *             Gnd     :   Gnd
 *     STONE
 *         Ecran Dynamique Stone     
 *      
**/
#include <Arduino.h>

#define RXD2 18//16
#define TXD2 19//17
#define BAUD_RATE 115200

#include <iostream>

#include "MyButton.h"
MyButton *myButtonT4 = new MyButton();
MyButton *myButtonT5 = new MyButton();

#include "MyStone.h"
MyStone *myStone;

#include "MyTemp.h"
MyTemp *myTemp;

int BoutonDemarre = 0;
float temp = 0;
float temp_minimal = 25;
float temps_sechage = 20;
float nb = 0;

std::string intToHexa(int value){
  char buffer[10];
  sprintf(buffer , "0x%4X", value);
  return (buffer);
};

std::string charToString(char str, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), str);
  return (buffer);
};

std::string intToString(int value, std::string formatStr){
  char buffer[10];
  sprintf(buffer , formatStr.c_str(), value);
  return (buffer);
};



//Thread qui permet de LOOPER et lire le contenu du port serie
//avec l'aide du la fonction getValidsDatasIfExists
void readStoneData() {
  datasRead rd = myStone->getValidsDatasIfExists();
  //std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
  switch(rd.id){

      case 0x0002: { //Version
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneVersion = rd.name;
          std::cout << "Version : " <<  stoneVersion.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));

          break;
          }
      case 0x1001: { //Button
          std::cout << "GData : " << intToHexa(abs(rd.id)) << " " << rd.command << " " << rd.name << " " << rd.type << "\n";
          std::string stoneName = rd.name;
          std::cout << "Button : " <<  stoneName.c_str() << "\n";

          //std::this_thread::sleep_for(std::chrono::milliseconds(10));
          if(strcmp(stoneName.c_str(), "four_demarrage") == 0 && rd.type == 1){ 
            BoutonDemarre = 1;
          }

          break;
          }
      }
  if(rd.id<0) std::cout << "Data received ( id: : " << intToHexa(abs(rd.id)) << "  Command: " << rd.command << " Type: " << rd.type<< ")\n";
}


void setup() {
    
  Serial.begin(9600);

  Serial.println("Stone serial Txd is on pin: "+String(TXD2));
  Serial.println("Stone serial Rxd is on pin: "+String(RXD2));
  myStone = new MyStone(115200, SERIAL_8N1, RXD2, TXD2);
  

  myButtonT4->init(T4);
  myButtonT4->autoSensibilisation(); //Trouve la sensibilité automatiquement

  myButtonT5->init(T5);
  myButtonT5->autoSensibilisation(); //Trouve la sensibilité automatiquement

  myTemp = new MyTemp();

  cout << std::string("Début de l'exemple Stone de base pour le ESP32")  << "\n";

   // Affichage des information relative au bois choisie.
    myStone->setLabel("label_bois", "Erable");
    myStone->setLabel("label_type", "Dur");
    myStone->setLabel("label_origine", "US");
    char sechage[10];
    sprintf(sechage, "%4.0f secondes", temps_sechage);
    myStone->setLabel("label_sechage", sechage);
    char minimum[10];
    sprintf(minimum, "%4.1f°C", temp_minimal);
    myStone->setLabel("label_minimum", minimum);

    //Affichage des information relative du bois dans le four.
    myStone->setLabel("four_bois", "Erable");
    myStone->setLabel("four_minimum", "25°C");
    myStone->setLabel("four_etat", "");
}

void loop() {
  char buffer[10];
  readStoneData();

  if(BoutonDemarre == 1){
    if(temp >= temp_minimal*0.90 && temp <= temp_minimal*1.10){
      while(nb < temps_sechage && (temp >= temp_minimal*0.90 && temp <= temp_minimal*1.10)){
        delay(1000);

        nb++;
        char secondes[20];
        sprintf(secondes, "%4.0f s / %4.0f s", nb, temps_sechage);
        myStone->setLabel("four_secondes", secondes);

        temp = myTemp->ReadTemp();
        sprintf(buffer, "%4.1f °C", temp);
        myStone->setLabel("temperature", buffer);
      }
      if(nb < 20){
        myStone->setLabel("four_etat", "Température trop éloigné.");
      }else{
        myStone->setLabel("four_etat", "Cuissson terminé.");
      }
      nb = 0;
    }
    BoutonDemarre = 0;
  }

  int buttonActionT4 = myButtonT4->checkMyButton();
      if(buttonActionT4 > 2)  {  //Si appuyé plus de 0.2 secondes
            Serial.println("Button T4 pressed");
            if(myStone) myStone->changePage();
            }
  int buttonActionT5 = myButtonT5->checkMyButton();
      if(buttonActionT5 > 2)  {  //Si appuyé plus de 0.2 secondes
            Serial.println("Button T5 pressed");
            //Dans la version  1.2, nous allons remplacer ces lignes pour utiliser la
            //méthode getVersion()
            //char cmdFormat2[99];
            //strcpy(cmdFormat2, "ST<{\"cmd_code\":\"sys_version\",\"type\":\"system\"}>ET");
            //std::cout << cmdFormat2 << "\n";
            //myStone->writeIt((char*)cmdFormat2);
            if(myStone) myStone->getVersion();
            Serial.print("La température est de : ");
            Serial.println(temp);
          }
    temp = myTemp->ReadTemp();
    sprintf(buffer, "%4.1f °C", temp);
    myStone->setLabel("temperature", buffer);

   
  }