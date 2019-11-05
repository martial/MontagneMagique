#ifndef BirdSprite_hpp
#define BirdSprite_hpp

#include "ofMain.h"

class BirdSprite {
  
public:
    
    void setup();
    void update();
    void draw(float x, float y, float scale);
    
    float posYCosAmplitude;
    
private:
    
    ofImage bodyImage;
    vector<ofImage> images, stillImages;
    vector<ofImage> * currentSet;
    float currFrame, vel;
    
    float   timeElapsed;
    int     delay;
    float   posY, blurredPosY;    
    bool    bWaitForFrame;
};

#endif /* BirdSprite_hpp */
