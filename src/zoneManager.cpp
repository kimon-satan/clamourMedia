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

        if(!n_it->second->getIsActive()){

             if(n_it->second->getZonePair()){
                            //incase the node has jumped straight from one zone to the next
                            n_it->second->getZonePair()->removeNode(n_it->second);
                            n_it->second->resetZonePair();
                        }

            ++n_it;
            continue;

        }

        for(z_it = mZones.begin(); z_it != mZones.end(); ++z_it)
        {


            if(z_it->second->getShapeType() == "circle")
            {

                ofVec2f d = n_it->second->getMeanPos_abs() - z_it->second->getPos_abs();

                if(d.length() < z_it->second->getRadius())
                {

                    if(n_it->second->getZonePair() != z_it->second)
                    {

                        if(n_it->second->getZonePair()){
                            //incase the node has jumped straight from one zone to the next
                            n_it->second->getZonePair()->removeNode(n_it->second);
                            n_it->second->resetZonePair();
                        }

                        n_it->second->setZonePair(z_it->second);
                        z_it->second->addNode(n_it->second);

                    }

                    n_it++; //no need to do anything else (nodes can't occupy 2 zones)
                    continue;

                }
                else
                {
                    if(n_it->second->getZonePair() == z_it->second)
                    {
                        z_it->second->removeNode(n_it->second);
                        n_it->second->resetZonePair();
                    }
                }

            }

            ++n_it;

        }


    }


}

void zoneManager::createZone(string name)
{

    //there will be alot more detail here at some point
    //perhaps pass the zone from the main app
    ofPtr<zone> z = ofPtr<zone>(new zone());
    mZones[name] = z;

}

void zoneManager::hideZone(string name)
{

}

void zoneManager::showZone(string name)
{

}

void zoneManager::destroyZone(string name)
{

    //probably flag for destroy incase synths are attached
}

map<string, ofPtr<zone> > zoneManager::getZones()
{
    return mZones;
}

zoneManager::~zoneManager()
{
    //dtor
}
