//
//  MontagneApp.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 14/12/2018.
//

#include "MontagneApp.hpp"

void MontagneApp::setup() {
    
    mode = SCENE_MODE;
    
}

void MontagneApp::setupTrackers(int trackerInputWidth, int trackerInputHeight) {
    
    this->trackerInputWidth     = trackerInputWidth;
    this->trackerInputHeight    = trackerInputHeight;
    
    arToolKitManager.setup(trackerInputWidth, trackerInputHeight);
    arSceneManager.setup(arToolKitManager.trackers);

    
}

void MontagneApp::setupFbos() {
    
    debugFboLayer.allocate(trackerInputWidth, trackerInputHeight);
    fboLayer.allocate(trackerInputWidth, trackerInputHeight, GL_RGBA);
    fboLayer.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
}


void MontagneApp::updateTrackers(ofBaseHasPixels & input) {
    
     if(mode == TRACKING_MODE) {
         arToolKitManager.update(input);
     }
    
}

void MontagneApp::updateScene() {
    
    arSceneManager.update();

    
}


void MontagneApp::drawScene() {
    
   
    
    fboLayer.begin();
    //ofEnableAlphaBlending();
    ofClear(0,0,0,0);
    glEnable(GL_DEPTH);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    
    if(mode == SCENE_MODE) {
        
       
        arSceneManager.drawScene(0);
    
    }
    if(mode == TRACKING_MODE) {
        
        for(int i=0; i< arToolKitManager.trackers.size(); i++) {
            
            // TODO remove that for scenes 
          //  if(arToolKitManager.trackers[i]->isFound()){
            
            
                string markerID = arToolKitManager.trackers[i]->markerid;
                
                arToolKitManager.trackers[i]->beginAR();
                arSceneManager.draw(i, *arToolKitManager.trackers[i]);
                arToolKitManager.trackers[i]->endAR();
                
           //@ }
            
        }
    }
    glDisable(GL_BLEND);
    fboLayer.end();
    
    if(mode == SCENE_MODE) {
        fboLayer.getTexture().getTextureData().bFlipTexture = false;
        fboLayer.draw(0.0, 0.0, trackerInputWidth, trackerInputHeight);
        
    } else {
        fboLayer.getTexture().getTextureData().bFlipTexture = true;
        fboLayer.draw(0.0, trackerInputHeight, trackerInputWidth, -trackerInputHeight);
        
    }


}


void MontagneApp::processDebugDraw() {
    
    fboLayer.begin();
    ofClear(255);
    arToolKitManager.debugDraw();
    fboLayer.end();
}


void MontagneApp::debugDrawTrackers() {
    
    ofEnableAlphaBlending();
    fboLayer.draw(0.0, trackerInputHeight, trackerInputWidth, -trackerInputHeight);
    
}
