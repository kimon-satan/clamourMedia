//
//  nodeManager.cpp
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#include "nodeManager.h"

nodeManager::nodeManager()
{



    //populate nodeArray
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {

            string t_index = ofToString(char(65 + j)) + "_" + ofToString(i + 1);
            ofPtr<clamourNode> t_mn = ofPtr<clamourNode>( new clamourNode(i + 1, ofToString(char(65 + j))));
            t_mn->setDrawType("DEBUG");
            mNodes[t_index] = t_mn;


        }
    }


}

nodeManager::nodeManager(vector<string> indexes)
{


    //populate nodeArray
    for(int i = 0; i < indexes.size(); i++)
    {

        string r = indexes[i].substr(0,1);
        int s = ofToInt(indexes[i].substr(2));
        ofPtr<clamourNode> t_mn = ofPtr<clamourNode>( new clamourNode(s,r));
        t_mn->setDrawType("DEBUG");
        mNodes[indexes[i]] = t_mn;


    }


}


void nodeManager::beginShift(string t_index, float x, float y)
{

    mNodes[t_index]->resetShift(x, y);

}


void nodeManager::updateNodes()
{

    map<string, ofPtr<clamourNode> >::iterator it;

    it = mNodes.begin();

    while(it != mNodes.end()){

        it->second->update();

        if(it->second->getIsFiring()){
            it->second->updateHistory();
            it->second->updateDrawData();
            it->second->updateSoundData();
        }


        ++it;

    }


}

void nodeManager::distributeNodes(vector<string> clients, string pattern, map<string, float> params, bool dimp, bool posp)
{

    float w_prop = (float)screenData::width/screenData::height;

    if(pattern == "RANDOM_CIRCLE")
    {

        ofVec2f c(params["X"], params["Y"]);
        if(posp)c *= ofVec2f(w_prop,1.0);
        float r = params["RADIUS"];

        for(int i = 0; i < clients.size(); i++)
        {

            ofVec2f p(0,ofRandom(0,r));
            p = p.getRotated(ofRandom(-180,180));
            if(dimp)p *= ofVec2f(w_prop,1.0);
            mNodes[clients[i]]->setRawPos_abs(p+c);

        }

    }
    else if(pattern == "CIRCUM")
    {

        ofVec2f c(params["X"], params["Y"]);
        if(posp)c *= ofVec2f(w_prop,1.0);
        float r = params["RADIUS"];

        float interval = (float)360.0/clients.size();

        for(int i = 0; i < clients.size(); i++)
        {

            ofVec2f p(0,r);
            p = p.getRotated(-180 + i * interval);
            if(dimp)p *= ofVec2f(w_prop,1.0);
            mNodes[clients[i]]->setRawPos_abs(p+c);

        }



    }
}


void nodeManager::switchOffAllNodes()
{

    map<string, ofPtr<clamourNode> >::iterator it;

    it = mNodes.begin();

    while(it != mNodes.end()){
        switchOffNode(it->second->getName());
        ++it;
    }

}


void nodeManager::switchOffNodes(vector<string> v)
{

    //change to flags
    for(int i = 0; i < v.size(); i ++)
    {
        switchOffNode(v[i]);
    }


}

void nodeManager::switchOffNode(string t_index)
{

    if(mNodes[t_index]->getIsFired()){
        mNodes[t_index]->setIsFired(false);
        mNodes[t_index]->setChanged(CLAMOUR_ON_OFF);
        mNodes[t_index]->clearHistory();
    }

}

void nodeManager::switchOnNode(string t_index)
{
    if(!mNodes[t_index]->getIsFired()){
        mNodes[t_index]->setIsFired(true);
        mNodes[t_index]->react();
        mNodes[t_index]->setChanged(CLAMOUR_ON_OFF);
    }
}

void nodeManager::switchOnNode(string t_index, float x, float y)
{

    mNodes[t_index]->setRawPos_rel(ofVec2f(x, y));
    switchOnNode(t_index);

}


void nodeManager::updateNodePosition(string t_index, float x, float y)
{
    mNodes[t_index]->setRawPos_rel(ofVec2f(x,y));
}

void nodeManager::shiftNodePosition(string t_index, float x, float y)
{

    if(!mNodes[t_index]->getIsDragOn())return;
    ofVec2f s(x,y);
    ofVec2f p = mNodes[t_index]->getShiftStart() + s * mNodes[t_index]->getShiftAmount();

    mNodes[t_index]->setRawPos_rel(p);
    mNodes[t_index]->setChanged(CLAMOUR_POSITION);

}



map<string, ofPtr<clamourNode> > nodeManager::getNodes(){
        return mNodes;
}

ofVec2f nodeManager::getNodePosition(string index, bool isRel)
{

    return (isRel) ? mNodes[index]->getMeanPos_rel() : mNodes[index]->getMeanPos_abs();

}

ofPtr<clamourNode> nodeManager::getNode(string index)
{

    return mNodes[index];
}

void nodeManager::setNodeDrawType(vector<string> indexes, string dt)
{

    for(int i = 0; i < indexes.size(); i ++)
    {

        mNodes[indexes[i]]->setDrawType(dt);

    }

}


void nodeManager::setNodeSoundType(vector<string> indexes, string st)
{

    baseData sd = mSoundDictionary.createSoundData(st);

    for(int i = 0; i < indexes.size(); i ++)
    {

        mNodes[indexes[i]]->setSoundData(sd);
        if(mNodes[indexes[i]]->getIsFiring())mNodes[indexes[i]]->setChanged(CLAMOUR_SOUND);

    }

}


void nodeManager::setNodeDrawParam(vector<string> indexes, parameter p)
{

    for(int i = 0; i < indexes.size(); i ++)
    {

        p.init(mNodes[indexes[i]]->getMeanPos_rel()); //if mapped randomly only reset that parameter
        mNodes[indexes[i]]->getDrawData()->setParameter(p);

    }

}


void nodeManager::setNodeSoundParam(vector<string> indexes, parameter p)
{

    //sorry could be neater but it's late !

    parameter t = mNodes[indexes[0]]->getSoundData()->getParameter(p.name); //a copy of the original
    p.index = t.index; //copy these over
    p.warp = t.warp;

    for(int i = 0; i < indexes.size(); i ++)
    {

        p.init(mNodes[indexes[i]]->getMeanPos_rel());//if mapped randomly only reset that parameter
        mNodes[indexes[i]]->getSoundData()->setParameter(p);

    }

}




