//
//  MagiqueMarker.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 21/12/2018.
//

#ifndef MagiqueMarker_hpp
#define MagiqueMarker_hpp

#include <stdio.h>
#include "ofxARTNftTracker.h"
#include "Particles.hpp"

using namespace ofxArtool5;

class MagiqueMarker  : public NftTracker {
    
public:
    
    MagiqueMarker();
    
    void updateTimes();
    
    
    void beginAR();
    void endAR();
    
    bool hasBeenTracked();
    string markerid;
    
    // for testing
    void drawParticles();

    void setImage(ofImage * image);
    ofImage * image;
    float width, height;
    
    bool getIsSolidFound();
    int timeFoundElapsed, timeLostElapsed;
    int timeSolidFoundElapsed, timeSolidLostElapsed;

    bool bIsFound, bIsSolidFound;

    
private:
    
    ARdouble currentPose[16] ;
    bool bTracked;
    Particles particles;

    int timeFoundDelay, timeLostDelay;
    int timeFound, timeLost;
    int timeSolidFound, timeSolidLost;

};


#endif /* MagiqueMarker_hpp */
