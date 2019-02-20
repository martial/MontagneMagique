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
    
private:
    
    vector<SimpleParticle*> particles;
    
    float scl;
    float radius;
    
    ofxCvColorImage         colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvGrayscaleImage     grayBg;
    ofxCvGrayscaleImage     grayDiff;
    ofxCvContourFinder      contourFinder;
    
    int                 threshold;
    bool                bLearnBakground;
    
    ofImage             gradientPng;
    
    
};

#endif /* BigBang_hpp */
