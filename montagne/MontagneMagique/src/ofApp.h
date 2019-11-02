#pragma once

#include "ofMain.h"
#include "MontagneApp.hpp"
#include "ofxSyphon.h"
#include "OscManager.hpp"
#include "ofxGui.h"
#include "ofxHapPlayer.h"
#include "ofxTimeMeasurements.h"
#include "ofxFastFboReader.h"

#define INPUT_VIDEO 0
#define INPUT_CAMERA 1
#define INPUT_SYPHON 2
#define INPUT_IMAGE 3

//#define USE_SAMPLER

#ifdef USE_SAMPLER
#include "Sampler.hpp"
#endif

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
        int videoOutputWidth, videoOutputHeight;

        MontagneApp app;
    
        ofBaseHasPixels * trackedVideoInput;
    
        // this is the rectangle canvas with position & size for full screen
        ofRectangle                 cameraRectCanvas;

        ofVideoPlayer               videoInput;
        ofVideoGrabber              cameraInput;
        ofImage                     imageInput;
    
        ofxSyphonServerDirectory    syphonDir;
        ofxSyphonClient             syphonInput;
        ofxSyphonServer             syphonLayer;

        ofxFastFboReader            reader;
        ofFbo                       resizedFbo;
        ofPixels                    resizedInputPixels;
        ofImage                     resizedInputImg;
    
        OscManager                  oscManager;
    
        ofJson                      configJson;

        // Gui
        ofxFloatSlider bigBangDampingMin,bigBangDampingMax;
        ofxFloatSlider bigBangScaleMin,bigBangScaleMax;
        ofxFloatSlider bigBangScaleDampingScale;
        ofxFloatSlider bigBangRepulsionFactor;
        ofxIntSlider bigBangParticleIntensity;

        ofxPanel gui;
        ofParameter<bool> debugMode;
        ofParameterGroup parameters;
        bool    bDrawGui;
    
        // messages
        void addMessage(string message);
        vector<string> messages;
        string lastMessage;
        int duplicateCount;
    
        // debug
        bool bDebugTrackers;
        int intputMode;
        string messageString;
    
        bool bDrawPreview;
        bool bDrawMessages;
    
        float currentFrameRate;
    
#ifdef USE_SAMPLER
        Sampler sampler;
        bool bDrawSampler;
#endif
    
};
