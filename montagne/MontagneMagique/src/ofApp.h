#pragma once

#include "ofMain.h"
#include "MontagneApp.hpp"
#include "ofxSyphon.h"

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
    
        void setInputMode(int mode);
    
        int videoInputWidth, videoInputHeight;
    
        MontagneApp app;
    
        // this is the rectangle canvas with position & size for full screen
        ofRectangle  cameraRectCanvas;

        ofVideoPlayer   videoInput;
        ofVideoGrabber  cameraInput;
        ofxSyphonClient syphonInput;
        ofPixels        syphonInputPixels;
        ofImage         syphonInputImg;
    
        ofJson          configJson;
    
        int intputMode;
        ofxSyphonServer syphonLayer;
    
        ofFbo syphonFbo;
        string messageString;
};
