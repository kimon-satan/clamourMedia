//
//  nodeManager.h
//  clamourMedia
//
//  Created by Simon Katan on 06/09/2013.
//
//

#ifndef __clamourMedia__nodeManager__
#define __clamourMedia__nodeManager__

#include "ofMain.h"
#include "clamourNode.h"

class nodeManager{

    public:

    nodeManager();
    nodeManager(vector<string> indexes);

    void resetNodes();
    void updateNodes();


    void switchOnNode(string t_index);
    void switchOnNode(string t_index, float x, float y);

    void switchOffAllNodes();
    void switchOffNode(string t_index);
    void switchOffNodes(vector<string> v);

    void beginShift(string t_index, float x, float y);
    void updateNodePosition(string t_index, float x, float y);
    void shiftNodePosition(string t_index, float x, float y);
    void updateOnlineClients(vector<string> v);
    vector<string> getOnlineClients();
    bool getIsClientOnline(string t_index);

    vector<string> getOffNodes();
    vector<string> getOnNodes();
    ofVec2f getNodePosition(string index);
    ofPtr<clamourNode> getNode(string index);

    void distributeNodes(vector<string> clients, string pattern, map<string, float> params, bool dimp, bool posp);

    void setNodeDrawType(vector<string> indexes, string dt);
    void setNodeDrawParam(vector<string> indexes, parameter p);
    void setNodeSoundType(vector<string> indexes, string st);
    void setNodeSoundParam(vector<string> indexes, parameter p);

    void setCtrlIndexes(vector<string> clients, int len);
    void flagNodesReturn(vector<string> clients);
    void flagNodeReturn(string client);
    void setScreenProp(float p);



    private:

    map< string, ofPtr<clamourNode> > mNodes;
    vector<string> onNodes;
    vector<string> offNodes;
    vector<string> mOnlineClients;

    soundDictionary mSoundDictionary;
    float screenProp;

};

#endif /* defined(__clamourMedia__nodeManager__) */
