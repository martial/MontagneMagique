#pragma once

#include "ofMain.h"
#include "VideoSampler.hpp"
#include "ofxPSBlend.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxAnimatableFloat.h"

#define BLACKMAGIC
//#define VIDEO_TEST

#ifdef BLACKMAGIC
#include "ofxBlackMagic.h"
#endif

#define PORT 9999

class ofApp : public ofBaseApp {

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
    
        void parseOsc();
        void fadeIn(int sec);
        void fadeOut(int sec);
    
        void exit();
    
#ifdef BLACKMAGIC
        ofxBlackMagic cam;
    
#ifdef VIDEO_TEST
        ofVideoPlayer videoPlayer;
#endif
    
#else
        ofVideoGrabber cam;
#endif
    
        int camWidth, camHeight;
        vector<VideoSampler> samplers;
        ofImage camImage;
    
        int currentSampler;
    
        ofPixels pixs;

        ofFbo mainFbo;
        ofFbo tempFbo;
        ofFbo camFbo;
        ofxPSBlend psBlend;
    
        ofFbo syphonFbo;
        ofxSyphonServer             syphonLayer;
        ofShader deinterlaceShader;
        ofxOscReceiver receiver;
        ofxAnimatableFloat cameraFadePct;

        int blendId;
    
        bool bDrawThumbs;
};
