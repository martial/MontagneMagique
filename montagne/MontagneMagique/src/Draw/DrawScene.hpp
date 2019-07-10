//
//  GenericScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 10/04/2019.
//

#ifndef Drawscene_hpp
#define Drawscene_hpp

#include <stdio.h>
#include "AbstractARScene.hpp"
#include "ofxOpenCv.h"

class VectorObject {
  
public:
    
    ofPath      path;
    ofPolyline  line;
    ofImage     img;
    float       scalez;
    float       scaleVel;
    
    float       rot;
    float       rotationVel;
    VectorObject() {
        scalez = ofRandom(-2000, -1000);
        scaleVel = ofRandom(50, 100);
        
        rot = 0;
        rotationVel = ofRandom(10);
    }
    
    void updateScale() {
        
        rot += rotationVel;

        
        scalez += scaleVel;
        if(scalez > 1000)
             scalez = ofRandom(-2000, -1000);
    }
    
    void draw() {
        
        ofPath p;
        p.moveTo(line.getVertices()[0]);
        for(int j=0; j<line.getVertices().size(); j++) {
            
            float randx = ofRandom(-1, 1);
            float randy = ofRandom(-1, 1);
            
            ofVec2f pnt = line.getVertices()[j];
            pnt.x += randx;
            pnt.y += randy;
            
            p.curveTo(pnt);
        }
        p.close();
        
        p.setColor(path.getFillColor());
        p.setStrokeColor(path.getStrokeColor());
        p.setStrokeWidth(0);
        
        //p.draw();
        
        img.draw(line.getBoundingBox().x,line.getBoundingBox().y);
        
    }
    
    
};

class DrawScene : public AbstractARScene {
    
public:
    
    DrawScene() {};
    
    void setup(string name);
    void update();
    void draw();
    void updateCamera(ofBaseHasPixels & input);
    void onMarkerTracked();
    
    void captureImages();
    void captureShapes();

    
private:
    
    ofxCvColorImage         colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvContourFinder      contourFinder;
    
    float thresold;
    
    ofShader shader;
    
    vector<ofColor>             colors;
    vector<ofPolyline>          lines;
    vector<ofPath>              paths;
    vector<VectorObject>        vectorObjects;

};


#endif /* GenericScene_hpp */
