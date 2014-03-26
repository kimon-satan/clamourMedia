#include "pathFactory.h"

void pathFactory::createPath(ofPath &p, string shapeType, float t_size){

    pathFactory::createPath(p, shapeType, 1.0, 1.0, t_size);

}

void pathFactory::createPath(ofPath &p, string shapeType, float x_dim, float y_dim, float t_size){

    if(shapeType == "mousePointer")pathFactory::mousePointer(p, x_dim, y_dim, t_size);
    if(shapeType == "mousePointerStraight")pathFactory::mousePointerStraight(p, x_dim, y_dim, t_size);
    if(shapeType == "roundedRect")pathFactory::roundedRect(p, x_dim, y_dim, t_size);

}

void pathFactory::roundedRect(ofPath &p, float x_dim, float y_dim, float t_size){

    ofRectangle r;
    r.setFromCenter(0,0, x_dim * t_size, y_dim * t_size);
    //p.rectRounded(r, t_size/10); //need to draw manually in of0072
    //a rect just for testing
    p.lineTo(ofPoint(r.x, r.y));
    p.lineTo(ofPoint(r.x + r.width, r.y));
    p.lineTo(ofPoint(r.x + r.width, r.y + r.height));
    p.lineTo(ofPoint(r.x, r.y + r.height));
    p.lineTo(ofPoint(r.x, r.y));
    p.close();
    



}

void pathFactory::mousePointer(ofPath &p, float x_dim, float y_dim, float t_size){

    vector<ofPoint> tpoints;

    float tw = 0.3;
    float sw = 0.074;
    float cl = 1;
    float al = 0.74;
    float bl = 0.55;

    tpoints.push_back(ofPoint(0,0));
    tpoints.push_back(ofPoint(-tw,al));//a1
    tpoints.push_back(ofPoint(-sw,bl)); // b1
    tpoints.push_back(ofPoint(-sw,cl)); //c1
    tpoints.push_back(ofPoint(sw,cl));//c2
    tpoints.push_back(ofPoint(sw,bl)); // b2
    tpoints.push_back(ofPoint(tw,al)); //a2
    tpoints.push_back(ofPoint(0,0));

    float a = tan(tw/al);

    for(int i = 0; i < tpoints.size(); i++){
        tpoints[i].rotateRad(0,0,-a);
        tpoints[i] *= ofVec2f(x_dim, y_dim);
        tpoints[i] *= t_size;
        p.lineTo(tpoints[i]);
    }

    p.close();

}


void pathFactory::mousePointerStraight(ofPath &p, float x_dim, float y_dim, float t_size){

    vector<ofPoint> tpoints;

    float tw = 0.3;
    float sw = 0.074;
    float cl = 1;
    float al = 0.74;
    float bl = 0.55;

    tpoints.push_back(ofPoint(0,0));
    tpoints.push_back(ofPoint(-tw,al));//a1
    tpoints.push_back(ofPoint(-sw,bl)); // b1
    tpoints.push_back(ofPoint(-sw,cl)); //c1
    tpoints.push_back(ofPoint(sw,cl));//c2
    tpoints.push_back(ofPoint(sw,bl)); // b2
    tpoints.push_back(ofPoint(tw,al)); //a2
    tpoints.push_back(ofPoint(0,0));


    for(int i = 0; i < tpoints.size(); i++){
        tpoints[i] *= ofVec2f(x_dim, y_dim);
        tpoints[i] *= t_size;
        p.lineTo(tpoints[i]);
    }

    p.close();

}
