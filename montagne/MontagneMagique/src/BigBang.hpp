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
    
    int thresold;
    ofColor colorBlobTarget;
    float colorBlobDistanceMax;

    
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
    
    int                 nConnectedMax, nConnectedGap;
    
    ofImage             particleImage;
    
    vector<ofColor>     blobsAverageColors;
    vector<int>         allowedBlobs;
    
};

#endif /* BigBang_hpp */
