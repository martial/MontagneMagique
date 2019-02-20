//
//  OscManager.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#include "OscManager.hpp"

void OscManager::setup() {
    
    receiver.setup(7002);
    sender.setup("169.254.134.129", 7003);
    
}

void OscManager::update() {
    
    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        ofLogNotice("OSC: ") << m.getAddress();
        
    }
    
    
}

void OscManager::sendMessage(string adress, string label) {
    
    ofxOscMessage m;
    m.setAddress(adress);
    m.addStringArg(label);
    sender.sendMessage(m, false);
    
}



void OscManager::draw() {
    
    
}
