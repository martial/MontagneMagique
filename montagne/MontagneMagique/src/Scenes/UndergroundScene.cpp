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
    
    for(int i = 0; i < 10; i ++) {
        
        SimpleParticle  * p = new SimpleParticle();
        p->setInitialCondition(ofRandom(400),ofRandom(400),0.0,0.0);
        p->scale = ofRandom(0.05, 0.2);
        p->img = &bug0;
        float xRand = ofRandom(-1, 1) / 1000;
        float yRand = ofRandom(-1, 1) / 1000;
        p->addForce(xRand, yRand);
        particles.push_back(p);
        
    }
    
}

void UndergroundScene::update() {
    
}

void UndergroundScene::draw(int markerIndex, int markerWidth, int markerHeight) {
    
    ofSetColor(255);
    
    for(int i = 0; i < 10; i ++) {
        
        if( ofGetFrameNum() % 50 == 0) {
            
            particles[i]->resetForce();
            // add random force
            float xRand = ofRandom(-1, 1) / 1000;
            float yRand = ofRandom(-1, 1) / 1000;
            particles[i]->addForce(xRand, yRand);

            
        }
        
        particles[i]->update();
        particles[i]->draw();
        
    }
    
    //ofSetColor(255,0, 0);
    //ofDrawRectangle(0.0, 0.0, markerWidth, markerHeight);
    
    //ofSetColor(255);
    //bug0.draw(0.0, 0.0);
    
}
