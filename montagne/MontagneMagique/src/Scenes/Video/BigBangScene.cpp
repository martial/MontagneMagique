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
    setProps();
}

void BigBangScene::setProps() {
    
    float thresold              = configJson["thresold"];
    string targetColorHex       = configJson["color-blob-target"];
    ofColor targetColor         = ofColor::fromHex(ofHexToInt(targetColorHex));
    float colorBlobDistanceMax  = configJson["color-blob-target-distance-max"];
    
    bigBang.colorBlobTarget         = targetColor;
    bigBang.thresold                = thresold;
    bigBang.colorBlobDistanceMax    = colorBlobDistanceMax;
}


void BigBangScene::update() {
    
    if(hasConfigChanged()) {
        
        setProps();
        
    }
    
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
