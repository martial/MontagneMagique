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
        
        this->marker = NULL;
        this->bDebugMarker = false;
        
    };
    
    virtual void setup(string name) {
        
        this->name = name;
        this->markerID = "";
        this->dataPath = "scenes/"+name;
        this->animInMillisDelay     = 2000;
        this->animOutMillisDelay    = 1000;
        
        ofFile file;
        file.open(this->dataPath + "/config.json");
        
        assert(file.exists());
        
        if(file.exists())
            file >> configJson;
        
        this->markerID = configJson["marker-id"].get<std::string>();
        
    }
    virtual float getInOuPct() {
        
        if(!marker)
            return 1.0;
        
        if(marker->getIsSolidFound())
            return ofClamp(ofMap(marker->timeSolidFoundElapsed, 0, this->animInMillisDelay, 0, 1), 0, 1);
        else
            return ofClamp(ofMap(marker->timeSolidLostElapsed, 0, this->animOutMillisDelay, 1, 0),0, 1);
        

    }
    
    virtual void update(){};
    virtual void draw(){};
    
    string markerID, name;
    std::shared_ptr<MagiqueMarker> marker;
    
    bool bDebugMarker;

    
protected:
    
    // this is used for fade in / out
    int animInMillisDelay, animOutMillisDelay;
    
    // for configuration
    string dataPath;
    ofJson configJson;
    virtual void onFoundMarker(){};
    virtual void onLostMarker(){};

};


#endif /* AbstractARScene_hpp */
