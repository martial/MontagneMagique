#include "BigBangScene.hpp"

void BigBangScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    bigBang.setup(this->dataPath);
    setProps();
}

void BigBangScene::setProps() {
    
    float thresold              = configJson.value("thresold", 1);
    string targetColorHex       = configJson["color-blob-target"];
    ofColor targetColor         = ofColor::fromHex(ofHexToInt(targetColorHex));
    float colorBlobDistanceMax  = configJson.value("color-blob-target-distance-max", 200);
    
    bigBang.colorBlobTarget         = targetColor;
    bigBang.thresold                = thresold;
    bigBang.colorBlobDistanceMax    = colorBlobDistanceMax;
}


void BigBangScene::update() {
    
    if(hasConfigChanged()) {
        setProps();
    }
    
    bigBang.update();
    
}

void BigBangScene::draw() {
    
    float pct = getInOuPct();
    
    if(pct > 0 ) {
        
        ofSetColor(255, pct * 255);
        ofPushMatrix();
        bigBang.draw();
        ofPopMatrix();
        
    }
    
}
