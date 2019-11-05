#include "OscManager.hpp"
#include "BigBangScene.hpp"
#include "BirdsScene.hpp"
#include "DrawScene.hpp"
#include "ofApp.h"

void OscManager::setup(int receiverPort, int senderPort, string senderIp) {
    
    receiver.setup(receiverPort);
    sender.setup(senderIp, senderPort);
    
}

void OscManager::update() {
    
    ofApp * app = (ofApp*) ofGetAppPtr();

    while(receiver.hasWaitingMessages()){
        
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        string adress           = m.getAddress();
        vector<string> splitted = ofSplitString(adress, "/", true);
        string first            = splitted[0];

        // HAP Video ----------------------------------------------------------------------------------------------------
        if(first == "video") {
            
             if(splitted.size() == 2) {
                 
                 if(splitted[1] == "stop") {
                     
                     // deprecated
                     
                 } else {
                 
                    app->app.hapPlayer.setVideo(splitted[1]);
                     
                 }
                 
             } else if(splitted.size() == 3) {
                 
                 if(splitted[2] == "loop") {
                     
                     app->app.hapPlayer.setVideo(splitted[1], true);
                     
                 }
                 
             }
            
        }
        
        // Setting scene conf ----------------------------------------------------------------------------------------------------
        
        if(first == "scene") {
            
            
            if(splitted.size() > 1) {
                
                app->app.currentSceneName = splitted[1];
                
                 if(splitted.size() > 2) {
                     
                     app->app.currentSubSceneName = splitted[2];

                 } else {
                     
                     app->app.currentSubSceneName = "";
                 }
                
            } else {
                
                app->app.currentSceneName       = "";
                app->app.currentSubSceneName    = "";
                
            }
            
            // we check for scene-locks here
            this->sceneManager->activateMarkersFromScene();
            
            // set modes
            
            if(app->app.currentSceneName == "cosmogonie" ) {
                
                this->montagneApp->setMode(SCENE_MODE);
                
            } else if(app->app.currentSceneName == "ete" ) {
                
                this->montagneApp->setMode(DRAW_MODE);
                
            } else if(app->app.currentSceneName == "bulles" ) {
                
                this->montagneApp->setMode(BUBBLES_MODE);
                
            }
            
            else {
                
                this->montagneApp->setMode(TRACKING_MODE);
                
            }
            
        }
        
        // Scenes ----------------------------------------------------------------------------------------------------
        
        if(m.getAddress() == "/scene/blackout") {
             
        }
        
        if(m.getAddress() == "/scene/cosmogonie/accueilpublic") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 0;
            
        }
        
        if(m.getAddress() == "/scene/cosmogonie/introkalimba") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 1;
        }
        
        if(m.getAddress() == "/scene/cosmogonie/melodiekalimba") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 2;
        }
        
        if(m.getAddress() == "/scene/cosmogonie/titrage") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.mode = 2;
            
        }
        
        if(m.getAddress() == "/scene/printemps/nuit") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/soleil") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/ours") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/oiseaubulles") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/pluie") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/underground") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/nuit2") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/finnuit") {
            
        }
        
        if(m.getAddress() == "/scene/printemps/finabeilles") {
            
        }
        
        if(m.getAddress() == "/scene/ete/voloiseau") {
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->intro();
        }
        
        if(m.getAddress() == "/event/ete/voloiseau/virages") {
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->scene2();
        }
        
        if(m.getAddress() == "/event/ete/voloiseau/espace") {
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->scene1();
        }
        
        if(m.getAddress() == "/event/ete/voloiseau/fin") {
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->outro();
        }
        
        // Events ----------------------------------------------------------------------------------------------------
        
        if(m.getAddress() == "/event/cosmogonie/particules") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            
            int nParticles      = m.getArgAsInt(0);
            float minSize       = m.getArgAsFloat(1);
            float maxSize       = m.getArgAsFloat(2);
            float minFriction   = m.getArgAsFloat(3);
            float maxFriction   = m.getArgAsFloat(4);
            
            bigBangScene->bigBang.addParticles(nParticles, minSize, maxSize, minFriction, maxFriction);
            
        }
        
        if(m.getAddress() == "/event/cosmogonie/kalimba") {
            
            BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
            bigBangScene->bigBang.startRepulsion( m.getArgAsFloat(0));
            
        }
        
        if(m.getAddress() == "/event/stoploops") {
            
            this->sceneManager->setGenericSceneLoopMode(OF_LOOP_NONE);
            
        }
        
        if(m.getAddress() == "/event/drawn/thresold") {
            
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->setTreshold(m.getArgAsFloat(0));
        }
        
        if(m.getAddress() == "/event/drawn/captureimg") {
            
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->captureShapes(SHAPE_TYPE_IMG);
        }
        
        if(m.getAddress() == "/event/drawn/capturevector") {
        
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->captureShapes(SHAPE_TYPE_VECTOR);
        }
        
        if(m.getAddress() == "/event/drawn/undo") {
            
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->undo();
        }
        
        if(m.getAddress() == "/event/drawn/removelast") {
            
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->removeLastCaptured();
        }
        
        if(m.getAddress() == "/event/drawn/removeall") {
            
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->removeAll();
        }
        
        if(m.getAddress() == "/event/drawn/quit") {
            
            DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
            drawScene->quit();
        }
        
        if(m.getAddress() == "/event/printemps/oiseau/pitch") {
            
            BirdsScene * birds = (BirdsScene*) this->sceneManager->getSceneIndexForPath("OISEAU_BULLES");
            birds->setPitchPct(ofNormalize(m.getArgAsFloat(0), 0, 0.6));
            
        }
        
        if(m.getAddress() == "/event/printemps/oiseau2/pitch") {
            
            BirdsScene * birds = (BirdsScene*) this->sceneManager->getSceneIndexForPath("OISEAU_BULLES2");
            birds->setPitchPct(ofNormalize(m.getArgAsFloat(0), 0, 0.6));
            
        }
        
        if(m.getAddress() == "/event/printemps/oiseau3/pitch") {
            
            BirdsScene * birds = (BirdsScene*) this->sceneManager->getSceneIndexForPath("OISEAU_BULLES3");
            birds->setPitchPct(ofNormalize(m.getArgAsFloat(0), 0, 0.6));
            
        }

        app->addMessage(m.getAddress());
        
    }
    
    app = NULL;
    
}

void OscManager::setSceneEvent(ofxOscMessage & m) {}

void OscManager::keyPressed(int key) {
    
    
    if(key == 'i') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->scene1();
        
    }
    
    if(key == 'o') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->scene2();
        
    }
    
    if(key == 'O') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->outro();
        
    }
    
    if(key == 'a') {
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
        bigBangScene->bigBang.addParticles(1, 2, 6, 0.1, 0.2);
        
    }
    
    if(key == 'r') {
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
        bigBangScene->bigBang.startRepulsion(2);
        
    }
    
    if(key == 'R') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->removeLastCaptured();
        
    }
    
    if(key == 's') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->captureShapes(SHAPE_TYPE_IMG);
        
    }
    
    if(key == 'c') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->clear();
        
    }
    
    if(key == 'u') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->undo();
        
    }
    
    if(key == 'S') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->save();
        
    }
    
    if(key == 'Q') {
        
        DrawScene * drawScene = (DrawScene*) this->sceneManager->getSceneIndexForPath("draw");
        drawScene->quit();
        
    }
    
    if( key == OF_KEY_RIGHT) {
        
        BigBangScene * bigBangScene = (BigBangScene*) this->sceneManager->getSceneIndexForPath("bigbang");
        bigBangScene->bigBang.mode ++;
        if(bigBangScene->bigBang.mode > 2)
            bigBangScene->bigBang.mode = 0;
        
    }
    
}

void OscManager::setSceneManager(ArSceneManager * manager) {
    
    this->sceneManager = manager;
    
}

void OscManager::setMontagneApp(MontagneApp * app) {
    
    this->montagneApp = app;
    
}

void OscManager::sendMessage(string adress, string label) {
    
    ofxOscMessage m;
    m.setAddress(adress);
    m.addStringArg(label);
    sender.sendMessage(m, false);
    
}

void OscManager::sendMessage(string adress, int value) {
    
    ofxOscMessage m;
    m.setAddress(adress);
    m.addIntArg(value);
    sender.sendMessage(m, false);
    
}
