/**
 * @file MyTemp.h
 * @author Thomas Baudry
 * @brief  Gestion de température pour l'écran Stone
 * @version 1.0
 * @date 2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 *  Historique des versions   
 *          Versions  Date      Auteur      Description
 *          1.0        2022-04-20     TB       Première version
 *     board = esp32doit-devkit-v1
 *     framework = arduino
 *     lib_deps = 
 *     Autres librairies (à copier dans le répertoire lib)
 *         Aucune
 *     GPIO 
 *         GPIO utilisés pour le ESP32
 *         I2C
 *             SCL     :   GPIO22
 *             SDA     :   GPIO21
 *             VCC     :   3.3v
 *             Gnd     :   Gnd
 * 
 *
 *
 *    Exemple d'utilisation 
 *        
**/
#include "DHT.h"

#ifndef MYTEMP_H
#define MYTEMP_H

#define DHTPIN 21     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

class MyTemp {
    private:
        DHT *dht;
    public:
        MyTemp();
        ~MyTemp() { };
        float ReadTemp();
};
#endif