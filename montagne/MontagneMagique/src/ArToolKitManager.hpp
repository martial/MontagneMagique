//
//  ArToolKitManager.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 13/12/2018.
//

#ifndef ArToolKitManager_hpp
#define ArToolKitManager_hpp

#include "ofMain.h"
#include "MagiqueMarker.hpp"
class ArToolKitManager {
    
public:
    
    void setup(int width, int height);
    void loadTrackers();
    
    void update(ofBaseHasPixels & input);
    void debugDraw();
    
    void onNewMarker(int & mId);
    void onLostMarker(int & mId);
    
    void clean();
    
    void setDelays( int markerLostDelay, int markerFoundDelay);
    
    vector<std::shared_ptr<MagiqueMarker>> trackers;
    vector<ofImage> images;
    
    int frameDrop;
    
private:
    
    void onSolidFoundEvent(string & markerid);
    void onSolidLostEvent(string & markerid);

    
    int inputWidth, inputHeight;
    int markerLostDelay, markerFoundDelay;
    
    
    
};

#endif /* ArToolKitManager_hpp */
