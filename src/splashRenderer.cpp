#include "splashRenderer.h"



splashRenderer::splashRenderer(){

    largeTitle.loadFont("fonts/Goulong/Goulong.ttf", 50);


}



void splashRenderer::drawTitles(vector<ofPtr<title> > titles){

    vector<ofPtr<title> >::iterator it;
    //increment the fade ins and outs

    for(it = titles.begin(); it != titles.end(); it ++){

        ofSetColor(255, (*it)->alpha);
        ofRectangle r(largeTitle.getStringBoundingBox((*it)->text, 0, 0));
        largeTitle.drawString((*it)->text, screenData::width/2 - r.width/2, screenData::height/2 - r.height/2);

    }

}






