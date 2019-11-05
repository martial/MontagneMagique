#include "BeesDepthScene.hpp"
#include "ofApp.h"

void BeesDepthScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
    bee0.load(this->dataPath + "/Abeilles5.png");
    bee1.load(this->dataPath + "/Abeilles4.png");
    bee2.load(this->dataPath + "/Abeilles3.png");
    beeShadow.load(this->dataPath + "/Abeilles_shadow.png");
    
    bee0.setAnchorPercent(0.5,0.5);
    bee1.setAnchorPercent(0.5,0.5);
    bee2.setAnchorPercent(0.5,0.5);
    beeShadow.setAnchorPercent(0.5,0.5);
    
}

void BeesDepthScene::update() {
    
    hasConfigChanged();
    
    float pct = getInOuPct();
    
    if(pct > 0 && this->marker->getIsSolidFound()) {
                
        float minScale = 0.2;
        float maxScale = 1;
        
        if(bees.size() == 0) {
            
            for(int i=0; i<6; i++) {
                
                Bee bee;
                ofVec2f pos(ofRandom(-marker->width,  marker->width*2), ofGetHeight() *2);
                bee.setInitialStatus(pos, ofRandom(minScale, maxScale));
                
                ofVec2f beePos = bee.pos;
                beePos.y =  ofRandom(-marker->height,  marker->height*2);
                bee.setPosition(beePos);

                bees.push_back(bee);
               
            }
            
        }
        
        for(int i=0; i<bees.size(); i++) {
            bees[i].update();
            bees[i].setAlpha(pct);
        }
        
        // change randomly
        if( ofRandom(1) > .99 ) {
            
             ofVec2f pos(ofRandom(-marker->width,  marker->width*2), ofRandom(-marker->height,  marker->height*2));
            
            // pick a random index
            int rdmIndex = floor(ofRandom(bees.size()));
            
            bees[rdmIndex].setPosition(pos);
            bees[rdmIndex].setScale(ofRandom(minScale, maxScale));
          
            // send osc ?
            ofApp * app = (ofApp*) ofGetAppPtr();
            app->oscManager.sendMessage("/AR/bees/move", "move");
            
        }
        
    } else {
        
        bees.clear();
        
    }
    
    sort(bees.begin(), bees.end());
    
}


void BeesDepthScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct <= 0 || !this->marker->getIsSolidFound())
        return;
        
    if(pct > 0 && this->marker->getIsSolidFound()) {
        
        beginFlip();

        ofSetColor(255, 255);
        
        float rotationRange = 1;
        for(int i=0; i<bees.size(); i++) {
            
            //float pct = 0.5 + cos(ofGetElapsedTimef() + i) * 0.5;
            //pct = ofMap(pct, 0.0, 1.0, scales[i], 1.0);
            float pct = 0.5 + cos(ofGetElapsedTimef() + i + 100) * 0.5;
            bees[i].rotationX = -rotationRange + pct * rotationRange * 2;
            pct = 0.5 + cos(ofGetElapsedTimef() + i + 200) * 0.5;
            bees[i].rotationY = -rotationRange + pct * rotationRange * 2;
            bees[i].drawShadow(beeShadow);
            
        }
        
        for(int i=0; i<bees.size(); i++) {

            bees[i].draw(bee0, bee1, bee2);
            
        }
        
        endFlip();
        
    }
    
}
