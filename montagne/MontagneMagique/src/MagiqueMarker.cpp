//
//  MagiqueMarker.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 21/12/2018.
//

#include "MagiqueMarker.hpp"
#include "ofApp.h"

MagiqueMarker::MagiqueMarker() {
    
    image           = NULL;
    bTracked        = false;
    bIsFound        = false;
    bIsSolidFound   = false;
    bIsActive       = false;
    bIsAlwaysActive = false;
    
    timeFoundDelay  = 200;
    timeLostDelay   = 50;
    timeFound       = ofGetElapsedTimeMillis();
    timeLost        = ofGetElapsedTimeMillis();
        
    timeSolidFoundElapsed = 0;
    timeSolidLostElapsed  = 0;
    
    NftTracker();
    
    for(int i=0; i<16; i++) {
        
        currentPose[i] = 0.0;
        
        SimpleKalmanFilter kl;
        kl.setup();
        filters.push_back(kl);
        
    }
    
    
    // TODO move this in another place ofApp or config 
    // get viewport size
    ofApp * app = (ofApp*)ofGetAppPtr();
    viewportSize.x = app->app.videoOutputWidth;
    viewportSize.y = app->app.videoOutputHeight;

}

void MagiqueMarker::setImage(ofImage * image) {
    
    this->image     = image;
    this->width     = image->getWidth() / 9.0;
    this->height    = image->getHeight() / 9.0;
    
}

void MagiqueMarker::updateBlank() {
    
    ofImage img;
    img.allocate(camSize.x,camSize.y, OF_IMAGE_COLOR);
    for(int i=0; i<camSize.x*camSize.y; i++ )
        img.setColor(i, ofColor(0));
    
    this->update(img.getPixels().getData());
    int dummy = 0;
    ofNotifyEvent(evLostMarker, dummy);
    this->bIsSolidFound = false;
    
}

void MagiqueMarker::updateTimes() {
    
    int currentTime     = ofGetElapsedTimeMillis();
    
    // if there is a change, reset timers.
    if(this->isFound() != bIsFound) {
        timeFound       = currentTime;
        timeLost        = currentTime;
        bIsFound        = this->isFound();
    }
    
    
    if(bIsSolidFound)
        timeSolidFoundElapsed = currentTime - timeSolidFound;
    else
        timeSolidLostElapsed  = currentTime - timeSolidLost;
    
    if(timeSolidLostElapsed < 0 )
        timeSolidLostElapsed = 0;

    // simple ms timer stuff to avoid quick losts of focus
    if(bIsFound) {
        
        timeFoundElapsed    = currentTime - timeFound;
        if(!bIsSolidFound && timeFoundElapsed > timeFoundDelay) {
            
            bIsSolidFound           = true;
            timeSolidFound          = currentTime;
            timeSolidLostElapsed    = 0;
            
            ofNotifyEvent(solidFoundEvent, this->markerid, this );
            
        }
        
    } else {
        
        //timeFoundElapsed    = 0;
        timeLostElapsed     = currentTime - timeLost;
        
        if(bIsSolidFound && timeLostElapsed > timeLostDelay) {
            
            bIsSolidFound           = false;
            timeSolidLost           = currentTime;
            timeSolidFoundElapsed   = 0;
            
            ofNotifyEvent(solidLostEvent, this->markerid, this );

        }
        
    }
    
}

bool MagiqueMarker::getIsSolidFound() {
    
    return bIsSolidFound;
    
}

void MagiqueMarker::beginAR() {
    
    float scale     = 12.0;
    this->width     = image->getWidth() / scale;
    this->height    = image->getHeight() / scale;
    
    ARMarkerNFT * mk = &getSelectedMarker();
    
    ofRectangle r (0,0, 1920, 1080);
    ofPushView();
    ofViewport(r);
    loadProjectionMatrix();
    
    if(mk->valid && this->isFound()){
        // store in memory and smooth
        for(int i=0; i<16; i++) {
            
            currentPose[i] = filters[i].predict_and_correct( mk->pose.T[i]);
            //currentPose[i] = mk->pose.T[i];
            
        }
        
        bTracked = true;
        
    }
    
    if(bTracked){
        glLoadMatrixd(currentPose);
    }
    
}

void MagiqueMarker::beginHardAR() {
    
    float scale = 12.0;
    this->width     = image->getWidth() / scale;
    this->height    = image->getHeight() / scale;
    
    ARMarkerNFT * mk = &getSelectedMarker();
    
    ofRectangle r(0,0,viewportSize.x,viewportSize.y);
    ofPushView();
    ofViewport(r);
    loadProjectionMatrix();
    
    if(mk->valid && this->isFound()){
        for(int i=0; i<16; i++)
            currentPose[i] = mk->pose.T[i];
        
        bTracked = true;
        
    }
        
    if(bTracked){
        glLoadMatrixd(currentPose);
    }
    
}



void MagiqueMarker::endAR() {
    
     ofPopView();
    
}

bool MagiqueMarker::hasBeenTracked() {
    
    return bTracked;
    
}
