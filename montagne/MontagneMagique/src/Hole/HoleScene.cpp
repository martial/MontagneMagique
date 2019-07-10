//
//  HoleScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 11/04/2019.
//

#include "HoleScene.hpp"



void HoleScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    this->bGenerateMask = true;
  
    
}

void HoleScene::update() {
    
    
}


void HoleScene::draw() {
    
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        beginFlip();
        marker->image->draw(ofGetMouseX(), 0.0);
        endFlip();
        
    }
    
    
}
