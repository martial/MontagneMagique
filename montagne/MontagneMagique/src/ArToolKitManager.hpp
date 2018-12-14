//
//  ArToolKitManager.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 13/12/2018.
//

#ifndef ArToolKitManager_hpp
#define ArToolKitManager_hpp

#include "ofMain.h"
#include "ofxARTNftTracker.h"

class ArToolKitManager {
    
public:
    
    void setup(int width, int height);
    void loadTrackers();
    
    void update(ofBaseHasPixels & input);
    void debugDraw();
    
    void onNewMarker(int & mId);
    void onLostMarker(int & mId);
    
    void clean();
    
private:
    
    int inputWidth, inputHeight;
    
    vector<std::shared_ptr<ofxArtool5::NftTracker>> trackers;
    
};

#endif /* ArToolKitManager_hpp */
