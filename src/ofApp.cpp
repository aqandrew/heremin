#include "ofApp.h"

// http://openframeworks.cc/documentation/ofxOpenCv/ofxCvContourFinder/

//--------------------------------------------------------------
void ofApp::setup(){
    //initialize the video grabber
    camWidth = 640;
    camHeight = 480;
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImg.allocate(camWidth, camHeight);
    grayImg.allocate(camWidth, camHeight);
    grayBg.allocate(camWidth, camHeight);
    grayDiff.allocate(camWidth, camHeight);

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
    panel.add(learnBackgroundButton.setup("learn background (B)"));
    learnBackgroundButton.addListener(this, &ofApp::learnBackground);
    // TODO add fluid controls
}

//--------------------------------------------------------------
void ofApp::update(){
    //grab a new frame
    vidGrabber.update();

    if (vidGrabber.isFrameNew()) {
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImg = colorImg; // convert our color image to a grayscale image
        grayDiff.absDiff(grayBg, grayImg);
        grayDiff.threshold(30);
        contourFinder.findContours(grayDiff, (camWidth * camHeight) / 20, (camWidth * camHeight) / 4, 3, false);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(ofColor::white);
//    vidGrabber.draw(0, 0, ofGetWidth(), ofGetHeight());
    vidGrabber.draw(0, 0, camWidth, camHeight);
    grayImg.draw(0, camHeight, camWidth / 3, camHeight / 3);
    grayBg.draw(camWidth / 3, camHeight, camWidth / 3, camHeight / 3);
    grayDiff.draw(camWidth * 2 / 3, camHeight, camWidth / 3, camHeight / 3);
//    contourFinder.draw(0, 0);

    for (int i = 0; i < contourFinder.blobs.size(); ++i) {
        ofPoint blobCentroid = contourFinder.blobs[i].centroid;
        ofDrawCircle(blobCentroid[0], blobCentroid[1], 10);
    }

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
        case 'b':
            learnBackground();
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

//--------------------------------------------------------------
void ofApp::learnBackground(){
    grayBg = grayImg; // update the background image
}
