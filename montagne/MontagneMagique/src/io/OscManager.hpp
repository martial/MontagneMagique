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
    
    void sendMessage(string adress, string label);
    void sendMessage(string adress, int value);
    
    void setSceneEvent(ofxOscMessage & m);
    
    void keyPressed(int key);
    
private:
    
    MontagneApp *       montagneApp;
    ArSceneManager *    sceneManager;
    
    ofxOscReceiver      receiver;
    ofxOscSender        sender;
    
    string              lastAddress;
    
};

#endif /* OscManager_hpp */
