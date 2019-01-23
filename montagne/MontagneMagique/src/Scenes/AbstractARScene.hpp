//
//  AbstractARScene.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 08/01/2019.
//

#ifndef AbstractARScene_hpp
#define AbstractARScene_hpp

#include "ofMain.h"

class AbstractARScene {
    
public:
    
    AbstractARScene(){};
    
    virtual void setup(string name) {
        this->dataPath = "scenes/"+name;
    }
    virtual void update(){};
    virtual void draw(int markerIndex, int markerWidth, int markerHeight){};
    virtual void setMarkerIndex(int index) { this->markerIndex = index };);
    
protected:
    
    string dataPath;
    int markerIndex;
    
    virtual void onFoundMarker(){};
    virtual void onLostMarker(){};

    
    
    
    
};


#endif /* AbstractARScene_hpp */
