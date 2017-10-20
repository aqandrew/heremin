#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //initialize the video grabber
    vidGrabber.setVerbose(true);
    vidGrabber.setup(320,240);

    setupGui();
}

//--------------------------------------------------------------
void ofApp::setupGui(){
    panel.setup("settings");
    panel.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
    panel.setDefaultFillColor(ofColor(160, 160, 160, 160));

    panel.add(doFullScreen.set("fullscreen (F)", false));
    doFullScreen.addListener(this, &ofApp::setFullScreen);
    panel.add(toggleGuiDraw.set("show GUI (G)", true));
    // TODO add fluid controls
}

//--------------------------------------------------------------
void ofApp::update(){
    //grab a new frame
    vidGrabber.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    vidGrabber.draw(20,20);

    if (toggleGuiDraw.get()) {
        ofDisableDepthTest();
        panel.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'f':
            doFullScreen.set(!doFullScreen.get());
            break;
        case 'g':
            toggleGuiDraw.set(!toggleGuiDraw.get());
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
