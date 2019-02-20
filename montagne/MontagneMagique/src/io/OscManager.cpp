//
//  OscManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#include "OscManager.hpp"
#include "BigBangScene.hpp"
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
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 0;
        }
        
        if(m.getAddress() == "/em/lmm/introkalimba") {
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 1;
        }
        
        if(m.getAddress() == "/em/lmm/melodie") {
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 2;
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
        
        ofLogNotice("OSC: ") << m.getAddress();
        
    }
    
}

void OscManager::setSceneManager(ArSceneManager * manager) {
    
    this->sceneManager = manager;
    
}

void OscManager::sendMessage(string adress, string label) {
    
    ofxOscMessage m;
    m.setAddress(adress);
    m.addStringArg(label);
    sender.sendMessage(m, false);
    
}

void OscManager::draw() {
    
    
}
