//
//  OscManager.hpp
//  MontagneMagique
//
//  Created by Martial Geoffre-Rouland on 19/02/2019.
//

#ifndef OscManager_hpp
#define OscManager_hpp

#include "ofMain.h"
#include "ofxOsc.h"

class OscManager {
  
public:
    
    void setup();
    void update();
    void draw();
    
    void sendMessage(string adress, string label);
    
private:
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    
};

#endif /* OscManager_hpp */
