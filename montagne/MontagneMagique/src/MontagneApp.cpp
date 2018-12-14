//
//  MontagneApp.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 14/12/2018.
//

#include "MontagneApp.hpp"

void MontagneApp::setup() {
    
    
}

void MontagneApp::setupTrackers(int trackerInputWidth, int trackerInputHeight) {
    
    this->trackerInputWidth     = trackerInputWidth;
    this->trackerInputHeight    = trackerInputHeight;
    
    arToolKitManager.setup(trackerInputWidth, trackerInputHeight);
    
}

void MontagneApp::setupFbos() {
    
    debugFboLayer.allocate(trackerInputWidth, trackerInputHeight);
    
}


void MontagneApp::updateTrackers(ofBaseHasPixels & input) {
    
    arToolKitManager.update(input);
    
}


void MontagneApp::processDebugDraw() {
    
    debugFboLayer.begin();
    ofClear(255);
    arToolKitManager.debugDraw();
    debugFboLayer.end();
}


void MontagneApp::debugDrawTrackers() {
    
    ofEnableAlphaBlending();
    debugFboLayer.draw(0.0, trackerInputHeight, trackerInputWidth, -trackerInputHeight);
    
}
