//
//  MagiqueMarker.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 21/12/2018.
//

#include "MagiqueMarker.hpp"

MagiqueMarker::MagiqueMarker() {
    
    bTracked    = false;
    NftTracker();
    
    particles.setup();

}


void MagiqueMarker::beginAR() {
    
    ARMarkerNFT *mk = &getSelectedMarker();
        
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
