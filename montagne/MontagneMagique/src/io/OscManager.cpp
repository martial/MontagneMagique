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
    
    ofApp * app = (ofApp*) ofGetAppPtr();

    
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        string adress           = m.getAddress();
        vector<string> splitted = ofSplitString(adress, "/", true);
        string first            = splitted[0];

        // for video player
        if(first == "video") {
            
             if(splitted.size() == 2) {
                 
                 if(splitted[1] == "stop") {
                     
                     this->montagneApp->setPastMode();
                     
                 } else {
                 
                    this->montagneApp->setMode(HAP_MODE);
                    app->app.hapPlayer.setVideo(splitted[1]);
                     
                 }
                 
             } else if(splitted.size() == 3) {
                 
                 if(splitted[2] == "loop") {
                     
                     this->montagneApp->setMode(HAP_MODE);
                     app->app.hapPlayer.setVideo(splitted[1], true);
                     
                 }
                
                 
             }
        }
        
        
        // for scene
        if(first == "scene") {
            
            
            if(splitted.size() > 1) {
                
                app->app.currentSceneName = splitted[1];
                
                 if(splitted.size() > 2) {
                     
                     app->app.currentSubSceneName = splitted[2];

                 } else {
                     
                     app->app.currentSubSceneName = "";
                 }
                
            } else {
                
                app->app.currentSceneName       = "";
                app->app.currentSubSceneName    = "";
                
            }
            
            // we check for scene-locks here
            this->sceneManager->activateMarkersFromScene();
            
        }
        
        if(m.getAddress() == "/event/stoploops") {
            
            this->sceneManager->setGenericSceneLoopMode(OF_LOOP_NONE);
            
        }
        
         if(m.getAddress() == "/scene/blackout") {
             
         }
        
       
        if(m.getAddress() == "/scene/cosmogonie/accueilpublic") {
            
            this->montagneApp->setMode(SCENE_MODE);
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 0;
            
        }
        
        if(m.getAddress() == "/scene/cosmogonie/introkalimba") {
            
            this->montagneApp->setMode(SCENE_MODE);
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 1;
        }
        
        if(m.getAddress() == "/scene/cosmogonie/melodiekalimba") {
            
            this->montagneApp->setMode(SCENE_MODE);
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 2;
        }
        
        if(m.getAddress() == "/scene/printemps/nuit") {
            
            this->montagneApp->setMode(TRACKING_MODE);

            
        }
        
        if(m.getAddress() == "/scene/printemps/soleil") {
            
            this->montagneApp->setMode(TRACKING_MODE);
            
        }
        
        if(m.getAddress() == "/scene/printemps/ours") {
            
            this->montagneApp->setMode(TRACKING_MODE);

        }
        
        if(m.getAddress() == "/scene/printemps/finabeilles") {
            
            this->montagneApp->setMode(TRACKING_MODE);

        }
        

        if(m.getAddress() == "/event/cosmogonie/particules") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            
            int nParticles      = m.getArgAsInt(0);
            float minSize       = m.getArgAsFloat(1);
            float maxSize       = m.getArgAsFloat(2);
            float minFriction   = m.getArgAsFloat(3);
            float maxFriction   = m.getArgAsFloat(4);
            
            bigBangScene->bigBang.addParticles(m.getArgAsInt(0), minSize, maxSize, minFriction, maxFriction);

            
        }
       
        
        if(m.getAddress() == "/event/cosmogonie/kalimba") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.startRepulsion( m.getArgAsFloat(0));
            
        }
        
        
        
        if(m.getAddress() == "/event/printemps/oiseau/pitch") {
            
            BirdsScene * birds = (BirdsScene*) this->sceneManager->getSceneIndexForPath("OISEAU_BULLES");
            birds->setPitchPct(ofNormalize(m.getArgAsFloat(0), 0, 0.6));
           // ofLogNotice("Pïtch: ") << m.getArgAsFloat(0);
            
        } else {
            
            ofLogNotice("OSC: ") << m.getAddress();
        }
        

        app->addMessage( m.getAddress());
        
        
                
    }
    
    app = NULL;
    
}

void OscManager::setSceneEvent(ofxOscMessage & m) {
    
    
}

void OscManager::keyPressed(int key) {
    
    if(key == 'a') {
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
        bigBangScene->bigBang.addParticles(1, 2, 6, 0.1, 0.2);
        //bigBangScene->bigBang.startRepulsion(2);
        
    }
    
    if(key == 'r') {
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
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

void OscManager::sendMessage(string adress, int value) {
    
    ofxOscMessage m;
    m.setAddress(adress);
    m.addIntArg(value);
    sender.sendMessage(m, false);
    
}


