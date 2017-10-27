#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCvContourFinder.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12345

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

    // Camera stuff
    int camWidth, camHeight;
    ofVideoGrabber vidGrabber;
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImg, grayBg, grayDiff;
    ofxCvContourFinder contourFinder;
    ofRectangle faceMask;

    // Sound stuff
    ofPoint pVolume;
    ofPoint pPitch;
    ofxOscSender sender;

    // GUI/helper stuff
    void setupGui();
    ofxPanel panel;
    ofxButton learnBackgroundButton;
    ofParameter<bool> toggleGuiDraw;
    ofParameter<bool> doFullScreen;
    void setFullScreen(bool& _value) { ofSetFullscreen(_value); }
    void learnBackground();
		
};
