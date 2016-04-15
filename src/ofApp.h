#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "scene.h"
#include "ofxClipboard.h"
#include "defines.h"
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include "myofApp.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

    myofApp myApp;
    ofxDatGui* setupGui;
    bool isSetup;
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onButtonEvent(ofxDatGuiButtonEvent e);
};
