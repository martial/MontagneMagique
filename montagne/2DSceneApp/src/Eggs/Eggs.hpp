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
    
    void setup(int width, int height, string dataPath = "");
    void update();
    void draw();
    
private:
    

    vector<ofImage>     images;
    vector<ofImage*>    imageRef;
    vector<ofxAnimatableFloat> radiuses;
    vector<ofVec2f> positions;
    
    vector<ofVec2f> allowedPositions;
    
    
};

#endif /* Eggs_hpp */
