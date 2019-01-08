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
    void beginAR();
    void endAR();
    
    void drawParticles();

    
    bool hasBeenTracked();
    
private:
    
    ARdouble currentPose[16] ;
    bool bTracked;
    
    Particles particles;

};


#endif /* MagiqueMarker_hpp */
