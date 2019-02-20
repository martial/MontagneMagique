//
//  BeesDepthScene.cpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#include "BeesDepthScene.hpp"


void BeesDepthScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    bee0.load(this->dataPath + "/Abeilles5.png");
    bee1.load(this->dataPath + "/Abeilles4.png");
    bee2.load(this->dataPath + "/Abeilles3.png");
    
    bee0.setAnchorPercent(0.5,0.5);
    bee1.setAnchorPercent(0.5,0.5);
    bee2.setAnchorPercent(0.5,0.5);
    
    


    
}

void BeesDepthScene::update() {
    
    if(positions.size() == 0) {
        
        for(int i=0; i<6; i++) {
            
            ofVec2f pos;
            pos.set(ofRandom(-marker->width,  marker->width*2), ofRandom(-marker->height,  marker->height*2));
            positions.push_back(pos);
            
            float scale = ofRandom(0.2, 1.0);
            scales.push_back(scale);
            
        }
        
    }
    
}


void BeesDepthScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        ofSetColor(255, pct * 255);
        ofPushMatrix();
        ofScale(1, -1.0);
        ofTranslate(0.0, -marker->height);
        
        //marker->image->draw(0.0, 0.0, marker->width, marker->height);
        
       
        
        for(int i=0; i<positions.size(); i++) {
            
            float pct = 0.5 + cos(ofGetElapsedTimef() + i) * 0.5;
            pct = ofMap(pct, 0.0, 1.0, scales[i], 1.0);
            
            ofPushMatrix();
            ofTranslate(positions[i].x, positions[i].y * pct, (1.0 -  pct) * 10);
            ofScale(scales[i], scales[i]);
            
            pct = 0.5 + cos(ofGetElapsedTimef() + i + 100) * 0.5;
            ofRotateXDeg(-5 + pct * 10);
            
            pct = 0.5 + cos(ofGetElapsedTimef() + i + 200) * 0.5;
            ofRotateYDeg(-5 + pct * 10);
            
            bee0.draw(0.0,0.0, 5);
            
            if((int)ofGetFrameNum() % 3 != 0)
                bee1.draw(0.0,0.0, 10);
            
            bee2.draw(0.0,0.0, 15);
            ofPopMatrix();
            
            
        }
        
      ofPopMatrix();
        /*
        ofPushMatrix();
        ofTranslate(bee0.getWidth() / 3.0, bee0.getHeight() / 3.0);
        ofScale(0.25, 0.25);
        ofRotateX(10);
        bee0.draw(0.0,0.0, 5);
        
        if((int)ofGetFrameNum() % 4 != 0)
            bee1.draw(0.0,0.0, 10);
        
        bee2.draw(0.0,0.0, 15);
        ofPopMatrix();
        
        ofPushMatrix();
        ofTranslate(-bee0.getWidth() / 3.0 /0.5, -bee0.getHeight() / 3.0 / 1.2);
        ofScale(0.3, 0.3);
        ofRotateX(-10);
        bee0.draw(0.0,0.0, 5);
        bee1.draw(0.0,0.0, 10);
        bee2.draw(0.0,0.0, 15);
        ofPopMatrix();
        
        // ofDrawRectangle(0.0, 0.0, marker->width, marker->height);
        ofPopMatrix();
         
         */
        
    }
    
    
}
