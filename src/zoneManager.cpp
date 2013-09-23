#include "zoneManager.h"

zoneManager::zoneManager()
{
    //ctor
}

void zoneManager::update(map<string, ofPtr<clamourNode> > tNodes)
{

    map<string, ofPtr<zone> >::iterator z_it;
    map<string, ofPtr<clamourNode> >::iterator n_it;

    //find node intersections
    n_it = tNodes.begin();

    while(n_it != tNodes.end())
    {

        if(!n_it->second->getIsActive())
        {

            if(n_it->second->getZonePair())
            {
                //incase the node has jumped straight from one zone to the next
                n_it->second->getZonePair()->removeNode(n_it->second);
                n_it->second->resetZonePair();
            }

            ++n_it;
            continue;

        }

        for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it)
        {

            if(checkInZone(n_it->second, z_it->second)){

                if(n_it->second->getZonePair() == z_it->second)break; //already in the zone

                //new nodes inside the zone

                if(z_it->second->getIsClosed()){
                    repellNode(n_it->second, z_it->second);
                }else{

                    if(n_it->second->getZonePair())
                    {
                        //incase the node has jumped straight from one zone to the next
                        n_it->second->getZonePair()->removeNode(n_it->second);
                        n_it->second->resetZonePair();
                    }

                    n_it->second->setZonePair(z_it->second);
                    z_it->second->addNode(n_it->second);

                    //now update zone reactions
                    if(getReaction(z_it->second))makeReaction(z_it->second);

                    break; //no need to check any more zones for this node
                }

            }
            else if(n_it->second->getZonePair() == z_it->second){

                z_it->second->removeNode(n_it->second);
                n_it->second->resetZonePair();

            }

        }

        ++n_it;

    }

    //call update method here which resets closed zones and increments reactions etc

    for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it)
    {
        z_it->second->update();
        if(z_it->second->getCaptureNodes().size() == 0)z_it->second->setIsClosed(false);

    }


}

bool zoneManager::checkInZone(ofPtr<clamourNode> n, ofPtr<zone> z){


    if(z->getShapeType() == "circle"){

        ofVec2f d = n->getMeanPos_abs() - z->getPos_abs();
        return(d.length() < z->getRadius());

    }

    return false;


}

void zoneManager::repellNode(ofPtr<clamourNode> n, ofPtr<zone> z){

    ofVec2f d = (n->getMeanPos_abs() - z->getPos_abs()).normalize();

    //ultimately methods for other shapes too
    n->setRawPos_abs(z->getPos_abs()+ d * z->getRadius());
    n->modifyHistory();

}

bool zoneManager::getReaction(ofPtr<zone> z){

    if(z->getIsReacting())return false;
    if(z->getCaptureNodes().size() == 1)return true;

    return false;

}

void zoneManager::makeReaction(ofPtr<zone> z){

    z->react(); //start the clock ticking
    z->setChanged(CLAMOUR_ON_OFF);
    z->setIsClosed(true);

}

void zoneManager::createZone(string name)
{

    //there will be alot more detail here at some point
    //perhaps pass the zone from the main app
    ofPtr<zone> z = ofPtr<zone>(new zone());
    mZones[name] = z;

}



void zoneManager::createZone(zone z)
{

    ofPtr<zone> zp = ofPtr<zone>(new zone(z));
    mZones[zp->getName()] = zp;

}

void zoneManager::hideZone(string name)
{

}

void zoneManager::showZone(string name)
{

}

void zoneManager::destroyZone(string name)
{


    if(mZones[name]->getCaptureNodes().size() > 0)
    {

        map<string, ofPtr<clamourNode> > :: iterator it = mZones[name]->getCaptureNodes().begin();

        while(it != mZones[name]->getCaptureNodes().end())
        {

            it->second->resetZonePair();

            ++it;
        }

    }

    mZones.erase(name);

}

void zoneManager::destroyAllZones()
{

    map<string, ofPtr<zone> > :: iterator zit = mZones.begin();

    while(zit != mZones.end())
    {

        if(zit->second->getCaptureNodes().size() == 0)
        {
            ++zit;
            continue;
        }

        map<string, ofPtr<clamourNode> > :: iterator nit = zit->second->getCaptureNodes().begin();


        while(nit != zit->second->getCaptureNodes().end())
        {

            nit->second->resetZonePair();

            ++nit;
        }

        ++ zit;
    }

    mZones.clear();

}

map<string, ofPtr<zone> > zoneManager::getZones()
{
    return mZones;
}

zoneManager::~zoneManager()
{
    //dtor
}
