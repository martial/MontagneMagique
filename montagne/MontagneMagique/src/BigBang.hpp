//
//  BigBang.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 18/02/2019.
//

#ifndef BigBang_hpp
#define BigBang_hpp

#include "ofMain.h"
#include "SimpleParticle.hpp"
#include "ofxOpenCv.h"

class BigBang {
    
public:
    
    void setup(string dataPath = "");
    void update();
    void draw();
    
    void addParticles(int nParticles, float minSize, float maxSize, float minDamp, float maxDamp);
    void startRepulsion(float scale);
    
    int  mode;

    
private:
    
    float scale;
    
    vector<SimpleParticle*> particles;
    int particlesCreated;
    
    float scl;
    float radius;
    
    float repulsionScale;
    
    ofxCvColorImage         colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvContourFinder      contourFinder;
    
    int                 threshold;
    bool                bLearnBakground;
    
    float               lineDistance;
    float               blobDistance;
    float               forceRandomNessScale;
    float               maxParticleLife;
    float               fadeInOutPct;
    
    int                 lastMode;
    
    
};

#endif /* BigBang_hpp */