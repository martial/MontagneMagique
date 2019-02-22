//
//  MagiqueMarker.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 21/12/2018.
//

#include "MagiqueMarker.hpp"

MagiqueMarker::MagiqueMarker() {
    
    image           = NULL;
    bTracked        = false;
    bIsFound        = false;
    bIsSolidFound   = false;
    timeFoundDelay  = 200;
    timeLostDelay   = 1000;
    timeFound       = ofGetElapsedTimeMillis();
    timeLost        = ofGetElapsedTimeMillis();
    
    blurRate        = 0.9;
    
    timeSolidFoundElapsed = 0;
    timeSolidLostElapsed  = 0;
    
    NftTracker();
    
    for(int i=0; i<16; i++)
        currentPose[i] = 0.0;

}

void MagiqueMarker::setImage(ofImage * image) {
    
    this->image     = image;
    this->width     = image->getWidth() / 9.0;
    this->height    = image->getHeight() / 9.0;
    
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

    // simple ms timer stuff to avoid quick losts of focus
    if(bIsFound) {
        
        timeFoundElapsed    = currentTime - timeFound;
        if(!bIsSolidFound && timeFoundElapsed > timeFoundDelay) {
            bIsSolidFound           = true;
            timeSolidFound          = currentTime;
            timeSolidLostElapsed    = 0;
        }
        
    } else {
        
        //timeFoundElapsed    = 0;
        timeLostElapsed     = currentTime - timeLost;
        if(bIsSolidFound && timeLostElapsed > timeLostDelay) {
            bIsSolidFound           = false;
            timeSolidLost           = currentTime;
            timeSolidFoundElapsed   = 0;
        }
        
    }
    
}

bool MagiqueMarker::getIsSolidFound() {
    
    return bIsSolidFound;
    
}

void MagiqueMarker::beginAR() {
    
    float scale = 12.0;
    this->width     = image->getWidth() / scale;
    this->height    = image->getHeight() / scale;
    
    
  
    
    ARMarkerNFT * mk = &getSelectedMarker();
        
    ofRectangle r(0,0,viewportSize.x,viewportSize.y);
    ofPushView();
    ofViewport(r);
    loadProjectionMatrix();
    
    if(mk->valid && this->isFound()){
        // store in memory and smooth
        for(int i=0; i<16; i++)
            currentPose[i] = blurRate * currentPose[i]   +  (1.0f - blurRate) * mk->pose.T[i];
        
        bTracked = true;
        
    }
    
   
    
    if(bTracked){
        glLoadMatrixd(currentPose);
    }
    
}

void MagiqueMarker::endAR() {
    
    NftTracker::endAR();

}

bool MagiqueMarker::hasBeenTracked() {
    
    return bTracked;
    
}
