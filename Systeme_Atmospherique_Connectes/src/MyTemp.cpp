/**
    Gestion d'un écran LCD
    @file MyTemp.cpp
    @author Thomas Baudry
    @version 1.0 2022-04-20
*/
#include "MyTemp.h"
#include "DHT.h"



MyTemp::MyTemp(){
    dht = new DHT(DHTPIN, DHTTYPE);
    dht->begin();             
}

float MyTemp::ReadTemp(){
    return dht->readTemperature();
}