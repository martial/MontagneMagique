//
//  Eggs.cpp
//  2DSceneApp
//
//  Created by Martial Geoffre-Rouland on 16/01/2019.
//

#include "Eggs.hpp"

void Eggs::setup() {
    
    for(int i=0; i<3; i++) {
        
        ofImage img;
        images.push_back(img);
        images[i].load("OEUF_"+ofToString(i+1)+".png");
    }
    
    for(int i=0; i<10; i++) {
        
        int rdm = floor(ofRandom(images.size()));
        imageRef.push_back(&images[rdm]);
        
        ofxAnimatableFloat radius;
        radiuses.push_back(radius);
        radiuses[i].setup();
        radiuses[i].reset(0.0);
        radiuses[i].animateTo(0.5+ofRandom(0.5));
        radiuses[i].setCurve(EASE_OUT);
        radiuses[i].setDuration(5 + ofRandom(15));
        
        ofVec2f pos;
        positions.push_back(pos);
        positions[i].set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    }
    
}

void Eggs::update() {
    
    for(int i=0; i<radiuses.size(); i++) {
        radiuses[i].update(1.0 / 60.0f);
    }
    
}

void Eggs::draw() {
    
    for(int i=0; i<radiuses.size(); i++) {
        

        float pct = radiuses[i].getCurrentValue();
        float noise = ofMap(ofNoise(ofGetElapsedTimef() +i), 0.0, 1.0, 0.8, 1.0);
        pct *= noise;
        
        ofPushMatrix();
        ofTranslate(positions[i].x, positions[i].y);
        ofTranslate(imageRef[i]->getWidth() * .5, imageRef[i]->getHeight() * .5);
        ofScale(pct, pct);
        ofTranslate(-imageRef[i]->getWidth() * .5, -imageRef[i]->getHeight() * .5);

        imageRef[i]->draw(0.0, 0.0);
        ofPopMatrix();
        
    }
    
    
}
