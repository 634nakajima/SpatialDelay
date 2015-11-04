#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#define HOST "localhost"
#define PORT 12001

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxOscSender sender;
    ofVec3f origin, d[3];
    float angle, elapsed;
    int current, delayCount;
    bool showD1, showD2, showD3;
    ofVec3f prevs[500];
    ofxPanel gui;
    ofxFloatSlider speed;
    ofxFloatSlider delayTime;
    ofxIntSlider height;
    ofxIntSlider radius;
};
