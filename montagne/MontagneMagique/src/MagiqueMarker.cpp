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
        
    NftTracker();
    
    //particles.setup();

}

void MagiqueMarker::setImage(ofImage * image) {
    
    this->image     = image;
    this->width     = image->getWidth() / 3.0;
    this->height    = image->getHeight() / 3.0;
    
}

void MagiqueMarker::updateTimes() {
    
    int currentTime     = ofGetElapsedTimeMillis();
    // if there is a change, reset timers.
    if(this->isFound() != bIsFound) {
        timeFound       = currentTime;
        timeLost        = currentTime;
        bIsFound        = this->isFound();
    }
    
    timeFoundElapsed    = currentTime - timeFound;
    timeLostElapsed     = currentTime - timeLost;
    
    // simple ms timer stuff to avoid quick losts of focus
    if(bIsFound) {
        
        if(timeFoundElapsed > timeFoundDelay) {
            bIsSolidFound = true;
        }
        
    } else {
        
        if(timeLostElapsed > timeLostDelay) {
            bIsSolidFound = false;
        }
        
    }
    
    
}

bool MagiqueMarker::getIsSolidFound() {
    
    return bIsSolidFound;
    
}



void MagiqueMarker::beginAR() {
    
    ARMarkerNFT * mk = &getSelectedMarker();
        
    ofRectangle r(0,0,viewportSize.x,viewportSize.y);
    ofPushView();
    ofViewport(r);
    loadProjectionMatrix();
    
    if(mk->valid){
        // store in memory
        for(int i=0; i<16; i++)
            currentPose[i] = mk->pose.T[i];
        
        bTracked = true;
        
    }
   
    if(bTracked){
        glLoadMatrixd(currentPose);
    }
    
}

void MagiqueMarker::endAR() {
    
    
    NftTracker::endAR();

}

void MagiqueMarker::drawParticles() {
    
    particles.update();
    particles.draw();
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
    
   
    
}



bool MagiqueMarker::hasBeenTracked() {
    
    return bTracked;
    
}
