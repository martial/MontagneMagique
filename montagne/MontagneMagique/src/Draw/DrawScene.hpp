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
    
    float       opacity;
    float       opacityVel;

    int         type;
    
    VectorObject() {
        
        type        = SHAPE_TYPE_VECTOR;
        scalez      = ofRandom(-10000, -8000);
        scaleVel    = ofRandom(50, 50);
        rot         = 0;
        rotationVel = ofRandom(10);
        opacity     = .5;
        opacityVel  = 0.0;
    }
    
    void updateScale() {
        
        rot += rotationVel;
        scalez += scaleVel;
        if(scalez > 1000)
            scalez = ofRandom(-10000, -8000);
            //scalez *= 2;
        
        // decrease opacity for ending
        opacity += opacityVel;
        
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
            
            ofColor fillColor   = path.getFillColor();
            ofColor strokeColor = path.getStrokeColor();
            
            fillColor.a *= opacity;
            strokeColor.a *= opacity;
            
            p.setColor(path.getFillColor());
            p.setStrokeColor(path.getStrokeColor());
            p.setStrokeWidth(0);
            
            
            p.draw();
            
        }
        
        if(type == SHAPE_TYPE_IMG)  {

            ofSetColor(255, 255 * opacity);
            img.draw(line.getBoundingBox().x,line.getBoundingBox().y);
            
        }
        
        
    }
    
    void clear() {
        
        path.clear();
        line.clear();
        img.clear();
        
    }
    
    bool operator < (const VectorObject &other) const {
        return scalez < other.scalez;
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
    
    void captureShapes(int mode = SHAPE_TYPE_VECTOR);
    void clear();
    void undo();
    void save();
    
    void setTreshold(float thresold);
    void setAperture(float aperture);
    void loadFolder(string folder);
    
    void quit();
    
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
