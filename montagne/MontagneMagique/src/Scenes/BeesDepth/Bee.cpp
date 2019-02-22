//
//  Bee.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 22/02/2019.
//

#include "Bee.hpp"

void Bee::setInitialStatus(ofVec2f pos, float scale) {
    
    this->pos           = pos;
    this->targetPos     = pos;
    
    this->scale         = scale;
    this->targetScale   = scale;

    
}


void Bee::update() {
    
    float blurRate = 0.92;
    
    pos     = blurRate * pos   +  (1.0f - blurRate) * targetPos;
    scale   = blurRate * scale   +  (1.0f - blurRate) * targetScale ;
    
}


void Bee::setPosition(ofVec2f pos) {
    
    targetPos = pos;
    
}

void Bee::setScale(float scale) {
    
    targetScale = scale;
}

void Bee::drawShadow(ofImage & shadow) {
    
    
    
    ofSetColor(255, 40 * this->alpha);
    ofPushMatrix();
    ofTranslate(pos.x + 1, pos.y + 1);
    ofScale(scale,scale);
    ofRotateXDeg(rotationX);
    ofRotateYDeg(rotationY);
    shadow.draw(0.0,0.0, 0.0);
    ofPopMatrix();
    
}

void Bee::draw(ofImage & back, ofImage & mid, ofImage & front) {
    
    
    //ofLogNotice("scale") << scale;
    ofSetColor(255, 255 * this->alpha);

    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofScale(scale,scale);
    ofRotateXDeg(rotationX);
    ofRotateYDeg(rotationY);
    
    back.draw(0.0,0.0, 0);
    
    if((int)ofGetFrameNum() % 3 != 0)
        mid.draw(0.0,0.0, 1);
    
    front.draw(0.0,0.0, 2);
    ofPopMatrix();
    
}

void Bee::setAlpha(float alpha) {
    
    this->alpha = alpha;
    
}

