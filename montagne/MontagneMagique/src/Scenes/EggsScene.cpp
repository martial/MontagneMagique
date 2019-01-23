//
//  EggsScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 23/01/2019.
//

#include "EggsScene.hpp"


void EggsScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    eggs.setup(5, 5, this->dataPath);
    
}

void EggsScene::update() {
    
    eggs.update();

}

void EggsScene::draw() {
    
    float pct = 0.0;
    if(marker->getIsSolidFound()) {
        
        pct = ofMap(marker->timeFoundElapsed, 0, this->animInMillisDelay, 0, 1);
        
        ofSetColor(255, pct * 255);
        
        ofNoFill();
        ofDrawRectangle(0.0, 0.0, marker->width, marker->height);
        ofFill();
        
        eggs.draw();
        
    }
    
   
    
    
    //ofSetColor(255);
    //bug0.draw(0.0, 0.0);
    
}
