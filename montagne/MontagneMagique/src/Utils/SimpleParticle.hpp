//
//  SimpleParticle.hpp
//  MontagneMagique
//
//  Created by Zachary Lieberman !
//  Using it very often since 2009
//
//

#ifndef SimpleParticle_hpp
#define SimpleParticle_hpp

#include "ofMain.h"

class SimpleParticle
{
public:
    
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f frc;   // frc is also know as acceleration (newton says "f=ma")
    
    SimpleParticle();
    virtual ~SimpleParticle(){};
    
    void resetForce();
    void addForce(float x, float y);
    void addDampingForce();
    void setInitialCondition(float px, float py, float vx, float vy);
    void update();
    void draw();
    
    float damping;
    float rotation;
    float scale;
    
    ofImage * img;
    
protected:
private:
};


#endif /* SimpleParticle_hpp */
