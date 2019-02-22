//
//  Bee.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 22/02/2019.
//

#ifndef Bee_hpp
#define Bee_hpp

#include "ofMain.h"

class Bee {
  
public:
    
    
    void setInitialStatus(ofVec2f pos, float scale);
    void update();
    void setPosition(ofVec2f pos);
    void setScale(float scale);
    
    void setAlpha(float alpha);
    void drawShadow(ofImage & shadow);
    void draw(ofImage & back, ofImage & mid, ofImage & front);
    
    
    ofVec2f pos, targetPos;
    float scale, targetScale;
    float rotationX, rotationY;
    
    bool operator < (const Bee &other) const {
        return scale < other.scale;
    }

    
private:
    
    float alpha;
   
    
};

#endif /* Bee_hpp */
