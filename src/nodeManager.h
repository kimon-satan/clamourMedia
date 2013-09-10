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
    
    void resetNodes();
    void updateNodes();
    
    void switchOnNode(string t_index, float x, float y);
    void switchOffNode(string t_index);
    void updateNodePosition(string t_index, float x, float y);
    
    
    vector<string> getOnNodes();
    ofVec2f getNodePosition(string index);
    ofPtr<clamourNode> getNode(string index);
    
    private:
    
    map< string, ofPtr<clamourNode> > mNodes;
    vector<string> onNodes;


};

#endif /* defined(__clamourMedia__nodeManager__) */
