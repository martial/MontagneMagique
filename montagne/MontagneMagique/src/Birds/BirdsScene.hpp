//
//  BirdsScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 21/02/2019.
//

#ifndef BirdsScene_hpp
#define BirdsScene_hpp

#include "AbstractARScene.hpp"
#include "SimpleParticle.hpp"

class BirdsScene : public AbstractARScene {
    
public:
    
    BirdsScene() {};
    void setup(string name);
    void update();
    void draw();
    
    void setPitchPct(float pct);
    void onPitchStart();
    void onPitchEnd();
    
private:
    
    void drawCircle(ofVec2f pos, float radius, ofColor col);
    void drawCircleNoise(ofVec2f pos, float radius, ofColor col);
    
    ofImage backgroundImage;
    
    bool  bIsSinging;
    float pitchTime, pitchMillis;
    float pitchPct, targetPitchPct;
    
    vector<SimpleParticle* > particles;
    SimpleParticle *    currentParticle;
    
    ofColor startColor, endColor;
    vector<ofColor> colors;
    float currentRadius, radiusVel, radiusVelFactor;
    
    float globalScale;
};

#endif /* BirdsScene_hpp */
