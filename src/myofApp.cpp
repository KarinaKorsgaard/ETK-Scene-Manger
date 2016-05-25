#include "myofApp.h"

//--------------------------------------------------------------
void myofApp::setup(){
    
//    //setup from xml first. - fill subscenes after. Or setup scene with array of subscenes made in read xml.
//    string newClipboardContent = ofSystemTextBoxDialog("type name of dataFolder, or make a new folder");
//    ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
//    dataFolder = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
//    
//    string path = dataFolder;
//    ofDirectory dir(path);
//    if(!dir.exists()){
//        dir.createDirectory("Sequences/"+path);
//    }
//    dataFolder="Sequences/"+dataFolder;
   // secondSetup();
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
    enabled = new ofxDatGuiThemeWireframe();
    setupGui->setTheme(enabled);
    setupGui->setPosition(ofGetWidth()/2-setupGui->getWidth()/2, ofGetHeight()/2-100);
    setupGui->addHeader(":: ETK scene builder ::");
    for(int i = 0; i<dropDownSize.size();i++){
        setupGui->addButton(dropDownSize[i]);
    }
    setupGui->addBreak();
   // setupGui->addDropdown("SELECT EXISTING SEQUENCE",dropDownSize);
    setupGui->addButton("MAKE NEW SEQUENCE");
   // setupGui->onDropdownEvent(this, &myofApp::onDropdownEvent);
    setupGui->onButtonEvent(this, &myofApp::onButtonEvent);
    
    
}

void myofApp::secondSetup(){
    fbo.allocate(RES_W,RES_H);
    fbo.begin();
    ofClear(0);
    fbo.end();
    font.setup("GT.ttf",1.0, 1024, true, 8, 1.0);
    syphon.setName("PREVIEW");
    
    string path = dataFolder+"/img";
    ofDirectory dirImg(path); //relative dir?? -lol
    
    dirImg.listDir();
    vector<string> dropDownSize;
    for(int i = 0 ; i<dirImg.size();i++){
        vector<string> s = ofSplitString(dirImg.getPath(i),"/");
        dropDownSize.push_back(s.back());
        imagesString.push_back(s.back());
    }
    //if(imageString.size())
    vector<int>breaks = {2,1,/*1,*/3,3,4,4,4,3};
    
    alignerStrings.push_back("matrixX");
    alignerStrings.push_back("matrixY");
    
    alignerStrings.push_back("resultFontSize");
    
    //alignerStrings.push_back("liveResultFontSize");
    
    alignerStrings.push_back("timerSize");
    alignerStrings.push_back("timerX");
    alignerStrings.push_back("timerY");
    
    alignerStrings.push_back("legendFontSize");
    alignerStrings.push_back("legendX");
    alignerStrings.push_back("legendY");
    
    alignerStrings.push_back("tekst1FontSize");
    alignerStrings.push_back("tekst1X");
    alignerStrings.push_back("tekst1Y");
    alignerStrings.push_back("collumWidth1");
    
    alignerStrings.push_back("tekst2FontSize");
    alignerStrings.push_back("tekst2X");
    alignerStrings.push_back("tekst2Y");
    alignerStrings.push_back("collumWidth2");
    
    alignerStrings.push_back("tekst3FontSize");
    alignerStrings.push_back("tekst3X");
    alignerStrings.push_back("tekst3Y");
    alignerStrings.push_back("collumWidth3");
    
    alignerStrings.push_back("chartFontSize");
    alignerStrings.push_back("top10chartX");
    alignerStrings.push_back("top10chartY");
    
    preset = { 1000,50,40,300,330,750, 40,50,230, 50,50,80,400, 30,200,880,400, 40,50,150,400,30,50,100};
   // preset2 = {100,10,10,100,100,100,30,40,40,40,40,40,40,40,100,10,10,100,100,100,30,40,40,40,40,40,40,40};
    for(int i = 0 ; i<alignerStrings.size();i++){
        alignerMap[alignerStrings[i]] = preset[i];
    }
    cout<<alignerMap.size()<<endl;
    
    alignerInts.resize(alignerStrings.size());
    
    colorSelector = new ofxDatGui;
   // imageSelector = new ofxDatGui;
   // imageSelector->addHeader("images");
    colorSelector->addHeader("colors");
    for(int i = 0 ; i< MAX_SUB_ANSWER;i++){
     //   imageSelector->addDropdown("I"+ofToString(i+1), dropDownSize);
        colorSelector->addColorPicker("C"+ofToString(i+1));
    }
    
    //imageSelector->onDropdownEvent(this, &myofApp::onDropdownEvent);
    colorSelector->onColorPickerEvent(this, &myofApp::onColorPickerEvent);
   // imageSelector->addFooter();
    colorSelector->addFooter();
    
    tableNamesGui = new ofxDatGui;
    tableNamesGui->addHeader("tableNames");
    for (int i = 0; i<12; i++) {
        tableNamesGui->addTextInput("TAB_NAME"+ofToString(i+1),"Laud "+ofToString(i+1));
        tableNames.push_back("Laud " +ofToString(i+1));
    }
    tableNamesGui->addFooter();
    tableNamesGui->onTextInputEvent(this, &myofApp::onTextInputEvent);
    
    scenes.clear();
    
    ofParameterGroup main;
    main.add(addVote.set("add vote",false));
    main.add(addQuiz.set("add quiz",false));
    main.add(addAssesment.set("add assesment",false));
    main.add(save.set("SAVE",false));
    mainGui.setup(main);
    
    loadFromXml();
    
    path = dataFolder+"/img";
    ofDirectory newDir(path); //relative dir?? -lol
    
    dirImg.listDir();
    //imgs = new ofImage[newDir.size()];
    for(int i = 0 ; i<dirImg.size();i++){
        vector<string> s = ofSplitString(dirImg.getPath(i),"/");
        ofImage img = *new ofImage;
        imgs.push_back(img);
        imgs[i].load(dataFolder+"/img/"+s.back());
        imageStrings.push_back(dataFolder+"/img/"+s.back());
        cout << "loaded " + dataFolder+"/img/"+s.back()<< endl;
    }
    
    //    ofLogVerbose();
    //    for(int i = 0 ; i<scenes.size();i++){
    //        cout<<"scene subs" + ofToString(scenes[i].subs.size())<<endl;
    //    }
    
    
    //    for(int i = 0; i<scenes.size();i++){
    //        ofParameter<bool> b;
    //        b.set("scene: "+ofToString(i+1),false);
    //        editExisting.push_back(b);
    //        main.add(editExisting[i]);
    //    }
    
    disabled = new ofxDatGuiThemeMidnight();
    enabled = new ofxDatGuiThemeWireframe();
    
    //scene Guis ...
    
    ofParameterGroup commons;
    
    //gui1.add(addVote.set("add subscene",false)); //do need a count of amount of subs!
    // commons.add(amountOfSubs.set("amount of subscenes",0,10,1));
    commons.add(globalTimer.set("globalTimer",0,0,60));
    commons.add(useGlobalAligners.set("Use Global Aligners", true));
   // commons.add(useNumbers.set("Use Numbers", false));
    ofParameterGroup commonsSub;
    commonsSub.setName("subscenes");
    
    DatSub = new ofxDatGui;
    for(int i = 0; i<MAX_SUB;i++){
        ofParameter<bool> b;
        b.set("subscene: "+ofToString(i+1),false);
        editExistingSub.push_back(b);
        commonsSub.add(editExistingSub[i]);
        
        DatSub->addHeader();
        DatSub->setWidth(200);
        
        DatSub->addButton("subscene: "+ofToString(i+1));
    }
    DatSub->onButtonEvent(this, &myofApp::onButtonEvent);
    
    
    ofParameterGroup mode1;
    mode1.setName("mode1");
    mode1.add(weighed.set("weighed vote",false));
    mode1.add(consentual.set("consensus vote",false));
    mode1.add(commons);
    //mode1.add(commonsSub);
    
    ofParameterGroup mode2;
    mode2.add(amountOfSubs.set("amount of subscenes",1,1,MAX_SUB));
    mode2.setName("mode2");
    mode2.add(commons);
    //mode2.add(commonsSub);
    
    ofParameterGroup mode3;
    mode3.setName("mode1");
    mode3.add(amountOfSubs.set("amount of subscenes",1,1,MAX_SUB));
    mode3.add(totalPoints.set("totalPoints",10,0,200));
    mode3.add(substractive.set("substractive",false));
    mode3.add(commons);
    //mode3.add(commonsSub);
    
    sceneGui1.setup(mode1);
    sceneGui2.setup(mode2);
    sceneGui3.setup(mode3);
    
    ofParameterGroup subCommons;
    subCommons.setName("subsceneEditor");
    subCommons.add(amountAnswerOptions.set("Amount of Answers",4,0,MAX_SUB_ANSWER));
    subCommons.add(useImages.set("Use Images", false));
    subCommons.add(useGlobalTimer.set("Use Global Timer", true));
    subCommons.add(localTimer.set("Timer for This Sub", 20,0,100)); //secTime
    subCommons.add(rightAnswer.set("right A(quiz)",1,1,MAX_SUB_ANSWER+1));
    subSceneGui.setup(subCommons);
    
    
    
    
    
    //        new ofxDatGuiThemeWireframe(),
    //        new ofxDatGuiThemeMidnight(),
    //        new ofxDatGuiThemeAqua(),
    //        new ofxDatGuiThemeCharcoal(),
    //        new ofxDatGuiThemeAutumn(),
    //        new ofxDatGuiThemeCandy()};
    //tIndex = 0;
    
    sceneTextEditorGui = new ofxDatGui;
    sceneTextEditorGui->addHeader();
    sceneTextEditorGui->setWidth(sceneGui1.getWidth());
    sceneTextEditorGui->addTextInput("TEKST_1"," global for all subscene.");
    sceneTextEditorGui->getTextInput("TEKST_1")->setHeight(70);
    sceneTextEditorGui->onTextInputEvent(this, &myofApp::onTextInputEvent);
    sceneTextEditorGui->setVisible(false);
    
    subsceneTextEditorGui = new ofxDatGui;
    subsceneTextEditorGui->addHeader();
    subsceneTextEditorGui->setWidth(mainGui.getWidth());
    subsceneTextEditorGui->addTextInput("TEKST_2","Headline: individual for each subscene.");
    subsceneTextEditorGui->addTextInput("TEKST_3","Question: individual for each subscene.");
    subsceneTextEditorGui->getTextInput("TEKST_2")->setHeight(70);
    subsceneTextEditorGui->getTextInput("TEKST_3")->setHeight(70);
    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        subsceneTextEditorGui->addTextInput(ofToString(1+i)+"A","answer");
    }
    subsceneTextEditorGui->onTextInputEvent(this, &myofApp::onTextInputEvent);
    subsceneTextEditorGui->setVisible(false);
    
    //aligners;
    ofParameterGroup positions;
    ofParameterGroup fontsizes;
    ofParameterGroup aligners;
    
    DatSlider = new ofxDatGui;
    DatSlider->addHeader();
    int breakCounter = breaks[0];
    int counter = 0;
    for(int i = 0; i<alignerStrings.size();i++){
        int maxValue = 200;
        if(ofIsStringInString(alignerStrings[i], "Size"))maxValue = 300;
        else if(ofIsStringInString(alignerStrings[i], "Y"))maxValue = RES_H;
        else maxValue = RES_W;
            
        DatSlider->addSlider(alignerStrings[i], 0, maxValue, preset[i]);
        if(i+1>=breakCounter){
            DatSlider->addBreak();
            counter++;
            breakCounter+=breaks[counter];
        }
        DatSlider->getSlider(alignerStrings[i])->setHeight(20);
        DatSlider->getSlider(alignerStrings[i])->setPrecision(0);
    }
    DatSlider->onSliderEvent(this, &myofApp::onSliderEvent);

    
    // mainGui.setWidthElements(150);
    // globalAligners.setWidthElements(200);
    DatSub->setWidth(sceneGui1.getWidth());
    sceneTextEditorGui->setWidth(sceneGui1.getWidth());
    subsceneTextEditorGui->setWidth(sceneGui1.getWidth());
    
    //sceneGui1.loadFont("Verdana.ttf",8);
    //sceneGui1.setUseTTF(true);
    
    sceneGui1.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
    sceneGui2.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
    sceneGui3.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
    
    subSceneGui.setPosition(mainGui.getWidth()*2+40,mainGui.getPosition().y);
    
    sceneTextEditorGui->setPosition(sceneGui1.getPosition().x,sceneGui1.getPosition().y+sceneGui1.getHeight()+20);
    DatSub->setPosition(sceneTextEditorGui->getPosition().x,sceneTextEditorGui->getHeight()+sceneTextEditorGui->getPosition().y+20);
    subsceneTextEditorGui->setPosition(subSceneGui.getPosition().x,subSceneGui.getPosition().y+subSceneGui.getHeight()+30);
    
    DatSlider->setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
    
    colorSelector->setPosition(DatSub->getPosition().x, DatSub->getPosition().y+DatSub->getHeight()+20);
   // imageSelector->setPosition(subsceneTextEditorGui->getPosition().x, subsceneTextEditorGui->getPosition().y +subsceneTextEditorGui->getHeight()+20 );
    tableNamesGui->setPosition(mainGui.getPosition().x, ofGetHeight()-(tableNamesGui->getHeight()+40));
    
    colorSelector->setWidth(300);
   // imageSelector->setWidth(300);
    tableNamesGui->setWidth(300);
    
    if(scenes.size()>0){
        editExisting[0]=true;
        updateGuiToSceneAligners(scenes[0].useGlobalAligners);
        setGuiToSceneValues(0);
        setSubGuiToSubValues(0);
    }
    for(int i = 0; i<tableNames.size();i++){
        tableNamesGui->getTextInput("TAB_NAME"+ofToString(i+1))->setText(tableNames[i]);
        
    }
   //if(scenes.size()>0)updateGuiToSceneAligners(scenes[0]);

}

//--------------------------------------------------------------
void myofApp::update(){
    
    if(setupNow){
        setupNow = false;
        secondSetup();
    
    }
    
    if(isSetup){
    ofSetWindowTitle(dataFolder + " framerate:" + ofToString(ofGetFrameRate(),0));
    // main gui controls: adding and selecting sceenes;
    if(save){
        saveToXml();
        save = false;
    }
    if(addVote){
        string newClipboardContent = ofSystemTextBoxDialog("Name of Vote");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        addVote = false;
        if(!isNameTaken(clipboardContent)){
            addNewScene(0,clipboardContent);
        }else{
            ofSystemAlertDialog("name is already taken");
        }
    }
    if(addQuiz){
        string newClipboardContent = ofSystemTextBoxDialog("Name of Quiz");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        addQuiz = false;
        if(!isNameTaken(clipboardContent)){
            addNewScene(1,clipboardContent);
        }else{
            ofSystemAlertDialog("name is already taken");
        }
    }
    if(addAssesment){
        string newClipboardContent = ofSystemTextBoxDialog("Name of Assesment");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        addAssesment = false;
        if(!isNameTaken(clipboardContent)){
            addNewScene(2,clipboardContent);
        }else{
            ofSystemAlertDialog("name is already taken");
        }
    }
    
    showSome = false;
    int previousValue = whichScene;
    bool somethingChanged=false;
    int whichChanged=0;
    for(int i = 0; i<editExisting.size();i++){
        int tog=0;
        if(toggleScene[i])tog=1;
        int edi =0;
        if(editExisting[i])edi=1;
        if(tog!=edi && edi == 1){
            somethingChanged=true;
            whichChanged=i;
        }
    }
    if(somethingChanged){
        whichScene=whichChanged;
        if(scenes[whichScene].mode == 0){
            b_sceneGui1 = true;
            b_sceneGui2 = false;
            b_sceneGui3 = false;
           // imageSelector->setVisible(true);
            sceneTextEditorGui->setVisible(true);
            subsceneTextEditorGui->setVisible(true);
            
        }
        else if(scenes[whichScene].mode == 1){
            b_sceneGui1 = false;
            b_sceneGui2 = true;
            b_sceneGui3 = false;
           // imageSelector->setVisible(true);
            sceneTextEditorGui->setVisible(true);
            subsceneTextEditorGui->setVisible(true);
            
        }
        else if(scenes[whichScene].mode == 2){
            b_sceneGui1 = false;
            b_sceneGui2 = false;
            b_sceneGui3 = true;
            sceneTextEditorGui->setVisible(true);
            subsceneTextEditorGui->setVisible(true);
        }

        whichSub=0;
        updateGuiToSceneAligners(scenes[whichScene].useGlobalAligners);
        setGuiToSceneValues(whichScene);
        setSubGuiToSubValues(whichSub);
    }
        //previousValue=whichScene;
    
    for(int i = 0; i<editExisting.size();i++){
        if(i!=whichScene)editExisting[i]=false;
        toggleScene[i]=editExisting[i];
    }


    if(whichScene<scenes.size())
    {
        updateScene(&scenes[whichScene]);
        updateSubScene(&scenes[whichScene].subs[whichSub]);
    }
    
    if(globalAlignerToggle != useGlobalAligners){

        updateGuiToSceneAligners(useGlobalAligners);
    }
//    if(globalAlignerToggle && !useGlobalAligners){
//
//        updateGlobalAligners();
//    }
    globalAlignerToggle=useGlobalAligners;
    
    if(whichScene<scenes.size()){
        if(whichSub<scenes[whichScene].subs.size() ){
            preview(scenes[whichScene],scenes[whichScene].subs[whichSub]);
        }
    }
    ofFill();
    syphon.publishTexture(&fbo.getTexture());
        
    }
}

void myofApp::onSliderEvent(ofxDatGuiSliderEvent e){
    for(int i = 0 ; i< alignerStrings.size();i++){
        if(e.target->getLabel()==alignerStrings[i]){
            if(useGlobalAligners)alignerMap[alignerStrings[i]]=e.target->getValue();
            //cout<<alignerInts[i]<<endl;
            if(!useGlobalAligners)scenes[whichScene].alignerMap[alignerStrings[i]]=e.target->getValue();
            
        }
        //if(!useGlobalAligners)scenes[whichScene].aligners[i]=DatSlider->getSlider(alignerStrings[i])->getValue();
        //if(useGlobalAligners)alignerInts[i]=DatSlider->getSlider(alignerStrings[i])->getValue();
    }
}

//--------------------------------------------------------------
void myofApp::draw(){
    if(isSetup){
    ofBackground(0, 0, 50);
    mainGui.draw();
    
    if(whichScene<scenes.size()){
        subSceneGui.setName("sub "+ofToString(whichSub+1));
        subSceneGui.draw();
        globalAligners.draw();
        if(b_sceneGui1){
            sceneGui1.setName(scenes[whichScene].name);
            sceneGui1.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
            sceneGui1.draw();
            ofSetColor(0);
            //float h = 18*(MAX_SUB-scenes[whichScene].amountOfSubs);
            //ofDrawRectangle(sceneGui1.getPosition().x, 18*MAX_SUB-(sceneGui1.getPosition().y+h)+50, sceneGui1.getWidth(), h+50);
            
        }
        if(b_sceneGui2){
            sceneGui2.setName(scenes[whichScene].name);
            sceneGui2.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
            sceneGui2.draw();
        }
        
        
        
        
        if(b_sceneGui3){
            sceneGui3.setName(scenes[whichScene].name);
            sceneGui3.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
            sceneGui3.draw();
        }
        //    ofSetColor(0, 0, 50);
        //    ofDrawRectangle(sceneGui1.getPosition().x-1, sceneGui1.getPosition().y + 18*7 +18*amountOfSubs, sceneGui1.getWidth()+2, 19*(MAX_SUB-amountOfSubs)+3);
        //
        ofSetColor(255);
        ofDrawBitmapString("editing" +ofToString(whichScene+1) + " "+ofToString(whichSub+1) + " "+ofToString(scenes.size())+ " "+ofToString(scenes[whichScene].subs.size()), 10, ofGetHeight()-20);
        
        
        for(int i = 0 ; i<MAX_SUB;i++){
            if(i!=whichSub)DatSub->getButton("subscene: "+ofToString(i+1))->setTheme(disabled);
            if(i==whichSub)DatSub->getButton("subscene: "+ofToString(i+1))->setTheme(enabled);
        }
    }
    if(whichScene<scenes.size() && imgs.size()>0){
        if(whichSub<scenes[whichScene].subs.size()){
            if(scenes[whichScene].subs[whichSub].useImages){
                imageSelector(mX,mY);
            }
        }
    }
    }
    //for(int i = 0 ; i<scenes.size();i++)cout<<scenes[i].name<<endl;
    // subsceens...
}
//--------------------------------------------------------------

void myofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        if(e.target->getLabel() == ofToString(1+i)+"A")scenes[whichScene].subs[whichSub].setAnswerOption(e.target->getText(), i);
    }
    if(e.target->getLabel() == "TEKST_2")scenes[whichScene].subs[whichSub].headline =e.target->getText();
    if(e.target->getLabel() == "TEKST_3")scenes[whichScene].subs[whichSub].question =e.target->getText();
    
    if(e.target->getLabel() == "TEKST_1")scenes[whichScene].explanation =e.target->getText();
    
  //  cout << "onTextInputEvent: " << e.target->getLabel() << " " << e.target->getText() << endl;
    
    for(int i = 0; i<tableNames.size();i++){
        if(e.target->getLabel()=="TAB_NAME"+ofToString(i+1)){
            tableNames[i]=e.target->getText();
            height=0;
            
        }
    }
}


//--------------------------------------------------------------

void myofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if(isSetup){
        // Scenee guis: editing and selecting from subs !
        //editing all things with scene
        int previousValue = whichSub;
        for(int i = 0 ; i<MAX_SUB;i++){
            e.target->setTheme(disabled);
            if(e.target->getLabel()=="subscene: "+ofToString(i+1)){
                whichSub = i;
                
                if(previousValue!=whichSub)setSubGuiToSubValues(whichSub);
            }
        }
        
        
        updateSubScene(&scenes[whichScene].subs[whichSub]);
        subSceneGui.setName(scenes[whichScene].name + ": subscene :" +ofToString(whichScene+1));
        subSceneGui.setPosition(subSceneGui.getPosition()); //changing the name fix
    }
    if(!isSetup){
        // select existing
        if(!e.target->is("MAKE NEW SEQUENCE")){
            isSetup = true;
            setupNow = true;
            dataFolder = "Sequences/"+e.target->getLabel();
            setupGui->setVisible(false);
        }
        
        if(e.target->is("MAKE NEW SEQUENCE")){
            isSetup = true;
            setupNow = true;
            
            string newClipboardContent = ofSystemTextBoxDialog("name of new folder");
            ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
            dataFolder = "Sequences/"+ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
            
            
            ofDirectory dir(dataFolder);
            if(!dir.exists()){
                dir.createDirectory(dataFolder);
            }
            //dataFolder = "Sequences/"+e.target->getLabel();
            setupGui->setVisible(false);
        }
        
    }


}
//
//void myofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
//    //cout << e.target->getIndex();
//    
////    if(isSetup){
////        for(int u = 0 ; u<imagesString.size();u++){
////            if(e.target->getLabel()==imagesString[u]){
////                
////                scenes[whichScene].subs[whichSub].images[e.target->getIndex()-1] = u;
////            }
////        }
////    }
//    
//    if(!isSetup){
//        // select existing
//        if(e.target->is("SELECT EXISTING SEQUENCE")&&!isSetup){
//            isSetup = true;
//            setupNow = true;
//            dataFolder = "Sequences/"+e.target->getLabel();
//            setupGui->setVisible(false);
//        }
//
//    }
//}

void myofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){
    for (int i = 0; i<MAX_SUB_ANSWER; i++) {
        if(e.target->getLabel()=="C"+ofToString(i+1) && whichScene<scenes.size()){
            scenes[whichScene].colors[i].set(e.target->getColor());
        }
//        scenes[scene].colors[i].set(colorSelector->getColorPicker("C"+ofToString(i+1))->getColor());
    }
}

// for triggering when whichScene or which Sub scene is called!! - SubScene will edit my textstuff...
void myofApp::setGuiToSceneValues(int scene){
    
    amountOfSubs = scenes[scene].amountOfSubs;
    globalTimer = scenes[scene].globalTimer;
    useGlobalAligners =scenes[scene].useGlobalAligners;
    
    weighed=scenes[scene].weighed;
    totalPoints=scenes[scene].totalPoints;
    substractive=scenes[scene].substractive;
    consentual=scenes[scene].consentual;
    
    sceneTextEditorGui->getTextInput("TEKST_1")->setText( scenes[whichScene].explanation );
    
    setSubGuiToSubValues(whichSub); // setting colors
    for (int i = 0; i<MAX_SUB_ANSWER; i++) {
        colorSelector->getColorPicker("C"+ofToString(i+1))->setColor(scenes[scene].colors[i]);
       // scenes[scene].colors[i].set(colorSelector->getColorPicker("C"+ofToString(i+1))->getColor());
    }
    
 
    // text : TEXT1 and TEXT2 - Scene: text1,text2
}
void myofApp::setSubGuiToSubValues(int sub){
    // text : questions
    // answers : - MAX_SUB_ANSWERS
    
    amountAnswerOptions = scenes[whichScene].subs[sub].amountAnswerOptions;
    useImages = scenes[whichScene].subs[sub].useImages;
    useGlobalTimer = scenes[whichScene].subs[sub].useGlobalTimer;
    localTimer = scenes[whichScene].subs[sub].localTimer;
    rightAnswer = scenes[whichScene].subs[sub].rightAnswer;
    
    subsceneTextEditorGui->getTextInput("TEKST_2")->setText( scenes[whichScene].subs[sub].headline );
    subsceneTextEditorGui->getTextInput("TEKST_3")->setText( scenes[whichScene].subs[sub].question );
    
    for(int i = 0 ; i<scenes[whichScene].subs[sub].answerOptions.size();i++){
        subsceneTextEditorGui->getTextInput(ofToString(1+i)+"A")->setText(scenes[whichScene].subs[sub].answerOptions[i]);
    }
    for(int i = 0 ; i<scenes[whichScene].subs[sub].images.size();i++){
        int thisSubImage = scenes[whichScene].subs[sub].images[i];
      //  cout <<thisSubImage<<endl;
      //  cout <<imageStrings.size()<<endl;
        if(imageStrings.size()>thisSubImage){
        //cout<<imageSelector->getDropdown("I"+ofToString(i+1))->getChildAt(thisSubImage)<<endl;
        //imageSelector->getDropdown("I"+ofToString(thisSubImage))->setLabel(imagesString[thisSubImage]);
         //   imageSelector->getDropdown("I"+ofToString(thisSubImage+1))->setLabel(imageStrings[thisSubImage]);
        }
    }
}

void myofApp::updateScene(Scene* s){
    if(s->mode==0)amountOfSubs=1;
    if(s->mode!=1)rightAnswer=0;
    if(s->mode==2)amountAnswerOptions=0;
    s->amountOfSubs = amountOfSubs;
    s->globalTimer = globalTimer;
    //s->useNumbers = useNumbers;
    s->useGlobalAligners = useGlobalAligners;
    
    if(s->useGlobalAligners){
        DatSlider->getHeader()->setName("GLOBAL ALIGNERS");
        DatSlider->setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
//       // DatSlider->setTheme(enabled);
//        //globalAligners.setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
//        //globalAligners.setFillColor(ofColor(0,50,0));
//       // globalAligners.setPosition(mainGui.getPosition().x,mainGui.getHeight()+40);
      //  updateGlobalAligners();
    }
    else if(!s->useGlobalAligners){
//       // DatSlider->setTheme(disabled);
       DatSlider->getHeader()->setName("LOCAL ALIGNERS FOR:" + s->name);
        DatSlider->setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
//        //globalAligners.setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
//        //globalAligners.setFillColor(ofColor(50,0,0));
//       // globalAligners.setPosition(sceneGui1.getPosition().x,sceneGui1.getHeight()+40);
       // updateLocalAligners(s);
    }//add headline
    
   
    s->weighed = weighed;
    s->consentual = consentual;
    
    s->substractive = substractive;
    s->totalPoints = totalPoints;
    
    
    for(int i = 0; i<MAX_SUB;i++){
        if(i<amountOfSubs)DatSub->getButton("subscene: "+ofToString(i+1))->setVisible(true);
        if(i>=amountOfSubs)DatSub->getButton("subscene: "+ofToString(i+1))->setVisible(false);
    }
    if(s->mode == 0 || s->mode == 1){
        for(int i = 0; i<MAX_SUB_ANSWER;i++){
            if(i<amountAnswerOptions)colorSelector->getColorPicker("C"+ofToString(i+1))->setVisible(true);
            if(i>=amountAnswerOptions)colorSelector->getColorPicker("C"+ofToString(i+1))->setVisible(false);
        }
    }
    if(s->mode == 2){
        for(int i = 0; i<MAX_SUB_ANSWER;i++){
            if(i<2)colorSelector->getColorPicker("C"+ofToString(i+1))->setVisible(true);
            if(i>=2)colorSelector->getColorPicker("C"+ofToString(i+1))->setVisible(false);
        }
    }

}

void myofApp::updateSubScene(SubScene* s){
    if(s->mode==2){
        amountAnswerOptions=0;
        useImages = false;
       // useNumbers = true;
    }
    if(rightAnswer>amountAnswerOptions)rightAnswer=amountAnswerOptions;
    
    if(s->mode==0){
        useGlobalTimer=true;
    }
    s->amountAnswerOptions = amountAnswerOptions;
    s->useImages = useImages;
    
    if(s->mode==1&&rightAnswer>amountAnswerOptions)rightAnswer=amountAnswerOptions;
    s->rightAnswer = rightAnswer;
    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        if(i<amountAnswerOptions)subsceneTextEditorGui->getTextInput(ofToString(i+1)+"A")->setVisible(true);
        if(i>=amountAnswerOptions)subsceneTextEditorGui->getTextInput(ofToString(i+1)+"A")->setVisible(false);
        
       // if(i<amountAnswerOptions)imageSelector->getDropdown("I"+ofToString(i+1))->setVisible(true);
       // if(i>=amountAnswerOptions)imageSelector->getDropdown("I"+ofToString(i+1))->setVisible(false);
    }
    
    if(useGlobalTimer){
        s->localTimer = globalTimer;
        localTimer = globalTimer;
    }else{
        s->localTimer = localTimer;
    }
}


//--------------------------------------------------------------
void myofApp::addNewScene(int mode, string name){
    Scene copyFrom;
    if(scenes.size()>0){
        copyFrom = scenes.back();
    }else{
        for(int i = 0; i<MAX_SUB;i++){
            ofColor c;
            copyFrom.colors.push_back(c);
        }
    }
    
    Scene s = *new Scene;
    scenes.push_back(s);
    

    scenes.back().setup(mode,name,copyFrom);
    for(int i = 0 ; i<alignerStrings.size();i++){
       // scenes.back().aligners.push_back(preset[i]);
        scenes.back().alignerMap[alignerStrings[i]] = preset[i];
      //  else scenes.back().alignerMap[alignerStrings[i]] = preset2[i];
    }

    
    ofParameter<bool> b;
    b.set("scene: "+ofToString(editExisting.size()+1),false);
    editExisting.push_back(b);
    toggleScene.push_back(false);
    mainGui.add(editExisting.back());
}

//--------------------------------------------------------------
void myofApp::keyPressed(int key){
    if(key == OF_KEY_COMMAND){cmd=true;
    cout<<"cmd"<<endl;
    }
    cout <<key<<endl;
    if(key=='s' && cmd){
        saveToXml();
        cout<<"saved"<<endl;
    }
}

void myofApp::imageSelector(int x,int y){
    int ux=subSceneGui.getPosition().x;
    int uy=subsceneTextEditorGui->getPosition().y+subsceneTextEditorGui->getHeight()+20;
    
    float size=subsceneTextEditorGui->getWidth()/imgs.size();
    ofPushMatrix();
    ofTranslate(subSceneGui.getPosition().x,subsceneTextEditorGui->getPosition().y+subsceneTextEditorGui->getHeight()+20);
    
    for(int u = 0; u<imgs.size();u++){
        for(int i = 0; i<amountAnswerOptions;i++){
            
            //mechanics
            if(x>ux+u*size && x<ux+(1+u)*size &&
               y>uy+i*size && y<uy+(1+i)*size){
                scenes[whichScene].subs[whichSub].images[i]=u;
               // cout<<ofToString(u)+" "+ofToString(scenes[whichScene].subs[whichSub].images[i])<<endl;
            }

            ofSetColor(255);
            ofFill();
            imgs[u].draw(u*size,i*size,size,size);
            if(scenes[whichScene].subs[whichSub].images[i]==u){
                ofNoFill();
                ofSetLineWidth(3);
                ofDrawRectangle(u*size,i*size,size,size);
            }
        }
    }
    ofPopMatrix();
    
}

void myofApp::preview(Scene s,SubScene sub){
    
    std::map <string, int> al;
    if(!s.useGlobalAligners){
        al = s.alignerMap;
    }
    if(s.useGlobalAligners){
        al = alignerMap;
    }
    //int p = 0.5;
    fbo.begin();
    ofClear(0);
    // cout<<sub.useImages<<endl;
    ofPushMatrix();
    //ofScale(0.5f, 0.5f);
    
    ofBackground(0, 195, 0);
    //matrix;
    
    ofFill();
    ofPushMatrix();
    ofTranslate(al["matrixX"], al["matrixY"]);
    
    int grid = 0;
    for(int i = 0 ; i<12;i++){
        grid = i/4 * 20;
        for (int u = 0; u<6; u++) {
            
            if(sub.amountAnswerOptions>0){
                int col = (i+u)%sub.amountAnswerOptions;
                //colorSelector->getColorPicker("C"+ofToString(col))->getColor();
                ofColor theColor;
                theColor.setHex(s.colors[col].getHex());
                ofSetColor(theColor);
                ofDrawRectangle(i*70+grid, u*70, 70, 70);
                if (sub.useImages && sub.images[col]<imgs.size()) {
                    ofSetColor(255);
                    imgs[sub.images[col]].draw(i*70+grid, u*70,70,70);
                }
            }
        }
    }
    ofPopMatrix();
    
    if(s.mode == 2){ //assesment bs
        
        // ofTranslate(al["matrixX"], al["matrixY"]);
        
        grid = 0;
        for(int i = 0 ; i<12;i++){
            grid = i/4 * 20;
            for (int u = 0; u<6; u++) {
                ofPushMatrix();
                ofTranslate(70*i+al["matrixX"] + grid, 70*u+al["matrixY"]);
                int alpha = ofMap(u+i, 0, 12+6, 0, 255);
                
                ofColor theColor;
                theColor.setHex(s.colors[0].getHex());
                ofSetColor(theColor,alpha);
                
                ofDrawRectangle(0,0, 70, 70);
                
                theColor.setHex(s.colors[1].getHex());
                ofSetColor(theColor,255-alpha);
                
                ofDrawRectangle(0,0, 70, 70);
                
                ofSetColor(255);
                string let = ofToString(u+i);
                ofRectangle rect = font.getBBox("100", 40+10, 0, 0);
                //font.draw(let, fontSize, (70/2)-rect.width/2, (font.getLineHeight()*fontSize/2)+(70/2)-rect.height/2);
                int x = 0;
                int y = 0;
                // ofTranslate(i*70+grid, u*70+grid);
                if(s.substractive){
                    x =10;
                    y =rect.height+10;
                }else if(!s.substractive){
                    rect = font.getBBox(let, 40+10, 0, 0);
                    x = (70/2) - rect.width/2;
                    y = rect.height/2 - (70/2) + 70;
                }
                font.draw(let, 40+10, x, y);
                
                if(s.substractive){
                    ofRectangle smrect = font.getBBox(ofToString( s.totalPoints ), 25, 0, 0);
                    font.draw(ofToString( s.totalPoints-(u+i) ), 25, (rect.width+10)-smrect.width, rect.height*2+8);
                    
                    ofSetLineWidth(2);
                    ofDrawLine( 10,14+rect.height, 60,14+rect.height);
                }
                ofPopMatrix();
            }
        }
        
    }
    
    
        ofSetColor(255);
        //headline
        drawColumn(sub.headline, al["tekst2X"], al["tekst2Y"], al["tekst2FontSize"], al["collumWidth2"]);
    
        //explanation
        drawColumn(s.explanation, al["tekst1X"], al["tekst1Y"], al["tekst1FontSize"], al["collumWidth1"]);
        
        //question
        drawColumn(sub.question, al["tekst3X"], al["tekst3Y"], al["tekst3FontSize"], al["collumWidth3"]);
        
        //timer
    int time ;
    if(sub.useGlobalTimer)time = s.globalTimer;
    else time = sub.localTimer;
        font.draw(ofToString(time), al["timerSize"], al["timerX"], al["timerY"]);
        
        //legend
        ofPushMatrix();
        ofTranslate(al["legendX"], al["legendY"]);
        for(int i = 0 ; i<sub.amountAnswerOptions;i++){
            ofSetColor(255);
            font.draw(sub.answerOptions[i], al["legendFontSize"], 70, i * 60+25);
            ofSetColor(s.colors[i]);
            ofDrawRectangle(0, 60*i, 60, 60);
            ofSetColor(255);
            ofNoFill();
            ofDrawRectangle(0, 60*i, 70*12+40, 60);
            ofFill();
        }
        ofPopMatrix();
        
        //result
        ofSetColor(255);
        ofRectangle rectt = font.getBBox("result - always in the middle", al["resultFontSize"], 0, 0);
        font.draw("result - always in the middle", al["resultFontSize"], (RES_W/2)-(rectt.width/2), RES_H/2-(rectt.height/2));
      //  font.draw("liveResult fontsize", al[7], (RES_W/2)-(rectt.width/2), RES_H/2-(rectt.height/2)+40);
        
        //live results;
        ofSetColor(255);
        font.draw("LIVE RESULTS", al["liveResultFontSize"], al["legendX"], al["legendY"]);
        
        ofRectangle name = font.getBBox("CHART, RESULT", al["chartFontSize"], 0,0);
        for(int i = 0; i <3;i++){
            int x = al["top10chartX"];
            int y = al["top10chartY"];
            ofSetColor(255);
            
            font.draw("#"+ofToString(i+1), al["chartFontSize"], x, y+(name.height+15)*i);
            font.draw("CHART, RESULT", al["chartFontSize"], x+200, y+(name.height+15)*i);
            font.draw(ofToString(i+1*2), al["chartFontSize"], x+200*2+20+name.width, y+(name.height+15)*i);
        }
        
    
    fbo.end();
        
}

void myofApp::saveToXml(){
    
        xml = *new ofxXmlSettings;
        xml.loadFile(dataFolder+"/data.xml");
        //xml.loadFile("Sequences/"+name+".xml");
    
        xml.clear();
        xml.addTag("document");
        xml.pushTag("document");
    
    setGlobalAlignersToXml(&xml);
    
    string table;
    for(int i= 0; i<tableNames.size();i++){
        if(i<tableNames.size()-1){table.append(tableNames[i]+"//");}
        else {table.append(tableNames[i]);}
    }
    //cout<<table<<endl;
    xml.addTag("tableNames");
    xml.setValue("tableNames", table);
    
    // mising table names, images and wrong image
    
  //  cout << ofToString(scenes.size()) + " this many scenes"<<endl;
    for(int i = 0; i<scenes.size(); i++){
            xml.addTag("scene");
            xml.pushTag("scene",i);
            xml.addTag("file");
            xml.setValue("file",scenes[i].name+".xml");
            xml.popTag();
        }
    xml.popTag();
    xml.saveFile(dataFolder+"/data.xml");
    
    for(int i = 0 ; i<scenes.size();i++){
        
        xml.loadFile(dataFolder+"/"+scenes[i].name+".xml");
        xml.clear();
        xml.addTag("document");
        xml.pushTag("document");
        
        if(scenes[i].mode == 0){
            xml.addTag("vote");
            xml.pushTag("vote");
        }
        if(scenes[i].mode == 1){
            xml.addTag("quiz");
            xml.pushTag("quiz");
        }
        if(scenes[i].mode == 2){
            xml.addTag("assesment");
            xml.pushTag("assesment");
        }
        if(!scenes[i].useGlobalAligners){
            setSceneAlignersToXml(scenes[i],&xml);
        }
        if(scenes[i].globalTimer>0){
            xml.addTag("globalTime");
            xml.setValue("globalTime",scenes[i].globalTimer);
        }
        
        xml.addTag("explanation");
        xml.setValue("explanation",scenes[i].explanation);
        
       // xml.addTag("explanation");
       // xml.setValue("explanation",scenes[i].explanation);
        
        //---------------vote-------------------------------------------------------------------------
        if(scenes[i].mode == 0){
            if(scenes[i].weighed)xml.addTag("weighed");
            else if(scenes[i].consentual)xml.addTag("consentual");
            else xml.addTag("majority");
            
            xml.addTag("headline");
            xml.setValue("headline",scenes[i].subs[0].headline);
            
            xml.addTag("question");
            xml.pushTag("question");
            xml.addTag("headline");
            xml.setValue("headline",scenes[i].subs[0].headline);
            if(!scenes[i].subs[0].useGlobalTimer){
                xml.addTag("secTime");
                xml.setValue("secTime",scenes[i].subs[0].localTimer);
            }
            xml.addTag("q");
            xml.setValue("q",scenes[i].subs[0].question);//ATTENTION
            
            for(int p = 0 ; p<scenes[i].subs[0].amountAnswerOptions;p++){
                xml.addTag("a");
                xml.setValue("a",scenes[i].subs[0].answerOptions[p],p);
                xml.pushTag("a",p);
                xml.addTag("color");
                xml.setValue("color",scenes[i].colors[p].getHex()); // ATTENTION
                if(scenes[i].subs[0].useImages){
                    xml.addTag("img");
                    xml.setValue("img",scenes[i].subs[0].images[p]); // ATTENTION
                }
                xml.popTag();//a
            }
            xml.popTag();

        }
        // ---------------------quiz! -----------------------
        if(scenes[i].mode == 1){
            for(int u = 0 ; u<scenes[i].amountOfSubs;u++){
                xml.addTag("question");
                xml.pushTag("question",u);
                xml.addTag("headline");
                xml.setValue("headline",scenes[i].subs[u].headline);
                if(!scenes[i].subs[u].useGlobalTimer){
                    xml.addTag("secTime");
                    xml.setValue("secTime",scenes[i].subs[u].localTimer);
                }
                xml.addTag("q");
                xml.setValue("q",scenes[i].subs[u].question);//ATTENTION
                
                for(int p = 0 ; p<scenes[i].subs[u].amountAnswerOptions;p++){
                    xml.addTag("a");
                    xml.setValue("a",scenes[i].subs[u].answerOptions[p],p);
                    xml.pushTag("a",p);
                    xml.addTag("color");
                    xml.setValue("color",scenes[i].colors[p].getHex()); // ATTENTION
                    if(scenes[i].subs[u].useImages){
                        xml.addTag("img");
                        xml.setValue("img",scenes[i].subs[u].images[p]); // ATTENTION
                    }
//                    if(!scenes[i].subs[u].useGlobalTimer){
//                        xml.addTag("secTime");
//                        xml.setValue("secTime",scenes[i].subs[u].localTimer); // ATTENTION
//                    }
                    xml.popTag();
                }
                xml.popTag();
            }
            xml.addTag("rightAnswers");
            xml.pushTag("rightAnswers");
            for(int u = 0; u<scenes[i].amountOfSubs;u++){
                xml.addTag("a");
                xml.setValue("a",scenes[i].subs[u].rightAnswer,u);
                
            }
            xml.popTag();
        }
        
        
        // ---------------------assesment! -----------------------
        if(scenes[i].mode == 2){

            if(scenes[i].substractive)xml.addTag("subtractive");
            xml.addTag("number");
            xml.addTag("totalPoints");
            xml.setValue("totalPoints",scenes[i].totalPoints);
            
            xml.addTag("color");
            xml.pushTag("color");
            xml.addTag("c");
            xml.addTag("c");
            xml.setValue("c",scenes[i].colors[0].getHex(),0); //ATTENTION
            xml.setValue("c",scenes[i].colors[1].getHex(),1);
            xml.popTag(); //color
            
            
            for(int u = 0 ; u<scenes[i].amountOfSubs;u++){
                xml.addTag("question");
                xml.pushTag("question",u);
                xml.addTag("headline");
                xml.setValue("headline",scenes[i].subs[u].headline);
                if(!scenes[i].subs[u].useGlobalTimer){
                    xml.addTag("secTime");
                    xml.setValue("secTime",scenes[i].subs[u].localTimer);
                }
                xml.addTag("q");
                xml.setValue("q",scenes[i].subs[u].question);//ATTENTION
                //no images for assesment
                xml.popTag();
            }
            
        }
        
        xml.popTag(); // assesment/quiz/vote
        xml.popTag(); // document;
        xml.saveFile(dataFolder+"/"+scenes[i].name+".xml");
    }
}

void myofApp::loadFromXml(){
    
    xml = *new ofxXmlSettings;
    xml.clear();
    if(xml.loadFile(dataFolder+"/data.xml"))cout<<"loaded "+ dataFolder+"/data.xml"<<endl;
    //xml.loadFile("Sequences/"+name+".xml");
    
    
    xml.pushTag("document");
    setGlobalAlignersFromXml(xml);
        
    for(int i = 0; i<xml.getNumTags("scene"); i++){
        xml.pushTag("scene",i);
        sceneNames.push_back(ofSplitString(xml.getValue("file", ""),".")[0]); //cut of .xml
        cout<<"scenes "+ sceneNames[i]<<endl;
        xml.popTag();
    }
//    for(int i = 0; i<xml.getNumTags("img"); i++){
//        cout<<"images "+ ofToString(xml.getNumTags("img"))<<endl;
//        imagesString.push_back(xml.getValue("img", "",i));
//    }
    
    vector<string> tab= ofSplitString(xml.getValue("tableNames", ""), "//");
    for(int i = 0; i<12;i++){
        if(i<tab.size())tableNames[i]=(ofSplitString(xml.getValue("tableNames", ""), "//")[i]);
    }
    xml.popTag();

    xml.clear();
    
    for(int i = 0; i<sceneNames.size();i++){
        xml.clear();
        
        if(xml.loadFile(dataFolder+"/"+sceneNames[i]+".xml")){
            cout<<"loaded "+ dataFolder+"/"+sceneNames[i]+".xml"<<endl;

        if(xml.tagExists("document")){
         
            xml.pushTag("document");
            

            bool loadThis = false;
            if(xml.tagExists("vote")){
                addNewScene(0,sceneNames[i]);
                xml.pushTag("vote");
                loadThis=true;
            }
            else if(xml.tagExists("quiz")){
                addNewScene(1,sceneNames[i]);
                xml.pushTag("quiz");
                loadThis=true;
                
            }
            else if(xml.tagExists("assesment")){
                addNewScene(2,sceneNames[i]);
                xml.pushTag("assesment");
                loadThis=true;
            }
            if(loadThis){
                cout <<"amount of scenes" + ofToString(scenes.size()) <<endl;
                if(xml.tagExists("globalFontSize")){
                    setSceneAlignersFromXml(&scenes[i],xml);
                    scenes[i].useGlobalAligners = false;
                    
                }
                
                //if(xml.tagExists("number"))scenes[i].useNumbers =true;
                if(xml.tagExists("weighed"))scenes[i].weighed =true;
                if(xml.tagExists("substractive"))scenes[i].substractive =true;
                if(xml.tagExists("globalTime"))scenes[i].globalTimer =xml.getValue("globalTime", 0);
                if(xml.tagExists("totalPoints"))scenes[i].totalPoints =xml.getValue("totalPoints", 0);
                if(xml.tagExists("explanation"))scenes[i].explanation =xml.getValue("explanation", "",0);
    
                if(xml.tagExists("color")){ //colors for assesment is fucked up..
                    xml.pushTag("color");
                    scenes[i].colors[0].setHex(xml.getValue("c", 0,0));
                    scenes[i].colors[1].setHex(xml.getValue("c", 0,1));
                    xml.popTag();
                }
                if (xml.tagExists("headline") && scenes[i].subs.size()>0)scenes[i].subs[0].headline = xml.getValue("headline", ""); //for retarded vote!!!
                
                if(xml.tagExists("question"))scenes[i].amountOfSubs = xml.getNumTags("question");
                for(int u = 0 ; u<xml.getNumTags("question");u++){
                    xml.pushTag("question",u);
                    if (xml.tagExists("headline")&&scenes[i].subs.size()>u)scenes[i].subs[u].headline = xml.getValue("headline", "",0);
                    if (xml.tagExists("secTime")){scenes[i].subs[u].localTimer = xml.getValue("secTime", 0);
                        scenes[i].subs[u].useGlobalTimer = false;
                    }else{
                        scenes[i].subs[u].useGlobalTimer = true;
                    }
                    
                    if (xml.tagExists("q"))scenes[i].subs[u].question = xml.getValue("q", "");
                    for (int p = 0; p<xml.getNumTags("a"); p++) {
                        scenes[i].subs[u].amountAnswerOptions = xml.getNumTags("a");
                        scenes[i].subs[u].answerOptions[p] = xml.getValue("a", "",p);
                        xml.pushTag("a",p);
                        if (xml.tagExists("color")){
                            scenes[i].colors[p].setHex(xml.getValue("color", 0));
                        }
                        if (xml.tagExists("img")){
                            scenes[i].subs[u].images[p] = xml.getValue("img", 0);
                            scenes[i].subs[u].useImages = true;
                        }else{
                            scenes[i].subs[u].useImages = false;
                        }
                        xml.popTag();
                    }
                    
                    xml.popTag();
                    if(xml.tagExists("rightAnswers")){
                        xml.pushTag("rightAnswers");
                        for(int u = 0; u<xml.getNumTags("a");u++){
                            scenes[i].subs[u].rightAnswer = xml.getValue("a", 0,u);
                        }
                        xml.popTag();
                    }
                }
            } //if scene added
            xml.popTag(); //document
        } //if doc exists
        }// if loaded!! 
    }// for scenes
    
}

void myofApp::setGlobalAlignersFromXml(ofxXmlSettings xml){
    for(int i=0; i<alignerStrings.size();i++){
        if(xml.tagExists(alignerStrings[i]))alignerMap[alignerStrings[i]]=xml.getValue(alignerStrings[i], 0);
        cout<< alignerStrings[i] + " " + ofToString(alignerInts[i])<<endl;
    }


}
void myofApp::setSceneAlignersFromXml(Scene *s,ofxXmlSettings xml){
    for(int i = 0; i<alignerStrings.size();i++){
        if(xml.tagExists(alignerStrings[i]))s->alignerMap[alignerStrings[i]]=xml.getValue(alignerStrings[i], 0);
      //  cout<< alignerStrings[i] + " " + ofToString(s->aligners[i])<<endl;
    }


    
}
void myofApp::setGlobalAlignersToXml(ofxXmlSettings* xml){
    //aligners
    
    xml->addTag("globalFontSize");
    xml->setValue("globalFontSize",40);
    for(int i = 0 ; i< alignerStrings.size();i++){
        xml->addTag(alignerStrings[i]);
        xml->setValue(alignerStrings[i], alignerMap[alignerStrings[i]]);
    }


}

void myofApp::setSceneAlignersToXml(Scene s, ofxXmlSettings* xml){
    //aligners
    xml->addTag("globalFontSize");
    xml->setValue("globalFontSize",40);
    
    for(int i = 0; i<alignerStrings.size();i++){
        xml->addTag(alignerStrings[i]);
        xml->setValue(alignerStrings[i], s.alignerMap[alignerStrings[i]]);
    }
    
}





void myofApp::updateGuiToSceneAligners(bool g){
    for(int i = 0; i<alignerStrings.size();i++){
        if(!g /*&& s.aligners.size()>i*/)DatSlider->getSlider(alignerStrings[i])->setValue(scenes[whichScene].alignerMap[alignerStrings[i]]);
        if(g /*&& s.aligners.size()>i*/)DatSlider->getSlider(alignerStrings[i])->setValue(alignerMap[alignerStrings[i]]);
    }
    
}


//--------------------------------------------------------------
void myofApp::keyReleased(int key){
    if(key==OF_KEY_COMMAND)cmd=false;
}

void myofApp::drawColumn(string s, int x, int y, int fontSize,int width){
    
    string formattedText = s;
    
    ofPushMatrix();
    ofTranslate(x,y);
    ofVec2f size = font.drawMultiColumnFormatted(formattedText, fontSize, width,true,false);
    height =  size.y;
    // ofRect(0, 0, size.x, size.y);
    ofPopMatrix();
}

//--------------------------------------------------------------
void myofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void myofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void myofApp::mousePressed(int x, int y, int button){
    mX=x;
    mY=y;
}

//--------------------------------------------------------------
void myofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void myofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void myofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void myofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void myofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void myofApp::dragEvent(ofDragInfo dragInfo){

}
