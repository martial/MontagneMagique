//
//  BirdSprite.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 23/09/2019.
//

#include "BirdSprite.hpp"

void BirdSprite::setup() {
    
    // first load assets.
    
    // body
    bodyImage.load("assets/images/bird/body.png");
    
    // frames
    ofDirectory dir;
    dir.allowExt("png");
    dir.listDir("assets/images/bird/frames");
    
    for(int i=0; i<dir.size(); i++) {
        
        ofImage img;
        img.load(dir.getFile(i).getAbsolutePath());
        images.push_back(img);
        
    }
    
    currFrame   = 0.0f;
    vel         = .2f;
    
    delay = 5000;
    timeElapsed = ofGetElapsedTimeMillis();
    
    posY = 0.0;
    blurredPosY = 0.0;
}

void BirdSprite::update() {
    
    float diff = ofGetElapsedTimeMillis() - timeElapsed;
    
    // make it stop randomly
    if(diff > delay) {
        
        if(vel == 0.0) {
            
            vel = .3;
            delay = ofRandom(5000, 6000);

        } else {
            
            vel = 0.0;
            delay = ofRandom(600, 1000);
        }
        
        timeElapsed = ofGetElapsedTimeMillis();

    }
    
    currFrame += vel;
    if(currFrame >= images.size() )
        currFrame = 0;
    
    float amplitude = 800.0f;
    if (vel == 0.0f)
        amplitude /= 2;
    
    posY = cos((float)ofGetElapsedTimeMillis() / amplitude);
    
    // blur position
    float blurRate = 0.9;
    blurredPosY    = blurRate * blurredPosY   +  (1.0f - blurRate) * posY;

    
}

void BirdSprite::draw() {
    
    update();
    
    ofPushMatrix();
    ofTranslate(0.0, blurredPosY * 100);
    ofSetColor(255);
    ofScale(0.6,0.6);
    bodyImage.draw(0.0, 0.0);
    
    int currFrameInt = floor(currFrame);
    images[currFrameInt].draw(0.0, 0.0);
    
    ofPopMatrix();
    
}
