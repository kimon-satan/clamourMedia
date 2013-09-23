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

            if(z_it->second->getShapeType() == "circle")
            {

                ofVec2f d = n_it->second->getMeanPos_abs() - z_it->second->getPos_abs();

                if(d.length() < z_it->second->getRadius())
                {

                    //the node is inside the zone




                    if(n_it->second->getZonePair() != z_it->second)
                    {

                        //for all nodes not already inside the zone

                        if(z_it->second->getIsClosed())
                        {

                            //repell the node - put this into a function to deal with different shapes

                            n_it->second->setRawPos_abs(z_it->second->getPos_abs()+ d.normalize() * z_it->second->getRadius());
                            n_it->second->modifyHistory();

                            n_it++;
                            continue;
                        }

                        if(n_it->second->getZonePair())
                        {
                            //incase the node has jumped straight from one zone to the next
                            n_it->second->getZonePair()->removeNode(n_it->second);
                            n_it->second->resetZonePair();
                        }

                        n_it->second->setZonePair(z_it->second);
                        z_it->second->addNode(n_it->second);

                        //reactions here - a separate function
                        z_it->second->setIsClosed(true);

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
                        z_it->second->setIsClosed(false);
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
