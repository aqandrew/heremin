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

    reset();

    // Don't kill the user's shoulders. We only care about the neck-up
    colorImg.setROI(0, 0, camWidth, camHeight * 3 / 4);
    grayImg.setROI(0, 0, camWidth, camHeight * 3 / 4);
    grayBg.setROI(0, 0, camWidth, camHeight * 3 / 4);
    grayDiff.setROI(0, 0, camWidth, camHeight * 3 / 4);

    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);

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
    panel.add(nudgeMaskLLButton.setup("expand mask left (A)"));
    nudgeMaskLLButton.addListener(this, &ofApp::nudgeMaskLeftLeft);
    panel.add(nudgeMaskLRButton.setup("shrink mask left (S)"));
    nudgeMaskLRButton.addListener(this, &ofApp::nudgeMaskLeftRight);
    panel.add(nudgeMaskRLButton.setup("shrink mask right (K)"));
    nudgeMaskRLButton.addListener(this, &ofApp::nudgeMaskRightLeft);
    panel.add(nudgeMaskRRButton.setup("expand mask right (L)"));
    nudgeMaskRRButton.addListener(this, &ofApp::nudgeMaskRightRight);
    panel.add(resetMaskButton.setup("reset mask (M)"));
    resetMaskButton.addListener(this, &ofApp::resetMask);
    panel.add(resetButton.setup("reset (R)"));
    resetButton.addListener(this, &ofApp::reset);
    // TODO add fluid controls
}

//--------------------------------------------------------------
void ofApp::update(){
    //grab a new frame
    vidGrabber.update();

    // Detect movement
    if (vidGrabber.isFrameNew()) {
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImg = colorImg; // convert our color image to a grayscale image
        grayDiff.absDiff(grayBg, grayImg);
        grayDiff.threshold(30);

        // Don't look for contours in regions of grayDiff that lie within faceMask
        for (int i = 0; i < camWidth * camHeight; ++i) {
            if (ofInRange(i % camWidth,
                          faceMask.getX(),
                          faceMask.getX() + faceMask.getWidth())) {
                grayDiff.getPixels()[i] = 0;
            }
        }

        contourFinder.findContours(grayDiff,
                                   (camWidth * camHeight) / 25,
                                   (camWidth * camHeight) / 4,
                                   2,
                                   false);

        // Set the theremin controls if we detect two blobs
        if (contourFinder.blobs.size() == 2) {
            // Examine each blob
            for (int i = 0; i < 2; ++i) {
                ofPoint blobCentroid = contourFinder.blobs[i].centroid;

                // Right blob = volume
                if (blobCentroid[0] < camWidth / 2) {
                    pVolume = blobCentroid;
                }
                // Left blob = pitch
                else {
                    pPitch = blobCentroid;
                }
            }

            // Change volume/pitch no lower than the bottom half of the screen
            float oscVolume = ofMap(pVolume[1], camHeight / 2, 0, 0, 1);
            float oscPitch = ofMap(pPitch[1], camHeight / 2, 0, 261.63, 523.25); // C4 - C5
            oscVolume = oscVolume <= 0 ? 0 : oscVolume; // negative volume doesn't make sense

            ofxOscMessage m;
            m.setAddress("/heremin");
            m.addFloatArg(oscVolume);
            m.addFloatArg(oscPitch);
            sender.sendMessage(m);
            cout << "sending volume: " << m.getArgAsFloat(0) << ", pitch: " << m.getArgAsFloat(1) << endl;
        }
        // If blob number != 2, kill the noise
        else {
            silence();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();

    // Draw everything centered within the window
    ofTranslate((ofGetWidth() - camWidth) / 2, 0);

    ofFill();
    ofSetColor(ofColor::white);

    // It's faster to draw everything mirrored than to mirror before processing
    vidGrabber.draw(camWidth, 0, -camWidth, camHeight);
    grayImg.draw(camWidth / 3, camHeight, -camWidth / 3, camHeight / 3);
    grayBg.draw(camWidth * 2 / 3, camHeight, -camWidth / 3, camHeight / 3);
    grayDiff.draw(camWidth, camHeight, -camWidth / 3, camHeight / 3);
//    contourFinder.draw(0, 0);

    for (int i = 0; i < contourFinder.blobs.size(); ++i) {
        ofPoint blobCentroid = contourFinder.blobs[i].centroid;
        ofDrawCircle(camWidth - blobCentroid[0], blobCentroid[1], 10);
    }

    // Show the ignored region
    ofNoFill();
    ofDrawRectangle(faceMask);

    if (toggleGuiDraw.get()) {
        ofDisableDepthTest();
        panel.draw();

        if (!backgroundLearned) {
            ofDrawBitmapString("press B to learn background", 10, camHeight - 20);
        }
    }

    ofPopMatrix;
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
        case 'm':
            resetMask();
            break;
        case 'a':
            nudgeMaskLeftLeft();
            break;
        case 's':
            nudgeMaskLeftRight();
            break;
        case 'k':
            nudgeMaskRightLeft();
            break;
        case 'l':
            nudgeMaskRightRight();
            break;
        case 'r':
            reset();
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
    backgroundLearned = true;
}

//--------------------------------------------------------------
void ofApp::resetMask(){
    faceMask = ofRectangle(camWidth / 3, 0, camWidth / 3, camHeight);
}

//--------------------------------------------------------------
void ofApp::nudgeMaskLeftLeft(){
    faceMask.set(ofPoint(faceMask.getLeft() - 10, faceMask.getTop()), faceMask.getWidth() + 10, camHeight);
}

//--------------------------------------------------------------
void ofApp::nudgeMaskLeftRight(){
    faceMask.set(ofPoint(faceMask.getLeft() + 10, faceMask.getTop()), faceMask.getWidth() - 10, camHeight);
}

//--------------------------------------------------------------
void ofApp::nudgeMaskRightLeft(){
    faceMask.setWidth(faceMask.getWidth() - 10);
}

//--------------------------------------------------------------
void ofApp::nudgeMaskRightRight(){
    faceMask.setWidth(faceMask.getWidth() + 10);
}

//--------------------------------------------------------------
void ofApp::reset(){
    silence();
    grayBg.set(1.0); // set all pixels to black
    backgroundLearned = false;
    toggleGuiDraw.set(true);
    resetMask();
}

//--------------------------------------------------------------
void ofApp::silence(){
    pVolume.set(camWidth * 7 / 8, camHeight / 2);
}
