//
//  MontagneApp.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 14/12/2018.
//

#include "MontagneApp.hpp"

void MontagneApp::setup() {
    
    arSceneManager.setup();
    
}

void MontagneApp::setupTrackers(int trackerInputWidth, int trackerInputHeight) {
    
    this->trackerInputWidth     = trackerInputWidth;
    this->trackerInputHeight    = trackerInputHeight;
    
    arToolKitManager.setup(trackerInputWidth, trackerInputHeight);
    
}

void MontagneApp::setupFbos() {
    
    debugFboLayer.allocate(trackerInputWidth, trackerInputHeight);
    fboLayer.allocate(trackerInputWidth, trackerInputHeight);
    
}


void MontagneApp::updateTrackers(ofBaseHasPixels & input) {
    
    arToolKitManager.update(input);
    
}

void MontagneApp::updateScene() {
    
    arSceneManager.update();

    
}


void MontagneApp::drawScene() {
    
    fboLayer.begin();
    
    ofClear(255);
    
    
    for(int i=0; i< arToolKitManager.trackers.size(); i++) {
        
        //if(trackers[i]->isFound()){
        
        float w = arToolKitManager.images[i].getWidth() / 3.0;
        float h = arToolKitManager.images[i].getHeight() / 3.0;
        
        arToolKitManager.trackers[i]->beginAR();
        arSceneManager.draw(i, w, h);
        arToolKitManager.trackers[i]->endAR();
        
    }
    
    fboLayer.end();
    
    fboLayer.draw(0.0, trackerInputHeight, trackerInputWidth, -trackerInputHeight);

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
