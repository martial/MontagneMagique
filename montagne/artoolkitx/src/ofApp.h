#pragma once

#include "ofMain.h"
#include <ARX/ARController.h>
#include <ARX/ARUtil/time.h>
#include <ARX/AR/config.h>
#include <ARX/ARG/arg.h>
#include "ofxCv.h"

#define USE_BUFFER

struct marker {
    const char *name;
    float height;
};
static const struct marker markers[] = {
    {"pinball.jpg", 1.0}
};
static const int markerCount = (sizeof(markers)/sizeof(markers[0]));
static int32_t viewport[4];
static float projection[16];
static float gView[16];

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
    
        const char *vconf;
        const char *cpara;

        ARController * arController;
        int markerIDs[1];
    
        bool contextWasUpdated;
        uint32_t * pixelBuffer;
        ofTexture  tex;
        int camWidth, camHeight;
    

        AR2VideoBufferT     buffer;
        ofVideoGrabber      camera;
        ofPixels            pixels;
        ofImage             estImage;
};
