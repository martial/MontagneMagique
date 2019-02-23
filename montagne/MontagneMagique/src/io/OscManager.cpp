//
//  OscManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#include "OscManager.hpp"
#include "BigBangScene.hpp"
#include "BirdsScene.hpp"
#include "ofApp.h"

void OscManager::setup(int receiverPort, int senderPort, string senderIp) {
    
    receiver.setup(receiverPort);
    sender.setup(senderIp, senderPort);
    
}

void OscManager::update() {
    
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        

        
        if(m.getAddress() == "/em/lmm/accueilpublic") {
            
            this->montagneApp->mode = SCENE_MODE;
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 0;
            
        }
        
        if(m.getAddress() == "/em/lmm/introkalimba") {
            
            this->montagneApp->mode = SCENE_MODE;
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 1;
        }
        
        if(m.getAddress() == "/em/lmm/melodie") {
            
            this->montagneApp->mode = SCENE_MODE;
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 2;
        }
        
        if(m.getAddress() == "/em/lmm/finetoile") {
            
            this->montagneApp->mode = SCENE_MODE;
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 3;
        }
        
        if(m.getAddress() == "/em/lmm/oiseau") {
            
            ofSetCircleResolution(256);
            this->montagneApp->mode = TRACKING_MODE;
            
        }
        
        if(m.getAddress() == "/me/cosmogonie/bigbang") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            
            /*
            float repulsion = ofMap(m.getArgAsInt(0), 1, 8, 2, 4);
            bigBangScene->bigBang.startRepulsion(repulsion);
            bigBangScene = NULL;
             */
            
            bigBangScene->bigBang.addParticles(m.getArgAsInt(0) * 2);
            float repulsion = ofMap(m.getArgAsInt(0), 1, 8, 2, 3);
            bigBangScene->bigBang.startRepulsion(repulsion);
            
        }
        
        if(m.getAddress() == "/me/cosmogonie/kalimba") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.startRepulsion( m.getArgAsFloat(0));
            
        }
        
        
        
        if(m.getAddress() == "/me/printemps/oiseau/pitch") {
            
            BirdsScene * birds = (BirdsScene*) this->sceneManager->getSceneIndexForPath("birds");
            birds->setPitchPct(ofNormalize(m.getArgAsFloat(0), 0, 0.6));
           // ofLogNotice("Pïtch: ") << m.getArgAsFloat(0);
            
        } else {
            
            ofLogNotice("OSC: ") << m.getAddress();
        }
        

        ofApp* app = (ofApp*) ofGetAppPtr();
        app->addMessage( m.getAddress());
        
        
                
    }
    
}

void OscManager::keyPressed(int key) {
    
    if(key == 'a') {
        
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
        bigBangScene->bigBang.addParticles(4 * 2);
        bigBangScene->bigBang.startRepulsion(2);
        
    }
    
    if( key == OF_KEY_RIGHT) {
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
        bigBangScene->bigBang.mode ++;
        if(bigBangScene->bigBang.mode > 2)
            bigBangScene->bigBang.mode = 0;
        
    }
    
}


void OscManager::setSceneManager(ArSceneManager * manager) {
    
    this->sceneManager = manager;
    
}

void OscManager::setMontagneApp(MontagneApp * app) {
    
    this->montagneApp = app;
    
}


void OscManager::sendMessage(string adress, string label) {
    
    ofxOscMessage m;
    m.setAddress(adress);
    m.addStringArg(label);
    sender.sendMessage(m, false);
    
}

void OscManager::draw() {
    
    
}
