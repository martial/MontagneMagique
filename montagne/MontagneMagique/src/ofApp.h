#pragma once

#include "ofMain.h"
#include "MontagneApp.hpp"
#include "ofxSyphon.h"
#include "OscManager.hpp"
#include "ofxGui.h"

#define INPUT_VIDEO 0
#define INPUT_CAMERA 1
#define INPUT_SYPHON 2


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void exit();
    
        void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
        void serverUpdated(ofxSyphonServerDirectoryEventArgs &args);
        void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
    
        void setInputMode(int mode);
    
        int videoInputWidth, videoInputHeight;
    
        MontagneApp app;
    
        ofBaseHasPixels * trackedVideoInput;
    
        // this is the rectangle canvas with position & size for full screen
        ofRectangle  cameraRectCanvas;

        ofVideoPlayer   videoInput;
        ofVideoGrabber  cameraInput;
    
        ofxSyphonServerDirectory    syphonDir;
        ofxSyphonClient             syphonInput;
        ofxSyphonServer             syphonLayer;

        ofPixels                    syphonInputPixels;
        ofImage                     syphonInputImg;
    
        OscManager                  oscManager;
    
        ofJson          configJson;
    
        bool bDebugTrackers;
        int intputMode;
    
        ofFbo syphonFbo;
        string messageString;
    
        ofxFloatSlider bigBangDampingMin,bigBangDampingMax;
        ofxFloatSlider bigBangScaleMin,bigBangScaleMax;
        ofxFloatSlider bigBangScaleDampingScale;
        ofxFloatSlider bigBangRepulsionFactor;
        ofxIntSlider bigBangParticleIntensity;

        ofxPanel gui;
        ofParameter<bool> debugMode;
        ofParameterGroup parameters;
        bool    bDrawGui;
    
        void addMessage(string message);
        vector<string> messages;
        string lastMessage;
        int duplicateCount;
    
        bool bDrawPreview;
};
