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

void EggsScene::draw(int markerIndex, int markerWidth, int markerHeight) {
    
    ofSetColor(255);
    
    
    
    ofNoFill();
    ofDrawRectangle(0.0, 0.0, markerWidth, markerHeight);
    ofFill();
    
    eggs.draw();
    
    
    //ofSetColor(255);
    //bug0.draw(0.0, 0.0);
    
}
