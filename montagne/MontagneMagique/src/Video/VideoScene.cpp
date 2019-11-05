#include "VideoScene.hpp"

void VideoScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
}

void VideoScene::update() {
    
}

void VideoScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        ofSetColor(255, pct * 255);
        ofPushMatrix();
        ofScale(1, -1.0);
        ofTranslate(0.0, -marker->height);
        marker->image->draw(0.0, 0.0, marker->width, marker->height);
        ofPopMatrix();
        
    }

    
}
