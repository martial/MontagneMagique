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
    
    AbstractARScene(){};
    
    virtual void setup(string name) {
        
        this->markerID = "";
        this->dataPath = "scenes/"+name;
        this->animInMillisDelay     = 2000;
        this->animOutMillisDelay    = 2000;
        
        ofFile file;
        file.open(this->dataPath + "/config.json");
        
        assert(file.exists());
        
        if(file.exists())
            file >> configJson;
        
        this->markerID = configJson["marker-id"];
    }
    
    virtual void update(){};
    virtual void draw(){};
    
    string markerID;
    std::shared_ptr<MagiqueMarker> marker;
    
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
