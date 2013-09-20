#include "zoneManager.h"

zoneManager::zoneManager()
{
    //ctor
}

void zoneManager::update(){

    map<string, ofPtr<zone> >::iterator it;

    for(it = mZones.begin(); it != mZones.end(); it++){

        //do some stuff - eg check for collisions
    }


}

void zoneManager::createZone(string name){

    //there will be alot more detail here at some point
    //perhaps pass the zone from the main app
    ofPtr<zone> z = ofPtr<zone>(new zone());
    mZones[name] = z;

}

void zoneManager::hideZone(string name){

}
void zoneManager::showZone(string name){

}

void zoneManager::destroyZone(string name){

    //probably flag for destroy incase synths are attached
}

zoneManager::~zoneManager()
{
    //dtor
}
