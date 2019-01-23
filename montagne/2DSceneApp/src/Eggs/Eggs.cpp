//
//  Eggs.cpp
//  2DSceneApp
//
//  Created by Martial Geoffre-Rouland on 16/01/2019.
//

#include "Eggs.hpp"

void Eggs::setup(int width, int height, string dataPath) {
    
    for(int i=0; i<3; i++) {
        
        ofImage img;
        images.push_back(img);
        images[i].load(dataPath + "/OEUF_"+ofToString(i+1)+".png");
        images[i].setAnchorPoint(images[i].getWidth()/2, images[i].getHeight()/2);

    }
    
    // load mask and positions
    ofImage mask;
    mask.load(dataPath + "/FOURMILLIERE_0_mask.jpg");
    mask.mirror(true, false);
    ofPixels pixels = mask.getPixels();
    int w = mask.getWidth();
    int h = mask.getHeight();
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){
            
            float brt = pixels.getColor(i, j).getBrightness();
            
            if (brt == 255) {
                ofVec2f pos;
                pos.set(i,j);
                pos /= 3.0;
                allowedPositions.push_back(pos);
            }
        }
    }
    
    // load eggs
    for(int i=0; i<30; i++) {
        
        int rdm = floor(ofRandom(images.size()));
        imageRef.push_back(&images[rdm]);
        
        ofxAnimatableFloat radius;
        radiuses.push_back(radius);
        radiuses[i].setup();
        radiuses[i].reset(0.0);
        radiuses[i].animateTo(0.05+ofRandom(0.05));
        radiuses[i].setCurve(EASE_OUT);
        radiuses[i].setDuration(5 + ofRandom(15));
        
        ofVec2f pos;
        positions.push_back(pos);
        
        int rdmIndex = floor(ofRandom(allowedPositions.size()));
        positions[i].set(allowedPositions[rdmIndex]);
        
        opacities.push_back(ofRandom(0.8, 1.0));
    }
    
}

void Eggs::update() {
    
    for(int i=0; i<radiuses.size(); i++) {
        radiuses[i].update(1.0 / 60.0f);
    }
    
}

void Eggs::draw(float opacity) {
    
    ofEnableAlphaBlending();
    for(int i=0; i<radiuses.size(); i++) {
        

        float pct = radiuses[i].getCurrentValue();
        float noise = ofMap(ofNoise(ofGetElapsedTimef() +i), 0.0, 1.0, 0.4, 1.0);
        pct *= noise;
        
        ofSetColor( 255, 255 * opacities[i] * opacity);
        
        ofPushMatrix();
        ofTranslate(positions[i].x, positions[i].y);
        ofScale(pct, pct);
        imageRef[i]->draw(0.0, 0.0);
        ofPopMatrix();
        
    }
    

    
}
