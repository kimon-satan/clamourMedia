#include "clamourUtils.h"

int screenData::width = 640;
int screenData::height = 480;

int clamourUtils::rowToInt(string r){

    r = r.substr(0,1);
    int i = r[0] - 64; //convert into an integer
    return i;
}


bool clamourUtils::pointInPath(ofPath p, ofVec2f  v){

    vector<ofPolyline> pls = p.getOutline();
    vector<ofPolyline>::iterator it = pls.begin();


    while(it != pls.end()){
        ofRectangle r = it->getBoundingBox();
        if(r.inside(v)){
            if(it->inside(v))return true; // don't bother with ray cast unless already in bounding box
        }
        ++it;
    }

    return false;

}

mapType clamourUtils::stringToMapType(string s){

    if(s == "fixed")return CLAMOUR_MAP_FIXED;
    if(s == "x")return CLAMOUR_MAP_X;
    if(s == "y")return CLAMOUR_MAP_Y;
    if(s == "rand")return CLAMOUR_MAP_RAND;
    if(s == "slave")return CLAMOUR_MAP_SLAVE;
    if(s == "event")return CLAMOUR_MAP_EVENT;

    return CLAMOUR_MAP_FIXED;
}

ofPoint clamourUtils::getInsideIntersect(ofPath p, ofPoint centroid, ofPoint invader){

    //finding centroid is not solved here so this method only works if zones drawn from the center

    ofVec2f d = (invader -centroid).normalize();

    vector<ofPolyline> pls = p.getOutline();
    vector<ofPoint> pts;

    for(int i = 0; i < pls.size(); i++){
        vector<ofPoint> t = pls[i].getVertices(); //filter out to bounding box here
        for(int j = 0; j < t.size(); j++){
            pts.push_back(t[j]);
        }
    }

    float min_neg = -PI;
    float min_pos = PI;
    ofPoint negP, posP;

    for(int i = 0; i < pts.size(); i ++){
        ofVec2f v = (pts[i] - centroid).getNormalized();
        float f = d.angleRad(v);
        if(f == 0){
            negP.set(pts[i]);
            posP.set(pts[i]);
            break;
        }else if(f < 0 && f > min_neg){
            min_neg = f;
            negP = pts[i];
        }else if(f > 0 && f < min_pos){
            min_pos = f;
            posP = pts[i];
        }

    }



    ofVec2f edgeSeg = posP - negP;

    if(edgeSeg.length() == 0){
        return negP;
    }else{
        float prop = abs(min_neg/(min_pos+abs(min_neg)));
        return negP + edgeSeg * prop;
    }


}

ofRectangle clamourUtils::pathToPoints(ofPath p, vector<ofPoint> & pts){

    vector<ofPolyline> pls = p.getOutline();
    ofVec2f minVec(1,1);
    ofVec2f maxVec(0,0);

    for(int i = 0; i < pls.size(); i++){
        vector<ofPoint> t = pls[i].getVertices();
        for(int j = 0; j < t.size(); j++){
            if(t[j].x < maxVec.x)minVec.x = t[j].x;
            if(t[j].y < maxVec.y)minVec.y = t[j].y;
            if(t[j].x > maxVec.x)maxVec.x = t[j].x;
            if(t[j].y > maxVec.y)maxVec.y = t[j].y;
            pts.push_back(t[j]);
        }
    }

    ofVec2f dims = maxVec - minVec;
    return ofRectangle(minVec.x, minVec.y, dims.x, dims.y); //the bounding box

}

bool clamourUtils::pathInPath(ofPath intruder, ofPath host, ofPoint & intersect){

    vector<ofPoint> pts;
    vector<ofPoint> pts_h;

    ofRectangle ri = clamourUtils::pathToPoints(intruder, pts);
    ofRectangle rh = clamourUtils::pathToPoints(host, pts_h);

    for(int i = 0; i < pts.size(); i ++){
        if(rh.inside(pts[i])){ // only bother if it's inside the bounding box
            if(clamourUtils::pointInPath(host, pts[i])){
                intersect = pts[i];
                return true;
            }
        }
    }

    return false;
}

bool clamourUtils::pathOutPath(ofPath target, ofPath host, ofPoint &intersect){

    vector<ofPoint> pts_t;

    clamourUtils::pathToPoints(target, pts_t);

     for(int i = 0; i < pts_t.size(); i ++){ // no possibility for optimisations here
        if(!clamourUtils::pointInPath(host, pts_t[i])){

            intersect = pts_t[i]; //should find furthest really
            return true;
        }
    }

    return false;

}
