#include "GenericScene.hpp"


void GenericScene::setup(string dataPath) {
    
    AbstractARScene::setup(dataPath);
    
}

void GenericScene::loadVideos() {
    
    videoPlayers.clear();
    
    int numOfVideos = configJson["videos"].size();
    
    for(int i=0; i < numOfVideos; i++) {
        
        string name =  configJson["videos"][i]["name"];
        
        ofxHapPlayer * p = new ofxHapPlayer();
        p->load(this->dataPath + "/" + name);
        
        bool bLoopMode = configJson["videos"][i].value("loop", 1);
        
        if(!bLoopMode)
            p->setLoopState(OF_LOOP_NONE);
        
        p->play();
        videoPlayers.push_back(p);
      
    }
    
}

void GenericScene::update() {
    
    if(hasConfigChanged()) {
        loadVideos();
    }
        
}

void GenericScene::draw() {
    
    float pct = getInOuPct();

    if(pct <= 0 || !this->marker->getIsSolidFound())
        return;
    
    beginFlip();
    
    if(pct > 0 ) {
        
        glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        ofSetColor(255, pct * 255);
        ofSetRectMode(OF_RECTMODE_CENTER);
        for(int i=0; i < videoPlayers.size(); i++) {
            
            if(videoPlayers[i]->isLoaded()) {
                
                ofPushMatrix();
                ofTranslate(configJson["videos"][i]["x"], configJson["videos"][i]["y"]);
                ofScale(configJson["videos"][i]["scale"], configJson["videos"][i]["scale"]);
                videoPlayers[i]->draw(0.0, 0.0);
                ofPopMatrix();
                
            }
        }
        ofSetRectMode(OF_RECTMODE_CORNER);
        endFlip();
        
    }
    
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
    
}

void GenericScene::setLoopMode(ofLoopType loopMode) {
    
    // no need to change if not detected
    if(this->marker && this->marker->getIsSolidFound()) {
        
        for(int i=0; i < videoPlayers.size(); i++) {
            
            float pos = videoPlayers[i]->getPosition();
            videoPlayers[i]->stop();
            videoPlayers[i]->setLoopState(loopMode);
            videoPlayers[i]->setPosition(pos);
            videoPlayers[i]->play();
        }
        
    }
    
}


void GenericScene::onMarkerTracked() {
    
    
    loadVideos();
    // if video is not in loop, we restart again
    int numOfVideos = videoPlayers.size();
    for(int i=0; i < numOfVideos; i++) {
        
        bool bLoopMode = configJson["videos"][i].value("loop", 1);
        
        if(!bLoopMode) {
            
            videoPlayers[i]->setLoopState(OF_LOOP_NONE);
           
        } else {
            videoPlayers[i]->setLoopState(OF_LOOP_NORMAL);

        }
        
        videoPlayers[i]->setPosition(0.0);
        videoPlayers[i]->play();
        
    }
    
}

void GenericScene::onMarkerLost() {
    
    for(int i=0; i < videoPlayers.size(); i++) {
        videoPlayers[i]->close();
        videoPlayers[i] = NULL;
    }
    
}
