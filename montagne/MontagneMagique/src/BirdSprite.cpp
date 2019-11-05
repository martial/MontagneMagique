//
//  BirdSprite.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 23/09/2019.
//

#include "BirdSprite.hpp"

void BirdSprite::setup() {
    
    bWaitForFrame = false;
    
    // first load assets.
    // body
    bodyImage.load("assets/images/bird/body.png");
    
    // frames
    ofDirectory dir;
    dir.allowExt("png");
    dir.listDir("assets/images/bird/voloiseau_ANIM_all_separe 2");
    dir.sort();
    
    for(int i=0; i<dir.size(); i++) {
        
        ofImage img;
        img.load(dir.getFile(i).getAbsolutePath());
        images.push_back(img);
        
    }
    
    dir.allowExt("png");
    dir.listDir("assets/images/bird/voloiseau_ANIM_planage");
    dir.sort();
    
    for(int i=0; i<dir.size(); i++) {
        
        ofImage img;
        img.load(dir.getFile(i).getAbsolutePath());
        stillImages.push_back(img);
        
    }
    
    currentSet = &images;
    
    currFrame   = 0.0f;
    vel         =  1.0f;
    
    delay = 5000;
    timeElapsed = ofGetElapsedTimeMillis();
    
    posY = 0.0;
    blurredPosY = 0.0;
    
    posYCosAmplitude = 100;
}

void BirdSprite::update() {
    
    float diff = ofGetElapsedTimeMillis() - timeElapsed;
    
    // make it stop randomly
    if(diff > delay) {
        
        if(currentSet == &stillImages) {
            
            delay = ofRandom(5000, 6000);
            
            currentSet = &images;
            timeElapsed = ofGetElapsedTimeMillis();


        } else {
            
            bWaitForFrame = true;

        }
        
    }
    
    currFrame += vel;
    if(currFrame >= currentSet->size() )
        currFrame = 0;
    
    // a bit hacky but we wait for a certain frame before go flying
    if (bWaitForFrame && (floor(currFrame) == 5 || floor(currFrame) == 18)) {
        
        delay = ofRandom(5000, 6000);
        currentSet = &stillImages;
        bWaitForFrame = false;
        timeElapsed = ofGetElapsedTimeMillis();

    }
    
    float amplitude = 100.0f;
    if (vel == 0.0f)
        amplitude /= 2;
    
    posY = cos((float)ofGetElapsedTimeMillis() / amplitude * 0.25);
    
    // blur position
    float blurRate = 0.9;
    blurredPosY    = blurRate * blurredPosY +  (1.0f - blurRate) * posY;
    
}

void BirdSprite::draw(float x, float y, float scale) {
    
    update();
    
    float imgWidth  = bodyImage.getWidth() * scale;
    float imgHeight = bodyImage.getHeight() * scale;
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    ofTranslate(x,y);
    ofTranslate(0.0, blurredPosY * posYCosAmplitude);
    ofSetColor(255);
    ofScale(scale,scale);
    //bodyImage.draw(0.0, 0.0);
    
    int currFrameInt = floor(currFrame);
    currentSet->at(currFrameInt).draw(0.0, 0.0);
    
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CORNER);
    
}
