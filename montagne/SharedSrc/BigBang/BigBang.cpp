//
//  BigBang.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 18/02/2019.
//

#include "BigBang.hpp"
#include "ofApp.h"

void BigBang::setup(string dataPath) {
    
    radius = 10.0;
    threshold = 80;
    
    gradientPng.load(dataPath + "/Disney-Logo.png");
}


void BigBang::update() {
    
    ofApp * app = (ofApp*) ofGetAppPtr();

    
    if(!colorImg.bAllocated) {
    
        colorImg.allocate(app->videoInputWidth,app->videoInputHeight);
        grayImage.allocate(app->videoInputWidth,app->videoInputHeight);
        grayBg.allocate(app->videoInputWidth,app->videoInputHeight);
        grayDiff.allocate(app->videoInputWidth,app->videoInputHeight);
        
    }
    
    
    colorImg.setFromPixels(app->trackedVideoInput->getPixels());
    grayImage = colorImg;
    grayImage.threshold(100);
    contourFinder.findContours(grayImage, 20, (app->videoInputWidth*app->videoInputHeight), 10, false);    // find holes
    
   // scl += 0.0001;
    
    
    if(ofGetFrameNum() % 40 == 0) {
        
        SimpleParticle  * p = new SimpleParticle();
        p->setInitialCondition(ofRandom(-ofGetWidth(), ofGetWidth()),ofRandom(-ofGetHeight(), ofGetHeight()),0.0,0.0);
        p->scale = ofRandom(app->bigBangScaleMin, app->bigBangScaleMax);
        p->damping = ofRandom(app->bigBangDampingMin, app->bigBangDampingMax);
        particles.push_back(p);
        
   
    }
    
}

void BigBang::draw() {
    
 

    
    for(int i = 0; i < particles.size(); i ++) {
        
        
        particles[i]->resetForce();
        
        particles[i]->addForce(ofRandom(-2,2), ofRandom(-1.5,1.5));
        
        for (int j = 0; j < contourFinder.nBlobs; j++){
            
            particles[i]->addAttractionForce(ofVec2f(contourFinder.blobs[j].boundingRect.getCenter().x,
                                                    contourFinder.blobs[j].boundingRect.getCenter().y), ofGetWidth() * 5, 0.8+ scl);
            
           
        }
        
        
       // particles[i]->addAttractionForce(ofVec2f(ofGetWidth() * .5, ofGetHeight() * .5), ofGetWidth() * 5, 1.0+ scl);
        
    //  particles[i]->addAttractionForce(ofVec2f(ofGetMouseX(), ofGetMouseY()), ofGetWidth() * 5, 1.0);
        particles[i]->addDampingForce();
        particles[i]->update();
        
        if(particles[i]->bIsAlive && particles[i]->isPointInRadius(ofVec2f(ofGetWidth() * .5, ofGetHeight() * .5), radius)) {
            radius += 1;
            particles[i]->bIsAlive = false;
            
        }

        
         particles[i]->begin();
         ofDrawEllipse(0.0, 0.0, particles[i]->scale, particles[i]->scale);
         particles[i]->end();
        // particles[i]->bounceOffWalls();
        
      
    }
    
    ofFill();
    ofSetHexColor(0x333333);
    ofSetHexColor(0xffffff);
    
    // we could draw the whole contour finder
    //contourFinder.draw(360,540);
    
    // or, instead we can draw each blob individually from the blobs vector,
    // this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
        
        
    }
    
    /*
    ofBeginShape();
    
    for (int i=0; i<360/4; i++) {
       
        float r = radius;
        float noise = ofNoise((ofGetElapsedTimef()), i);
        float x = ofGetWidth()/2  + (r * cos(ofDegToRad(i*4))) + (noise * 20 );
        float y = ofGetHeight()/2 + (r * sin(ofDegToRad(i*4))) + (noise * 20 );
        ofVertex(x ,y );
    } // for
    
    ofEndShape();
     
     */
    
    
}
