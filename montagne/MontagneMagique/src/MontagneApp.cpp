//
//  MontagneApp.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 14/12/2018.
//

#include "MontagneApp.hpp"
#include "ofxImgSizeUtils.h"

void MontagneApp::setup() {
    
    mode        = DRAW_MODE;
    pastMode    = mode;
    
    currentSceneName    = "";
    currentSubSceneName = "";
    
    hapPlayer.setup();
    
}

void MontagneApp::setupTrackers(int trackerInputWidth, int trackerInputHeight) {
    
    ofLogNotice("Setup trackers for ") << trackerInputWidth << " x " << trackerInputHeight;
    
    this->trackerInputWidth     = trackerInputWidth;
    this->trackerInputHeight    = trackerInputHeight;
    
    arToolKitManager.setup(trackerInputWidth, trackerInputHeight);
    arSceneManager.setup(arToolKitManager.trackers);
    arSceneManager.activateMarkersFromScene();
    
    
}

void MontagneApp::setupFbos() {
    
    debugFboLayer.allocate(trackerInputWidth, trackerInputHeight);
    fboLayer.allocate(videoOutputWidth, videoOutputHeight, GL_RGBA);
    fboLayer.getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        
    maskFbo = new ofFbo();
    maskFbo->allocate(videoOutputWidth, videoOutputHeight, GL_RGBA);
    
}


void MontagneApp::updateTrackers(ofBaseHasPixels & input) {
    
     if(mode == TRACKING_MODE) {
        
         arToolKitManager.update(input);

     }
    
}

void MontagneApp::updateScene() {
    
    
    hapPlayer.update();
    
    /*
    if(mode == HAP_MODE && hapPlayer.player.isLoaded() && hapPlayer.player.getLoopState() == OF_LOOP_NONE ) {
        
        if( hapPlayer.player.getPosition() == 1.0) {
            setMode(pastMode);

        }
        
    }
     
     */
    
}


void MontagneApp::drawScene() {
    
     // see if we have a mask around..
    /*
     bool bHasMask = false;
     if(mode == TRACKING_MODE) {
         
         for(int i=0; i< arToolKitManager.trackers.size(); i++) {
             
             if(arToolKitManager.trackers[i]->getIsSolidFound() ) {
                 
                 AbstractARScene * scene = arSceneManager.getSceneIndexForIndex(i, *arToolKitManager.trackers[i]);
                 
                 if ( scene && scene->bGenerateMask ) {
                     
                     maskFbo->begin();
                     ofClear(0, 0, 0, 0);
                     
                     ofSetColor(0);
                     ofDrawRectangle(0.0,0.0, videoOutputWidth, videoOutputHeight);
                     
                     arToolKitManager.trackers[i]->beginAR();
                     scene->generateMaskFbo();
                     arToolKitManager.trackers[i]->endAR();
                     maskFbo->end();
                     
                     bHasMask = true;
                     
                 }
             }
         }
     }
     
     */
    
    fboLayer.begin();
    ofEnableAlphaBlending();
    glEnable(GL_BLEND);
   // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_ONE, GL_SRC_COLOR, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    ofClear(0, 0);
    
    if(mode == SCENE_MODE) {
        
        arSceneManager.drawScene(0);
    
    }
    
    if(mode == DRAW_MODE) {
        
        arSceneManager.drawScene(1);
        
    }
    
    if(mode == BUBBLES_MODE) {
        
        arSceneManager.drawScene(4);
        
    }
    
    if(mode == TRACKING_MODE) {
        
        for(int i=0; i< arToolKitManager.trackers.size(); i++) {
            

            if(arToolKitManager.trackers[i]->getIsSolidFound() ) {
                
                arToolKitManager.trackers[i]->beginAR();
                arSceneManager.draw(i, *arToolKitManager.trackers[i]);
                arToolKitManager.trackers[i]->endAR();
                
            }
            
        }
    }
    
    // we need to flip it of tracking mode..
    ofPushMatrix();
    if(mode == TRACKING_MODE ) {
        ofScale( 1, -1, 1 );
        ofTranslate( 0, -videoOutputHeight, 0 );
    }
    hapPlayer.draw(videoOutputWidth, videoOutputHeight);
    ofPopMatrix();
    
    glDisable(GL_BLEND);
    fboLayer.end();
    
    ofRectangle rect = ofxImgSizeUtils::getCenteredRect(ofGetWidth(), (ofGetHeight() == 360 * 2.0f) ? ofGetHeight() * 0.5f : ofGetHeight(), videoOutputWidth, videoOutputHeight, false);
    
    ofSetColor(255,255);

    /*
    // if we have a mask.. draw it for now
    if(bHasMask && maskFbo && maskFbo->isAllocated()) {
     
       // maskFbo->getTexture().getTextureData().bFlipTexture = true;
        //fboLayer.getTexture().getTextureData().bFlipTexture = true;
        //tempFbo.getTexture().getTextureData().bFlipTexture = false;
     
        tempFbo.begin();
        ofClear(0, 0, 0, 255);

        fboLayer.draw(0.0, 0.0,  videoOutputWidth, videoOutputHeight);
        tempFbo.end();
     
    }
     
     */
    
    ofEnableAlphaBlending();
   // glEnable(GL_BLEND);
    //glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    if(mode != TRACKING_MODE ) {
        
        fboLayer.getTexture().getTextureData().bFlipTexture = false;
        fboLayer.draw(0.0, 0.0, rect.width, rect.height);
        
    } else {
        
        fboLayer.getTexture().getTextureData().bFlipTexture = true;
        fboLayer.draw(0.0, 0.0,  rect.width, rect.height);
        
       
        
    }
   // glDisable(GL_BLEND);

}

void MontagneApp::drawOffScreen() {
    
    if(mode == DRAW_MODE) {
        
        arSceneManager.drawOffScreen(1);
        
    }
    
}



void MontagneApp::processDebugDraw() {
    
    fboLayer.begin();
    ofClear(255);
    arToolKitManager.debugDraw();
    fboLayer.end();
    
}


void MontagneApp::debugDrawTrackers() {
    
    
    if(mode == SCENE_MODE) {
        fboLayer.getTexture().getTextureData().bFlipTexture = false;
        fboLayer.draw(0.0, 0.0, trackerInputWidth, trackerInputHeight);
        
    } else {
        fboLayer.getTexture().getTextureData().bFlipTexture = true;
        fboLayer.draw(0.0, 0.0, trackerInputWidth, trackerInputHeight);
    }
    
    
}

void MontagneApp::setMode(int mode) {
    
    
    if(this->mode != mode) {
        
        pastMode    = this->mode;
        this->mode  = mode;

    }
    
}

void MontagneApp::setPastMode() {
    setMode(pastMode);
}


int MontagneApp::getMode() {
    
    return this->mode;
}
