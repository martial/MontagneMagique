//
//  UndergroundScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#include "UndergroundScene.hpp"


void UndergroundScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    bug0.load(this->dataPath+"/bug_0.png");
    bug1.load(this->dataPath+"/bug_1.png");

    
}

void UndergroundScene::update() {
    
}

void UndergroundScene::draw(int markerIndex, int markerWidth, int markerHeight) {
    
    //ofSetColor(255,0, 0);
    //ofDrawRectangle(0.0, 0.0, markerWidth, markerHeight);
    
    ofSetColor(255);
    bug0.draw(0.0, 0.0);
    
}
