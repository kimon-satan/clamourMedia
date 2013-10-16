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
#include "pathFactory.h"
#include "presetStore.h"

class nodeManager{

    public:

    nodeManager();
    nodeManager(vector<string> indexes);

    void setupRegions();

    void resetNodes();
    void updateNodes();
    void checkForCollisions(ofPtr<clamourNode> n);
    void addToCollisionRegions(ofPtr<clamourNode> n);

    void switchOnNode(string t_index);
    void switchOnNode(string t_index, float x, float y);

    void switchOffAllNodes();
    void switchOffNode(string t_index);
    void switchOffNodes(vector<string> v);

    void wakeupNode(string t_index);

    void beginShift(string t_index, float x, float y);
    void updateNodePosition(string t_index, float x, float y);
    void shiftNodePosition(string t_index, float x, float y);

    map<string, ofPtr<clamourNode> > getNodes();
    ofVec2f getNodePosition(string index, bool isRel);
    ofPtr<clamourNode> getNode(string index);

    void distributeNodes(vector<string> clients, string pattern, map<string, float> params, bool dimp, bool posp);

    void implementReactions(ofPtr<clamourNode> n, ofPtr<clamourNode> tgt);
    void implementReactions(ofPtr<clamourNode> n, bool isOn);
    void implementReaction(reaction & r, ofPtr<clamourNode> n, bool isOn = true);

    void setNodeAttSecs(vector<string> indexes, float att);
    void setNodeDecSecs(vector<string> indexes, float dec);
    void killNode(string index);
    void killNodes(vector<string> indexes);
    void killAllNodes();

    //add parameter versions for randomisation later

    void setNodeDraw(vector<string> indexes, baseData &bd);
    void setNodeDrawParam(vector<string> indexes, parameter &p);
    void setNodeSound(vector<string> indexes, baseData &bd);
    void setNodeSoundParam(vector<string> indexes, parameter &p);
    void setNodes(vector<string> indexes, clamourNode &n);

    static void setNode(ofPtr<clamourNode> target, clamourNode &temp);
    map<string, string> getAppReactions();

    private:

    map< string, ofPtr<clamourNode> > mNodes;
    vector<vector<ofPtr<clamourNode> > >mCollisionMap;
    vector<ofRectangle> mCollisionMapBounds;


    map<string, string> appReactions;
    vector<eventComm> mFutureEvents;


};

#endif /* defined(__clamourMedia__nodeManager__) */
