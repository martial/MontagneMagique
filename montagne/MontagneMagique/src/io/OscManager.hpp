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
#include "arSceneManager.hpp"
#include "MontagneApp.hpp"

class OscManager {
  
public:
    
    void setup(int receiverPort, int senderPort, string senderIp);
    void setSceneManager(ArSceneManager * manager);
    void setMontagneApp(MontagneApp * app);

    void update();
    void draw();
    
    void sendMessage(string adress, string label);
    
    
    void keyPressed(int key);
    
private:
    
    MontagneApp *       montagneApp;
    ArSceneManager *    sceneManager;
    
    ofxOscReceiver      receiver;
    ofxOscSender        sender;
    
    
};

#endif /* OscManager_hpp */
