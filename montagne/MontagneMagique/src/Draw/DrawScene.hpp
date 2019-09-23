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
#include "ofxPostProcessing.h"
#include "BirdSprite.hpp"

#define SHAPE_TYPE_VECTOR 0
#define SHAPE_TYPE_IMG 1
#define SHAPE_TYPE_BOTH 2

class VectorObject {
  
public:
    
    ofPath      path;
    ofPolyline  line;
    ofImage     img;
    float       scalez;
    float       scaleVel;
    
    float       rot;
    float       rotationVel;
    
    int         type;
    
    VectorObject() {
        
        type = SHAPE_TYPE_VECTOR;
        
        scalez = ofRandom(-10000, -10000);
        scaleVel = ofRandom(50, 50);
        
        rot = 0;
        rotationVel = ofRandom(10);
    }
    
    void updateScale() {
        
        rot += rotationVel;
        scalez += scaleVel;
        if(scalez > 1000)
            scalez = ofRandom(-5000, -4000);
    }
    
    void draw() {
        
        if(type == SHAPE_TYPE_VECTOR)  {
        
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
            
            p.draw();
            
        }
        
        if(type == SHAPE_TYPE_IMG)  {

            img.draw(line.getBoundingBox().x,line.getBoundingBox().y);
            
        }
        
    }
    
    
};

class DrawScene : public AbstractARScene {
    
public:
    
    DrawScene() {};
    
    void setup(string name);
    void setMarker(shared_ptr<MagiqueMarker> marker);
    void update();
    void draw();
    void drawOffScreen();

    void updateCamera(ofBaseHasPixels & input);
    void onMarkerTracked();
    
    void captureImages();
    void captureShapes(int mode = SHAPE_TYPE_VECTOR);
    void clear();
    void undo();
    void save();
    
    void setTreshold(float thresold);
    void setAperture(float aperture);
    void loadFolder(string folder); // todo
    
private:
    
    ofxCvColorImage         colorImg;
    ofxCvGrayscaleImage     grayImage;
    ofxCvContourFinder      contourFinder;
        
    ofShader shader;
    
    vector<ofColor>             colors;
    vector<ofPolyline>          lines;
    vector<ofPath>              paths;
    vector<VectorObject>        vectorObjects;
    
    // scene stuff
    ofxPostProcessing post;
    ofEasyCam cam;
    ofLight light;
    
    shared_ptr<DofPass> dofPass;
    
    BirdSprite bird;


};


#endif /* GenericScene_hpp */
