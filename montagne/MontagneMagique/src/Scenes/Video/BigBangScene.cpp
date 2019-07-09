//
//  BigBangScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#include "BigBangScene.hpp"

void BigBangScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    bigBang.setup(this->dataPath);
   
}

void BigBangScene::update() {
    
    bigBang.update();
        
}


void BigBangScene::draw() {
    
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        ofSetColor(255, pct * 255);
        ofPushMatrix();
        bigBang.draw();
        ofPopMatrix();
        
    }
    
    
}
