//
//  BirdSprite.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 23/09/2019.
//

#ifndef BirdSprite_hpp
#define BirdSprite_hpp

#include "ofMain.h"

class BirdSprite {
  
public:
    
    void setup();
    void update();
    void draw();
    
private:
    
    ofImage bodyImage;
    vector<ofImage> images;
    
    float currFrame, vel;
    
    float timeElapsed;
    int delay;
    
    float posY, blurredPosY;
};

#endif /* BirdSprite_hpp */
