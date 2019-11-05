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
#include "ofxTwistedRibbon.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfColor.h"

#define SHAPE_TYPE_VECTOR 0
#define SHAPE_TYPE_IMG 1
#define SHAPE_TYPE_BOTH 2
#define SHAPE_TYPE_TERRAIN 3

class VectorObject {
  
public:
    
    ofPath              path;
    ofPolyline          line;
    ofImage             *img;
    
    float               rot;
    float               opacity;
    float               opacityVel;
    int                 type;
    
    ofVec3f             position, originalPosition;
    ofVec3f             anchorPoint;
    
    ofxAnimatableFloat  scale;
    
    bool                bWaitToBeRemoved;
    
    VectorObject() {
        
        type                = SHAPE_TYPE_IMG;
        bWaitToBeRemoved    = false;
        rot                 = 0;
        opacity             = 1.0;
        opacityVel          = 0.0;
        scale.reset(1.0);
        
    }
    
    ofVec3f setAnchorPoint(ofVec3f _anchorPoint) {
        this->anchorPoint = _anchorPoint;
    }
    
    ofVec3f setAnchorPointToMiddleLine() {
        anchorPoint.set(this->line.getCentroid2D().x, this->line.getCentroid2D().y);
    }
    
    
    
    
    ofVec3f positionToWorld() {
        
        ofVec3f pos;
        pos.x = position.x - anchorPoint.x;
        pos.y = position.y - anchorPoint.y;
        pos.z = position.z;
        return pos;
        
    }
    
 
  
    void draw() {
        
        scale.update(1.0 / 60.0f);
        float currentScale = scale.getCurrentValue();
        
        if(type == SHAPE_TYPE_VECTOR)  {
            
            if(line.getVertices().size() == 0)
                return;
            
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
            
            //fillColor.a     *= opacity;
           // strokeColor.a   *= opacity;
            
            p.setColor(path.getFillColor());
            p.setStrokeColor(path.getStrokeColor());
            p.setStrokeWidth(0);
            
            // now we need to draw line to its point + anchor point
            //float x = position.x - line.getBoundingBox().x;
            //float y = position.y - line.getBoundingBox().y;
            // z remains untouched
            // we're at 0,0 now, let's deal with the anchor point
            
            ofPushMatrix();
            ofTranslate(position);
            ofScale(currentScale,currentScale); // do our transormation operations
            ofTranslate(-anchorPoint.x, -anchorPoint.y);
            // then draw at origin
            p.draw();
            ofTranslate(anchorPoint.x,anchorPoint.y);
            ofPopMatrix();
            
        }
        
        if(type == SHAPE_TYPE_IMG || type == SHAPE_TYPE_TERRAIN)  {

            if(!img)
                return;
            
            ofSetColor(255);
            ofPushMatrix();
            ofTranslate(position);
            ofScale(currentScale,currentScale); // do our transormation operations
            ofRotateZ(rot);
            ofTranslate(-anchorPoint.x, -anchorPoint.y);
            
            // ofDrawRectangle(0,0, 100, 100);
            img->draw(0,0);
            ofTranslate(anchorPoint.x,anchorPoint.y);
            ofPopMatrix();
            
            if(type == SHAPE_TYPE_IMG) {
               // ofLogNotice("currentScale") << currentScale;
            }
        }
        
        
    }
    
    void setScaleTransition(float _scale, float _duration, AnimCurve _curveStyle) {
        
        scale.setDuration(_duration);
        scale.setCurve(_curveStyle);
        scale.animateTo(_scale);
        
    }
    
    void clear() {
        
        path.clear();
        line.clear();
        img = NULL;
        
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
    void removeLastCaptured();
    void removeAll();

    void save();
    
    void setTreshold(float thresold);
    void setAperture(float aperture);
    void loadFolder(string folder);
    void loadFolderOfImageForTerrain(string folder);

    void quit();
    
    void intro();
    void scene1();
    void scene2();
    void outro();
    
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
    ofCamera cam;
    ofLight light;
    
    shared_ptr<DofPass> dofPass;
   // shared_ptr<ToonPass> toonPass;
    shared_ptr<FakeSSSPass> fakeSSSPass;

    BirdSprite bird;
    
    ofxTwistedRibbon *ribbon, *bigRibbon;
    int poszCount;
   // ofEasyCam cam;
    
    // path
    vector<ofVec3f> points;
    ofVec3f ribbonPath;
    int nPoints;
    
    // lights
    ofLight pointLight;
    ofMaterial material;

    // animation
    ofxAnimatableFloat      cameraPosY, lookAtPosY, terrainCurve, terrainCols, terrainColsScale, particlesScale, birdMainScale, birdMainPosY;
    ofxAnimatableFloat      pathXAmplitude, pathYAmplitude, birdPosYCosAmp;

    ofxAnimatableOfColor    backgroundColorTop, backgroundColorBottom, mountainsColor, mountainsSecondColor, pathColor;
    
    ofImage particle;
    vector<ofImage*> images;
    
    ofMesh  bgGradientMesh;
    ofImage bgParralaxFront, bgParralaxBack;
    //
    ofPoint viewportSize;

};


#endif /* GenericScene_hpp */
