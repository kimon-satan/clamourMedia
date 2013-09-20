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

    // soundDictionary::setup(); //load in default synth parameters from XML

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

    //soundDictionary::setup(); //load in default synth parameters from XML

    //populate nodeArray
    for(int i = 0; i < indexes.size(); i++)
    {

        string r = indexes[i].substr(0,1);
        int s = ofToInt(indexes[i].substr(2,1));
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

    //do not turn nodes off before here !

    for(int i = 0; i < mActiveNodes.size(); i ++)
    {

        mActiveNodes[i]->updateHistory();
        mActiveNodes[i]->updateDrawData();
        mActiveNodes[i]->updateSoundData();

    }

    //purge old nodes
    for(int i = 0; i < mOffNodes.size(); i++)
    {

        mOffNodes[i]->setIsOn(false);

        if(!mOffNodes[i]->getIsReturnToOn())  // if the this is flagged then the node will be swtiched straight back on with a new synth
        {
            vector<ofPtr<clamourNode> >::iterator it = remove(mActiveNodes.begin(), mActiveNodes.end(), mOffNodes[i]);
            if(it != mActiveNodes.end())mActiveNodes.erase(it);
        }
        else
        {
            mOffNodes[i]->setIsReturnToOn(false);
        }

    }


    mOffNodes.clear(); //oscManager already used them so no need to retain

    //turn on any flagged nodes




    // any nodes turned off after here will be switched off in the subsequent frame






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

    //turn off all current nodes
    for(int i = 0; i < mActiveNodes.size(); i ++)
    {

        mActiveNodes[i]->clearHistory();

    }

    mOffNodes = mActiveNodes;


}


void nodeManager::switchOffNodes(vector<string> v)
{

    for(int i = 0; i < v.size(); i ++)
    {

        mNodes[v[i]]->clearHistory();
        if(find(mActiveNodes.begin(), mActiveNodes.end(), mNodes[v[i]]) != mActiveNodes.end())mOffNodes.push_back(mNodes[v[i]]);

    }


}

void nodeManager::switchOffNode(string t_index)
{

    mNodes[t_index]->clearHistory();
    if(find(mActiveNodes.begin(), mActiveNodes.end(), mNodes[t_index]) != mActiveNodes.end())mOffNodes.push_back(mNodes[t_index]);

}

void nodeManager::switchOnNode(string t_index)
{

    if(find(mActiveNodes.begin(), mActiveNodes.end(), mNodes[t_index]) ==mActiveNodes.end())mActiveNodes.push_back(mNodes[t_index]);

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

}



vector<ofPtr<clamourNode> > nodeManager::getActiveNodes()
{
    return mActiveNodes;
}

vector<ofPtr<clamourNode> > nodeManager::getOffNodes()
{
    return mOffNodes; //recently turned off Nodes
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




void nodeManager::flagNodesReturn(vector<string> clients)
{

    for(int i =0; i < clients.size(); i++)
    {

        mNodes[clients[i]]->setIsReturnToOn(true);
    }
}

void nodeManager::flagNodeReturn(string client)
{

    mNodes[client]->setIsReturnToOn(true);
}

