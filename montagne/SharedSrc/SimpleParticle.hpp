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
    void addNonBackingForce(float mult);
    
    ofVec2f getRandomForceLessThan(float val, float mult);

    void addDampingForce(float scale = 1.0);
    void setInitialCondition(float px, float py, float vx, float vy);
    void bounceOffWalls();
    void update();
    void draw();
    
    void addRepulsionForce(ofVec2f posOfForce, float radius, float scale);
    void addAttractionForce(ofVec2f posOfForce, float radius, float scale);
    void addRepulsionForce(SimpleParticle &p, float radius, float scale);
    void addAttractionForce(SimpleParticle &p, float radius, float scale);
    bool isPointInRadius(ofPoint p, float radius);

    
    void begin(float xOffSet = 0, float yOffSet = 0);
    void end();

    float damping;
    float rotation, smoothedRotation;
    float scale;
    
    float life, lifeDelay;
    bool bIsAlive;
    
    ofImage * img;
    
protected:
private:
};


#endif /* SimpleParticle_hpp */
