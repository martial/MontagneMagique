//
//  UndergroundScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#include "UndergroundScene.hpp"


void UndergroundScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    ants.setup(this->dataPath);
    
}

void UndergroundScene::update() {
    
    ants.update();
}

void UndergroundScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        ofSetColor(255, pct * 255);
        ants.draw();
        
    }
    //ofSetColor(255,0, 0);
    //ofDrawRectangle(0.0, 0.0, markerWidth, markerHeight);
    
    //ofSetColor(255);
    //bug0.draw(0.0, 0.0);
    
}
