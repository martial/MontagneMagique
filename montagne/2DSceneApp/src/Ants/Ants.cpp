//
//  Ants.cpp
//  2DSceneApp
//
//  Created by Martial Geoffre-Rouland on 16/01/2019.
//

#include "Ants.hpp"

void Ants::setup(string dataPath) {
   
    for(int i = 0; i < 10; i ++) {
        
        SimpleParticle  * p = new SimpleParticle();
        
        p->setInitialCondition(ofRandom(-200, 200),ofRandom(-200, 200),0.0,0.0);
        p->scale = ofRandom(0.05, 0.1);
        
        float xRand = ofRandom(-1, 1) / 1000;
        float yRand = ofRandom(-1, 1) / 1000;
        p->addForce(xRand, yRand);
        particles.push_back(p);
        
        ImageSequencePlayer  imgPlayer;
        images.push_back(imgPlayer);

        images[i].loadFolder(dataPath + "/fourmis_1_sprites");
        images[i].setFps(31);
        images[i].setLoop(true);
        images[i].play();
        
        //ofLogNotice("i") << i;
    }
    
}

void Ants::update() {
    
    
    
}

void Ants::draw() {
    
    ofEnableAlphaBlending();
   // ofSetColor(255);
    
    for(int i = 0; i < particles.size(); i ++) {
        
        float mult = .1;
        ofVec2f currentForce = particles[i]->frc;
        particles[i]->resetForce();
        
        
        if( ofGetFrameNum() %1 == 0) {
            particles[i]->addNonBackingForce(mult);
        } else {
            particles[i]->addForce(currentForce.x, currentForce.y);
        }
        
        //particles[i]->addForce(currentForce.x, currentForce.y);
        
        //particles[i]->addDampingForce();
        particles[i]->update();
       // particles[i]->bounceOffWalls();
        
        images[i].update();
        
        particles[i]->begin(images[i].width / 2, images[i].height / 2);
        //particles[i]->draw();
        //ofSetRectMode(OF_RECTMODE_CENTER);
        //ofDrawRectangle(0.0, 0.0, 100, 100);
        images[i].draw(0.0, 0.0);
        
        
        particles[i]->end();
    }
}
