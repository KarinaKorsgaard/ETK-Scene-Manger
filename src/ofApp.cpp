#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    string path = "Sequences";
    ofDirectory dir(path); //relative dir?? -lol
    ofSetFrameRate(30);
    dir.listDir();
    vector<string> dropDownSize;
    for(int i = 0 ; i<dir.size();i++){
        vector<string> s = ofSplitString(dir.getPath(i),"/");
        dropDownSize.push_back(s.back());
    }
    
    setupGui = new ofxDatGui();
    ofxDatGuiTheme* theme;
    theme = new ofxDatGuiThemeWireframe();
    setupGui->setTheme(theme);
    setupGui->setPosition(ofGetWidth()/2-setupGui->getWidth()/2, ofGetHeight()/2-100);
    setupGui->addHeader(":: ETK scene builder ::");
    setupGui->addDropdown("SELECT EXISTING SEQUENCE",dropDownSize);
    setupGui->addButton("CREATE NEW SEQUENCE");
    setupGui->onDropdownEvent(this, &ofApp::onDropdownEvent);
setupGui->onButtonEvent(this, &ofApp::onButtonEvent);
    

}

//--------------------------------------------------------------
void ofApp::update(){
    if(isSetup){
        myApp.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(isSetup){
        myApp.draw();
    }
}
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    // select existing
    if(e.target->is("SELECT EXISTING SEQUENCE")&&!isSetup){
        isSetup = true;
        myApp.dataFolder = "Sequences/"+e.target->getLabel();
        setupGui->setVisible(false);
        myApp.setup();
    }
}
void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    // select existing
    if(e.target->is("CREATE NEW SEQUENCE")&&!isSetup){
        isSetup = true;
        //setup from xml first. - fill subscenes after. Or setup scene with array of subscenes made in read xml.
        string newClipboardContent = ofSystemTextBoxDialog("type name of new folder");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        myApp.dataFolder = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        
        string path = "Sequences/"+ofxClipboard::paste();;
        ofDirectory dir(path);
        if(!dir.exists()){
            dir.createDirectory(path);
        }
        myApp.dataFolder = "Sequences/"+ofxClipboard::paste();
        setupGui->setVisible(false);
        myApp.setup();
    }
}
//void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
//{
//    // make new sequence
//    if (e.target->is("MAKE NEW SEQUENCE") && !isSetup){
//        string newClipboardContent = ofSystemTextBoxDialog("Type name of Sequence:");
//        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
//        string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
//        sequence.name = clipboardContent;
//        //sequence.xml.saveFile(clipboardContent+".xml");
//        guiInit->setVisible(false);
//        for(int i = 0 ;i<buts.size();i++){
//            sequence.buts.push_back(&buts[i]);
//        }
//        sequence.setup();
//    }
//}


//--------------------------------------------------------------


