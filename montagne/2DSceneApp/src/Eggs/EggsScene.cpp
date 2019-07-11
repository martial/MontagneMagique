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
    
}

void EggsScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        eggs.update();
        ofSetColor(255, pct * 255);
        eggs.draw(pct);
        
    }

    
}