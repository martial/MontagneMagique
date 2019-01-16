//
//  Eggs.hpp
//  2DSceneApp
//
//  Created by Martial Geoffre-Rouland on 16/01/2019.
//

#ifndef Eggs_hpp
#define Eggs_hpp

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

class Eggs {
    
public:
    
    void setup();
    void update();
    void draw();
    
private:
    

    vector<ofImage>     images;
    vector<ofImage*>    imageRef;
    vector<ofxAnimatableFloat> radiuses;
    vector<ofVec2f> positions;

    
    
};

#endif /* Eggs_hpp */