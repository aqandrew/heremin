# heremin

A visual theremin, *here* in your webcam!

Homework 2: Kinect || Camera, for Art & Code & Interactivity.

![installation diagram](https://raw.githubusercontent.com/dawneraq/heremin/master/heremin_installation.png)

## Instructions

0. Make sure you have [ChucK](http://chuck.cs.princeton.edu/) and [Xcode](https://developer.apple.com/xcode/) installed.
1. Start miniAudicle and add heremin.ck to ChucK's Virtual Machine.
2. (Build/)Run hereminDebug with the "My Mac (64-bit)" Xcode scheme.
3. Sit at a normal laptop-using distance from the screen, in front of a relatively plain backdrop.
4. Press B to establish a background against which the current webcam frame is compared.
  - A user's hands are detected as blobs on the webcam via ofxOpenCV.
5. **This is a musical instrument!** Control volume with the height of your right hand, and pitch with the height of your left hand.
6. By default, the middle third of the camera is ignored during blob detection. You can adjust this masked region with the A, S, K, and L keys.
7. Press R to reset the background and mask settings.
