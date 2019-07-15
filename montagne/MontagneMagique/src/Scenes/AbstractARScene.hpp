//
//  AbstractARScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#ifndef AbstractARScene_hpp
#define AbstractARScene_hpp

#include "ofMain.h"
#include "MagiqueMarker.hpp"

class AbstractARScene {
    
public:
    
    AbstractARScene(){
        
        this->marker        = NULL;
        this->bDebugMarker  = false;
        this->assetsDir     = "scenes/";
        this->scaleFactor   = 12.0f;
        this->bGenerateMask = false;
        this->lockedScene   = "";
        
    };
    
    virtual void setup(string name) {
        
        this->name = name;
        this->markerID = "";
        this->dataPath = this->assetsDir + name;
        this->animInMillisDelay     = 2000;
        this->animOutMillisDelay    = 1000;
        
        loadConfigJson();
        
    }
    
    virtual void setMarker(shared_ptr<MagiqueMarker> marker) {
        
        this->marker = marker;
        
    }
    
    void loadConfigJson() {
        
        file.open(this->dataPath + "/config.json");
        
        assert(file.exists());
        
        file >> configJson;
        
        // reset time stamp 
        timestamp                   = std::filesystem::last_write_time(file);
        
        lockedScene                 = configJson.value("scene-lock", "");
        //marker->bIsAlwaysActive     = configJson.value("always-active", false);
        animInMillisDelay           = configJson.value("animInMillisDelay", this->animInMillisDelay);
        animOutMillisDelay          = configJson.value("animOutMillisDelay", this->animOutMillisDelay);
        markerID                    = configJson["marker-id"].get<std::string>();

        
    }
    
    void saveConfigJson() {
        
        ofSaveJson(this->dataPath + "/config.json", configJson);
    }
    
    virtual float getInOuPct() {
        
        if(!marker)
            return 1.0;
        
        if(marker->getIsSolidFound())
            return ofClamp(ofMap(marker->timeSolidFoundElapsed, 0, this->animInMillisDelay, 0, 1), 0, 1);
        else
            return ofClamp(ofMap(marker->timeSolidLostElapsed, 0, this->animOutMillisDelay, 1, 0),0, 1);
        

    }
    
    virtual bool hasConfigChanged(){
        
        int currentTimeStamp = std::filesystem::last_write_time(file);
        
        if(currentTimeStamp != timestamp) {
            loadConfigJson();
            return true;
            
        }
        
        return false;
        
    };
    
    virtual void update(){};
    virtual void draw(){};
    virtual void drawOffScreen(){};
    
    virtual void updateCamera(ofBaseHasPixels & input){};

    
    string markerID, name;
    string assetsDir;
    std::shared_ptr<MagiqueMarker> marker;
    
    bool bDebugMarker;
    
    // if this string is not empty, it will discard scene if not in current ( see app.currentSceneName )
    string lockedScene;
    
    bool bGenerateMask;
    
    void generateMaskFbo() {
        

        beginFlip();
        ofSetRectMode(OF_RECTMODE_CENTER);
        ofSetColor(255);
        ofDrawRectangle(0.0,0.0, marker->image->getWidth(), marker->image->getHeight());
        endFlip();
        ofSetRectMode(OF_RECTMODE_CORNER);

        
    }
    
    virtual void onMarkerTracked(){};
    virtual void onMarkerLost(){};


    
protected:
    
    void beginFlip() {
        
        ofPushMatrix();
        ofScale( 1.0 / scaleFactor, -1.0 / scaleFactor);
        
        float x = marker->width * scaleFactor * .5;
        float y = - marker->height * scaleFactor + marker->height * scaleFactor * .5;
        
    // weird offset issue
       // x += 4;
        //y -= 4;
        
        ofTranslate(x,y);

        if(!configJson["translate"].is_null() && configJson["translate"].is_object()) {
            ofTranslate(configJson["translate"]["x"],configJson["translate"]["y"]);
        }
        
    }
    
    void endFlip() {
        
        if(bDebugMarker) {
            
            if(!configJson["translate"].is_null() && configJson["translate"].is_object()) {
                float offsetx = configJson["translate"]["x"];
                float offsety = configJson["translate"]["y"];
                ofTranslate(-offsetx, -offsety);
            }
            
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofSetColor(255, 125);
            marker->image->draw(0.0,0.0);
            ofSetColor(255, 255);
            
            ofPushMatrix();
            ofSetColor(255, 0, 0);
            ofDrawLine(-10, 0, 10, 0);
            ofDrawLine(0, -10, 0, 10);
            ofPopMatrix();
            ofSetRectMode(OF_RECTMODE_CORNER);
            
        }
        
        ofPopMatrix();
    }
    
    // this is used for fade in / out
    int animInMillisDelay, animOutMillisDelay;
    
    // for configuration
    string dataPath;
    ofJson configJson;
    virtual void onFoundMarker(){};
    virtual void onLostMarker(){};
    
    float scaleFactor;
    int timestamp;
    ofFile file;
    
};


#endif /* AbstractARScene_hpp */
