#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //setup from xml first. - fill subscenes after. Or setup scene with array of subscenes made in read xml.
    string newClipboardContent = ofSystemTextBoxDialog("type name of dataFolder, or make a new folder");
    ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
    dataFolder = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
    
    string path = dataFolder;
    ofDirectory dir(path);
    if(!dir.exists()){
        dir.createDirectory(path);
    }
    
    path = dataFolder+"/img";
    ofDirectory dirImg(path); //relative dir?? -lol

    dirImg.listDir();
    vector<string> dropDownSize;
    for(int i = 0 ; i<dirImg.size();i++){
        vector<string> s = ofSplitString(dirImg.getPath(i),"/");
        dropDownSize.push_back(s.back());
        imagesString.push_back(s.back());
    }
    //if(imageString.size())
    
    colorSelector = new ofxDatGui;
    imageSelector = new ofxDatGui;
    imageSelector->addHeader("images");
    colorSelector->addHeader("colors");
    for(int i = 0 ; i< MAX_SUB_ANSWER;i++){
        imageSelector->addDropdown("I"+ofToString(i+1), dropDownSize);
        colorSelector->addColorPicker("C"+ofToString(i+1));
    }
    imageSelector->onDropdownEvent(this, &ofApp::onDropdownEvent);
    colorSelector->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    imageSelector->addFooter();
    colorSelector->addFooter();
    
    tableNamesGui = new ofxDatGui;
    tableNamesGui->addHeader("tableNames");
    for (int i = 0; i<12; i++) {
        tableNamesGui->addTextInput("TAB_NAME"+ofToString(i+1),"Laud "+ofToString(i+1));
        tableNames.push_back("Laud " +ofToString(i+1));
    }
    tableNamesGui->addFooter();
    tableNamesGui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    
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
    commons.add(useNumbers.set("Use Numbers", false));
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
    DatSub->onButtonEvent(this, &ofApp::onButtonEvent);

    
    ofParameterGroup mode1;
    mode1.setName("mode1");
    mode1.add(weighed.set("weighed vote",false));
    mode1.add(consentual.set("consensus vote",false));
    mode1.add(commons);
    //mode1.add(commonsSub);
    
    ofParameterGroup mode2;
    mode2.add(amountOfSubs.set("amount of subscenes",0,1,MAX_SUB));
    mode2.setName("mode2");
    mode2.add(commons);
    //mode2.add(commonsSub);
    
    ofParameterGroup mode3;
    mode3.setName("mode1");
    mode3.add(amountOfSubs.set("amount of subscenes",0,1,MAX_SUB));
    mode3.add(totalPoints.set("totalPoints",0,0,200));
    mode3.add(substractive.set("substractive",false));
    mode3.add(commons);
    //mode3.add(commonsSub);
    
    sceneGui1.setup(mode1);
    sceneGui2.setup(mode2);
    sceneGui3.setup(mode3);
    
    ofParameterGroup subCommons;
    subCommons.setName("subsceneEditor");
    subCommons.add(amountAnswerOptions.set("Amount of Answers",0,2,MAX_SUB_ANSWER));
    subCommons.add(useImages.set("Use Images", false));
    subCommons.add(useGlobalTimer.set("Use Global Timer", true));
    subCommons.add(localTimer.set("Timer for This Sub", 0,0,100)); //secTime
    subCommons.add(rightAnswer.set("right A(quiz)",0,0,MAX_SUB_ANSWER));
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
    sceneTextEditorGui->addTextInput("E","explanation");
    sceneTextEditorGui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    sceneTextEditorGui->setVisible(false);
    
    subsceneTextEditorGui = new ofxDatGui;
    subsceneTextEditorGui->addHeader();
    subsceneTextEditorGui->setWidth(mainGui.getWidth());
    subsceneTextEditorGui->addTextInput("HEAD","headline");
    subsceneTextEditorGui->addTextInput("QUE","question");
    

    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        subsceneTextEditorGui->addTextInput(ofToString(1+i)+"A","answer option");
    }
    subsceneTextEditorGui->onTextInputEvent(this, &ofApp::onTextInputEvent);
    subsceneTextEditorGui->setVisible(false);
    
    //aligners;
    ofParameterGroup positions;
    ofParameterGroup fontsizes;
    ofParameterGroup aligners;
    
    positions.setName("positions");
    fontsizes.setName("fontSizes");
    //ALIGNERS: seperate from parameters!
    positions.add(p_matrixX.set("matrix x",0,0,RES_W));
    positions.add(p_matrixY.set("matrix y",0,0,RES_W));
    
    positions.add(p_timerX.set("timer x",0,0,RES_W));
    positions.add(p_timerY.set("timer y",0,0,RES_W));
    
    fontsizes.add(p_timerSize.set("timer",0,0,100));
    fontsizes.add(p_liveResultFontSize.set("liveResult",0,0,100));
    fontsizes.add(p_explanationFontSize.set("explanation",0,0,100));
    fontsizes.add(p_chartFontSize.set("chartFont",0,0,100));
    
    fontsizes.add(p_legendFontSize.set("legendFontSize",0,0,100));
    fontsizes.add(p_questionFontSize.set("questionFontSize",0,0,100));
    fontsizes.add(p_resultFontSize.set("resultFontSize",0,0,100));
    fontsizes.add(p_headLineFontSize.set("headLineFontSize",0,0,100));
    
    positions.add(p_liveResultX.set("liveResult x",0,0,RES_W));
    positions.add(p_liveResultY.set("liveResult x",0,0,RES_W));
    
    positions.add(p_legendX.set("legend x",0,0,RES_W));
    positions.add(p_legendY.set("legend y",0,0,RES_W));
    
    positions.add(p_headlineX.set("headline x",0,0,RES_W));
    positions.add(p_headlineY.set("headline y",0,0,RES_W));
    positions.add(p_collumWidth1.set("collum width head",0,0,800));
    
    positions.add(p_explanationX.set("explanation x",0,0,RES_W));
    positions.add(p_explanationY.set("explanation y",0,0,RES_W));
    positions.add(p_collumWidth2.set("collum width exp",0,0,800));
    
    positions.add(p_questionX.set("question x",0,0,RES_W));
    positions.add(p_questionY.set("question x",0,0,RES_W));
    positions.add(p_collumWidth3.set("collum width ques",0,0,800));
    
    positions.add(p_top10chartAssesmentX.set("top10chartAssesment x",0,0,RES_W));
    positions.add(p_top10chartAssesmentY.set("top10chartAssesment y",0,0,RES_W));
    
    positions.add(p_top10chartQuizX.set("top10chartQuizX",0,0,RES_W));
    positions.add(p_top10chartQuizY.set("top10chartQuizY",0,0,RES_W));
    
    aligners.add(fontsizes);
    aligners.add(positions);
    globalAligners.setName("");
    globalAligners.setup(aligners);
    
   // mainGui.setWidthElements(150);
   // globalAligners.setWidthElements(200);
    DatSub->setWidth(sceneGui1.getWidth());
    sceneTextEditorGui->setWidth(sceneGui1.getWidth());
    subsceneTextEditorGui->setWidth(sceneGui1.getWidth());
    
    sceneGui1.loadFont("Verdana.ttf",8);
    sceneGui1.setUseTTF(true);
    
    sceneGui1.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
    sceneGui2.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
    sceneGui3.setPosition(mainGui.getWidth()+20,mainGui.getPosition().y);
    
    subSceneGui.setPosition(mainGui.getWidth()*2+40,mainGui.getPosition().y);
    
    sceneTextEditorGui->setPosition(sceneGui1.getPosition().x,sceneGui1.getPosition().y+sceneGui1.getHeight()+20);
    DatSub->setPosition(sceneTextEditorGui->getPosition().x,sceneTextEditorGui->getHeight()+sceneTextEditorGui->getPosition().y+20);
    subsceneTextEditorGui->setPosition(subSceneGui.getPosition().x,subSceneGui.getPosition().y+subSceneGui.getHeight()+30);
    
    globalAligners.setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);

    colorSelector->setPosition(DatSub->getPosition().x, DatSub->getPosition().y+DatSub->getHeight()+20);
    imageSelector->setPosition(subsceneTextEditorGui->getPosition().x, subsceneTextEditorGui->getPosition().y +subsceneTextEditorGui->getHeight()+20 );
    tableNamesGui->setPosition(mainGui.getPosition().x, ofGetHeight()-(tableNamesGui->getHeight()+40));
    
    colorSelector->setWidth(300);
    imageSelector->setWidth(300);
    tableNamesGui->setWidth(300);
    
    if(scenes.size()>0){
        editExisting[0]=true;
        updateGuiToSceneAligners(scenes[0]);
        setGuiToSceneValues(0);
        setSubGuiToSubValues(0);
    }
    for(int i = 0; i<tableNames.size();i++){
        tableNamesGui->getTextInput("TAB_NAME"+ofToString(i+1))->setText(tableNames[i]);
 
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSetWindowTitle(dataFolder);
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
        addNewScene(0,clipboardContent);
        retardedCounters.push_back(0);
    }
    if(addQuiz){
        string newClipboardContent = ofSystemTextBoxDialog("Name of Quiz");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        addQuiz = false;
        addNewScene(1,clipboardContent);
        retardedCounters.push_back(0);
    }
    if(addAssesment){
        string newClipboardContent = ofSystemTextBoxDialog("Name of Assesment");
        ofxClipboard::copy(newClipboardContent); //<<<<<<<<<<<<<<<<<
        string clipboardContent = ofxClipboard::paste(); //<<<<<<<<<<<<<<<<<
        addAssesment = false;
        addNewScene(2,clipboardContent);
        retardedCounters.push_back(0);
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
            imageSelector->setVisible(true);
            sceneTextEditorGui->setVisible(true);
            subsceneTextEditorGui->setVisible(true);
            
        }
        else if(scenes[whichScene].mode == 1){
            b_sceneGui1 = false;
            b_sceneGui2 = true;
            b_sceneGui3 = false;
            imageSelector->setVisible(true);
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
        updateGuiToSceneAligners(scenes[whichScene]);
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
    
    if(!globalAlignerToggle && useGlobalAligners){
        globalAlignerToggle = true;
        Scene s;
        s.useGlobalAligners = true;
        updateGuiToSceneAligners(s);
    }
    globalAlignerToggle=useGlobalAligners;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 50);
    mainGui.draw();
    subSceneGui.setName("sub "+ofToString(whichSub+1));
    subSceneGui.draw();
    globalAligners.draw();
    if(b_sceneGui1){
        sceneGui1.setName(scenes[whichScene].name);
        sceneGui1.draw();
        ofSetColor(0);
        //float h = 18*(MAX_SUB-scenes[whichScene].amountOfSubs);
        //ofDrawRectangle(sceneGui1.getPosition().x, 18*MAX_SUB-(sceneGui1.getPosition().y+h)+50, sceneGui1.getWidth(), h+50);
        
    }
    if(b_sceneGui2){
        sceneGui2.setName(scenes[whichScene].name);
        sceneGui2.draw();
    }
    if(b_sceneGui3){
        sceneGui3.setName(scenes[whichScene].name);
        sceneGui3.draw();
    }
//    ofSetColor(0, 0, 50);
//    ofDrawRectangle(sceneGui1.getPosition().x-1, sceneGui1.getPosition().y + 18*7 +18*amountOfSubs, sceneGui1.getWidth()+2, 19*(MAX_SUB-amountOfSubs)+3);
//    
    ofSetColor(255);
    ofDrawBitmapString("editing" +ofToString(whichScene+1) + " "+ofToString(whichSub+1), 10, ofGetHeight()-20);
    
    
    for(int i = 0 ; i<MAX_SUB;i++){
        if(i!=whichSub)DatSub->getButton("subscene: "+ofToString(i+1))->setTheme(disabled);
        if(i==whichSub)DatSub->getButton("subscene: "+ofToString(i+1))->setTheme(enabled);
    }

    // subsceens...
}
//--------------------------------------------------------------

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        if(e.target->getLabel() == ofToString(1+i)+"A")scenes[whichScene].subs[whichSub].setAnswerOption(e.target->getText(), i);
    }
    if(e.target->getLabel() == "HEAD")scenes[whichScene].subs[whichSub].headline =e.target->getText();
    if(e.target->getLabel() == "QUE")scenes[whichScene].subs[whichSub].question =e.target->getText();
    
    if(e.target->getLabel() == "E")scenes[whichScene].explanation =e.target->getText();
    
    cout << "onTextInputEvent: " << e.target->getLabel() << " " << e.target->getText() << endl;
    
    for(int i = 0; i<tableNames.size();i++){
        if(e.target->getLabel()=="TAB_NAME"+ofToString(i+1)){
            tableNames[i]=e.target->getText();
        }
    }
}


//--------------------------------------------------------------

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
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

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e){
    cout << e.target->getIndex();
    for(int u = 0 ; u<imagesString.size();u++){
        if(e.target->getLabel()==imagesString[u]){
            
            scenes[whichScene].subs[whichSub].images[e.target->getIndex()-1] = u;
        }
    }
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){
    for (int i = 0; i<MAX_SUB_ANSWER; i++) {
        if(e.target->getLabel()=="C"+ofToString(i+1) && whichScene<scenes.size()){
            scenes[whichScene].colors[i].set(e.target->getColor());
        }
//        scenes[scene].colors[i].set(colorSelector->getColorPicker("C"+ofToString(i+1))->getColor());
    }
}

// for triggering when whichScene or which Sub scene is called!! - SubScene will edit my textstuff...
void ofApp::setGuiToSceneValues(int scene){
    
    amountOfSubs = scenes[scene].amountOfSubs;
    globalTimer = scenes[scene].globalTimer;
    useGlobalAligners =scenes[scene].useGlobalAligners;
    
    weighed=scenes[scene].weighed;
    totalPoints=scenes[scene].totalPoints;
    substractive=scenes[scene].substractive;
    consentual=scenes[scene].consentual;
    
    sceneTextEditorGui->getTextInput("E")->setText( scenes[whichScene].explanation );
    
    setSubGuiToSubValues(whichSub); // setting colors
    for (int i = 0; i<MAX_SUB_ANSWER; i++) {
        colorSelector->getColorPicker("C"+ofToString(i+1))->setColor(scenes[scene].colors[i]);
       // scenes[scene].colors[i].set(colorSelector->getColorPicker("C"+ofToString(i+1))->getColor());
    }
    
 
    // text : TEXT1 and TEXT2 - Scene: text1,text2
}
void ofApp::setSubGuiToSubValues(int sub){
    // text : questions
    // answers : - MAX_SUB_ANSWERS
    
    amountAnswerOptions = scenes[whichScene].subs[sub].amountAnswerOptions;
    useImages = scenes[whichScene].subs[sub].useImages;
    useGlobalTimer = scenes[whichScene].subs[sub].useGlobalTimer;
    localTimer = scenes[whichScene].subs[sub].localTimer;
    rightAnswer = scenes[whichScene].subs[sub].rightAnswer;
    
    subsceneTextEditorGui->getTextInput("HEAD")->setText( scenes[whichScene].subs[sub].headline );
    subsceneTextEditorGui->getTextInput("QUE")->setText( scenes[whichScene].subs[sub].question );
    
    for(int i = 0 ; i<scenes[whichScene].subs[sub].answerOptions.size();i++){
        subsceneTextEditorGui->getTextInput(ofToString(1+i)+"A")->setText(scenes[whichScene].subs[sub].answerOptions[i]);
    }
    for(int i = 0 ; i<scenes[whichScene].subs[sub].answerOptions.size();i++){
        if(imagesString.size()>0)imageSelector->getDropdown("I"+ofToString(i+1))->setLabel(imagesString[scenes[whichScene].subs[sub].images[i]]);
    }
}

void ofApp::updateScene(Scene* s){
    if(s->mode==0)amountOfSubs=1;
    if(s->mode!=1)rightAnswer=0;
    s->amountOfSubs = amountOfSubs;
    s->globalTimer = globalTimer;
    s->useNumbers = useNumbers;
    s->useImages = useImages;
    s->useGlobalAligners = useGlobalAligners;
    
    if(s->useGlobalAligners){
        globalAligners.setName("GOBAL ALIGNERS");
        globalAligners.setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
        globalAligners.setFillColor(ofColor(0,50,0));
       // globalAligners.setPosition(mainGui.getPosition().x,mainGui.getHeight()+40);
        updateGlobalAligners();
    }
    else if(!s->useGlobalAligners){
        globalAligners.setName("LOCAL ALIGNERS FOR: ");
        globalAligners.setPosition(mainGui.getWidth()+20*3+sceneGui1.getWidth()+subSceneGui.getWidth(),mainGui.getPosition().y);
        globalAligners.setFillColor(ofColor(50,0,0));
       // globalAligners.setPosition(sceneGui1.getPosition().x,sceneGui1.getHeight()+40);
        updateLocalAligners(s);
    }//add headline
    
    if(s->mode == 0){
        s->weighed = weighed;
        s->consentual = consentual;
    }
    if(s->mode == 2){
        s->substractive = substractive;
        s->totalPoints = totalPoints;
    }
    
    for(int i = 0; i<MAX_SUB;i++){
        if(i<amountOfSubs)DatSub->getButton("subscene: "+ofToString(i+1))->setVisible(true);
        if(i>=amountOfSubs)DatSub->getButton("subscene: "+ofToString(i+1))->setVisible(false);
    }
    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        if(i<amountOfSubs)colorSelector->getColorPicker("C"+ofToString(i+1))->setVisible(true);
        if(i>=amountOfSubs)colorSelector->getColorPicker("C"+ofToString(i+1))->setVisible(false);
    }
}

void ofApp::updateSubScene(SubScene* s){
    if(s->mode==2)amountAnswerOptions=0;
    s->amountAnswerOptions = amountAnswerOptions;
    if(s->mode==1&&rightAnswer>amountAnswerOptions)rightAnswer=amountAnswerOptions;
    s->rightAnswer = rightAnswer;
    for(int i = 0; i<MAX_SUB_ANSWER;i++){
        if(i<amountAnswerOptions)subsceneTextEditorGui->getTextInput(ofToString(i+1)+"A")->setVisible(true);
        if(i>=amountAnswerOptions)subsceneTextEditorGui->getTextInput(ofToString(i+1)+"A")->setVisible(false);
        
        if(i<amountAnswerOptions)imageSelector->getDropdown("I"+ofToString(i+1))->setVisible(true);
        if(i>=amountAnswerOptions)imageSelector->getDropdown("I"+ofToString(i+1))->setVisible(false);
    }
    
    if(useGlobalTimer){
        s->localTimer = globalTimer;
        localTimer = globalTimer;
    }else{
        s->localTimer = localTimer;
    }
}


//--------------------------------------------------------------
void ofApp::addNewScene(int mode, string name){
    Scene s = *new Scene;
    s.setup(mode,name);
    scenes.push_back(s);
    
    ofParameter<bool> b;
    b.set("scene: "+ofToString(editExisting.size()+1),false);
    editExisting.push_back(b);
    toggleScene.push_back(false);
    mainGui.add(editExisting.back());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //saveToXml();
}


void ofApp::saveToXml(){
    
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
    cout<<table<<endl;
    xml.addTag("tableNames");
    xml.setValue("tableNames", table);
    
    // mising table names, images and wrong image
    
    cout << ofToString(scenes.size()) + " this many scenes"<<endl;
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
        
        xml.addTag("explanation");
        xml.setValue("explanation",scenes[i].explanation);
        
        //---------------vote-------------------------------------------------------------------------
        if(scenes[i].mode == 0){
            if(scenes[i].weighed)xml.addTag("weighed");
            else if(scenes[i].consentual)xml.addTag("consentual");
            else xml.addTag("majority");
            
            xml.addTag("headline");
            xml.setValue("headline",scenes[i].subs[0].headline);
            
            xml.addTag("question");
            xml.pushTag("question");
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
                if(scenes[i].useImages){
                    xml.addTag("img");
                    xml.setValue("img",scenes[i].subs[0].images[p]); // ATTENTION
                }
                xml.popTag();//a
            }
            xml.popTag();

        }
        // ---------------------ASSESMENT -----------------------
        if(scenes[i].mode == 1){
            for(int u = 0 ; u<scenes[i].amountOfSubs;u++){
                xml.addTag("question");
                xml.pushTag("question",u);
                xml.addTag("headline");
                xml.setValue("headline",scenes[i].subs[u].headline);
                if(!scenes[i].subs[0].useGlobalTimer){
                    xml.addTag("secTime");
                    xml.setValue("secTime",scenes[i].subs[0].localTimer);
                }
                xml.addTag("q");
                xml.setValue("q",scenes[i].subs[u].question);//ATTENTION
                
                for(int p = 0 ; p<scenes[i].subs[u].amountAnswerOptions;p++){
                    xml.addTag("a");
                    xml.setValue("a",scenes[i].subs[u].answerOptions[p],p);
                    xml.pushTag("a",p);
                    xml.addTag("color");
                    xml.setValue("color",scenes[i].colors[p].getHex()); // ATTENTION
                    if(scenes[i].useImages){
                        xml.addTag("img");
                        xml.setValue("img",scenes[i].subs[0].images[p]); // ATTENTION
                    }
                    xml.popTag();
                }
                xml.popTag();
                xml.addTag("rightAnswers");
                xml.pushTag("rightAnswers");
                xml.addTag("a");
                xml.setValue("a",scenes[i].subs[u].rightAnswer);
                xml.popTag();
            }
        }
        if(scenes[i].mode == 2){

            if(scenes[i].substractive)xml.addTag("subtractive");
            if(scenes[i].useNumbers)xml.addTag("number");
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
                xml.pushTag("question");
                xml.addTag("headline");
                xml.setValue("headline",scenes[i].subs[u].headline);
                if(!scenes[i].subs[0].useGlobalTimer){
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

void ofApp::loadFromXml(){
    
    xml = *new ofxXmlSettings;
    xml.clear();
    if(xml.loadFile(dataFolder+"/data.xml"))cout<<"loaded "+ dataFolder+"/data.xml"<<endl;
    //xml.loadFile("Sequences/"+name+".xml");
    
    
    xml.pushTag("document");
    setGlobalAlignersFromXml(xml);
        
    for(int i = 0; i<xml.getNumTags("scene"); i++){
        cout<<"scenes "+ ofToString(xml.getNumTags("scene"))<<endl;
        xml.pushTag("scene",i);
        sceneNames.push_back(ofSplitString(xml.getValue("file", ""),".")[0]); //cut of .xml
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
        
        if(xml.loadFile(dataFolder+"/"+sceneNames[i]+".xml"))cout<<"loaded "+ dataFolder+"/"+sceneNames[i]+".xml"<<endl;
        xml.pushTag("document");
        
        if(xml.tagExists("vote")){
            addNewScene(0,sceneNames[i]);
            xml.pushTag("vote");
        }
        if(xml.tagExists("quiz")){
            addNewScene(1,sceneNames[i]);
            xml.pushTag("quiz");

        }
        if(xml.tagExists("assesment")){
            addNewScene(0,sceneNames[2]);
            xml.pushTag("assesment");

        }
        
        cout <<"amount of scenes" + ofToString(scenes.size()) <<endl;
        if(xml.tagExists("globalFontSize"))setSceneAlignersFromXml(&scenes[i],xml);
        
        if(xml.tagExists("number"))scenes[i].useNumbers =true;
        if(xml.tagExists("weighed"))scenes[i].weighed =true;
        if(xml.tagExists("substractive"))scenes[i].substractive =true;
        if(xml.tagExists("globalTime"))scenes[i].globalTimer =xml.getValue("globalTime", 0);
        if(xml.tagExists("totalPoints"))scenes[i].totalPoints =xml.getValue("totalPoints", 0);
        if(xml.tagExists("explanation")){scenes[i].explanation =xml.getValue("explanation", "");
            cout<< "loaded explanation  "+ scenes[i].explanation<<endl;}
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
            if (xml.tagExists("headline"))scenes[i].subs[u].headline = xml.getValue("headline", "");
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
        xml.popTag();
    }// for scenes
    
}

void ofApp::setGlobalAlignersFromXml(ofxXmlSettings xml){
    if(xml.tagExists("liveResultFontSize"))liveResultFontSize=xml.getValue("liveResultFontSize", 0);
    cout<<liveResultFontSize<<endl;
    if(xml.tagExists("headLineFontSize"))headLineFontSize=xml.getValue("headLineFontSize", 0);
    cout<<headLineFontSize<<endl;
    if(xml.tagExists("explanationFontSize"))explanationFontSize=xml.getValue("explanationFontSize", 0);
    cout<<explanationFontSize<<endl;
    if(xml.tagExists("questionFontSize"))questionFontSize=xml.getValue("questionFontSize", 0);
    if(xml.tagExists("chartFontSize"))chartFontSize=xml.getValue("chartFontSize", 0);
    if(xml.tagExists("legendFontSize"))legendFontSize=xml.getValue("legendFontSize", 0);
    if(xml.tagExists("timerSize"))timerSize=xml.getValue("timerSize", 0);
    if(xml.tagExists("resultFontSize"))resultFontSize=xml.getValue("resultFontSize", 0);
    if(xml.tagExists("matrixX"))matrixX=xml.getValue("matrixX", 0);
    if(xml.tagExists("matrixY"))matrixY=xml.getValue("matrixY", 0);
    if(xml.tagExists("timerX"))timerX=xml.getValue("timerX", 0);
    if(xml.tagExists("timerY"))timerY=xml.getValue("timerY", 0);
    if(xml.tagExists("legendX"))legendX=xml.getValue("legendX", 0);
    if(xml.tagExists("legendY"))legendY=xml.getValue("legendY", 0);
    if(xml.tagExists("explanationX"))explanationX=xml.getValue("explanationX", 0);
    if(xml.tagExists("explanationY"))explanationY=xml.getValue("explanationY", 0);
    if(xml.tagExists("headlineX"))headlineX=xml.getValue("headlineX", 0);
    if(xml.tagExists("headlineY"))headlineY=xml.getValue("headlineY", 0);
    if(xml.tagExists("questionX"))questionX=xml.getValue("questionX", 0);
    if(xml.tagExists("questionY"))questionY=xml.getValue("questionY", 0);
    if(xml.tagExists("top10chartAssesmentX"))top10chartAssesmentX=xml.getValue("top10chartAssesmentX", 0);
    if(xml.tagExists("top10chartAssesmentY"))top10chartAssesmentY=xml.getValue("top10chartAssesmentY", 0);
    if(xml.tagExists("top10chartQuizX"))top10chartQuizX=xml.getValue("top10chartQuizX", 0);
    if(xml.tagExists("top10chartQuizY"))top10chartQuizY=xml.getValue("top10chartQuizY", 0);
    if(xml.tagExists("collumWidth1"))collumWidth1=xml.getValue("collumWidth1", 0);
    if(xml.tagExists("collumWidth2"))collumWidth2=xml.getValue("collumWidth2", 0);
    if(xml.tagExists("collumWidth3"))collumWidth3=xml.getValue("collumWidth3", 0);
    
    


}
void ofApp::setSceneAlignersFromXml(Scene *s,ofxXmlSettings xml){
    if(xml.tagExists("liveResultFontSize"))s->liveResultFontSize=xml.getValue("liveResultFontSize", 0);
    if(xml.tagExists("headLineFontSize"))s->headLineFontSize=xml.getValue("headLineFontSize", 0);
    if(xml.tagExists("explanationFontSize"))s->explanationFontSize=xml.getValue("explanationFontSize", 0);
    if(xml.tagExists("questionFontSize"))s->questionFontSize=xml.getValue("questionFontSize", 0);
    if(xml.tagExists("chartFontSize"))s->chartFontSize=xml.getValue("chartFontSize", 0);
    if(xml.tagExists("legendFontSize"))s->legendFontSize=xml.getValue("legendFontSize", 0);
    if(xml.tagExists("timerSize"))s->timerSize=xml.getValue("timerSize", 0);
    if(xml.tagExists("resultFontSize"))s->resultFontSize=xml.getValue("resultFontSize", 0);
    if(xml.tagExists("matrixX"))s->matrixX=xml.getValue("matrixX", 0);
    if(xml.tagExists("matrixY"))s->matrixY=xml.getValue("matrixY", 0);
    if(xml.tagExists("timerX"))s->timerX=xml.getValue("timerX", 0);
    if(xml.tagExists("timerY"))s->timerY=xml.getValue("timerY", 0);
    if(xml.tagExists("legendX"))s->legendX=xml.getValue("legendX", 0);
    if(xml.tagExists("legendY"))s->legendY=xml.getValue("legendY", 0);
    if(xml.tagExists("explanationX"))s->explanationX=xml.getValue("explanationX", 0);
    if(xml.tagExists("explanationY"))s->explanationY=xml.getValue("explanationY", 0);
    if(xml.tagExists("headlineX"))s->headlineX=xml.getValue("headlineX", 0);
    if(xml.tagExists("headlineY"))s->headlineY=xml.getValue("headlineY", 0);
    if(xml.tagExists("questionX"))s->questionX=xml.getValue("questionX", 0);
    if(xml.tagExists("questionY"))s->questionY=xml.getValue("questionY", 0);
    if(xml.tagExists("top10chartAssesmentX"))s->top10chartAssesmentX=xml.getValue("top10chartAssesmentX", 0);
    if(xml.tagExists("top10chartAssesmentY"))s->top10chartAssesmentY=xml.getValue("top10chartAssesmentY", 0);
    if(xml.tagExists("top10chartQuizX"))s->top10chartQuizX=xml.getValue("top10chartQuizX", 0);
    if(xml.tagExists("top10chartQuizY"))s->top10chartQuizY=xml.getValue("top10chartQuizY", 0);
    if(xml.tagExists("collumWidth1"))s->collumWidth1=xml.getValue("collumWidth1", 0);
    if(xml.tagExists("collumWidth2"))s->collumWidth2=xml.getValue("collumWidth2", 0);
    if(xml.tagExists("collumWidth3"))s->collumWidth3=xml.getValue("collumWidth3", 0);
    

    
}
void ofApp::setGlobalAlignersToXml(ofxXmlSettings* xml){
    //aligners
    xml->addTag("globalFontSize");
    xml->setValue("globalFontSize",40);
    
    xml->addTag("liveResultFontSize");
    xml->setValue("liveResultFontSize",liveResultFontSize);
    xml->addTag("headLineFontSize");
    xml->setValue("headLineFontSize",headLineFontSize);
    xml->addTag("explanationFontSize");
    xml->setValue("explanationFontSize",explanationFontSize);
    xml->addTag("questionFontSize");
    xml->setValue("questionFontSize",questionFontSize);
    xml->addTag("chartFontSize");
    xml->setValue("chartFontSize",chartFontSize);
    xml->addTag("legendFontSize");
    xml->setValue("legendFontSize",legendFontSize);
    xml->addTag("timerSize");
    xml->setValue("timerSize",timerSize);
    xml->addTag("resultFontSize");
    xml->setValue("resultFontSize",resultFontSize);
    
    xml->addTag("matrixX");
    xml->setValue("matrixX",matrixX);
    xml->addTag("matrixY");
    xml->setValue("matrixY",matrixY);
    xml->addTag("timerX");
    xml->setValue("timerX",timerX);
    xml->addTag("timerY");
    xml->setValue("timerY",timerY);
    xml->addTag("legendX");
    xml->setValue("legendX",legendX);
    xml->addTag("legendY");
    xml->setValue("legendY",legendY);
    xml->addTag("explanationX");
    xml->setValue("explanationX",explanationX);
    xml->addTag("explanationY");
    xml->setValue("explanationY",explanationY);
    xml->addTag("headlineX");
    xml->setValue("headlineX",headlineX);
    xml->addTag("headlineY");
    xml->setValue("headlineY",headlineY);
    xml->addTag("questionX");
    xml->setValue("questionX",questionX);
    
    xml->addTag("collumWidth1");
    xml->setValue("collumWidth1",collumWidth1);
    xml->addTag("collumWidth2");
    xml->setValue("collumWidth2",collumWidth2);
    xml->addTag("collumWidth3");
    xml->setValue("collumWidth3",collumWidth3);
    
    xml->addTag("questionY");
    xml->setValue("questionY",questionY);
    xml->addTag("top10chartAssesmentX");
    xml->setValue("top10chartAssesmentX",top10chartAssesmentX);
    xml->addTag("top10chartAssesmentY");
    xml->setValue("top10chartAssesmentY",top10chartAssesmentY);
    xml->addTag("top10chartQuizX");
    xml->setValue("top10chartQuizX",top10chartQuizX);
    xml->addTag("top10chartQuizY");
    xml->setValue("top10chartQuizY",top10chartQuizY);

}

void ofApp::setSceneAlignersToXml(Scene s, ofxXmlSettings* xml){
    //aligners
    xml->addTag("globalFontSize");
    xml->setValue("globalFontSize",40);
    
    xml->addTag("liveResultFontSize");
    xml->setValue("liveResultFontSize",s.liveResultFontSize);
    xml->addTag("headLineFontSize");
    xml->setValue("headLineFontSize",s.headLineFontSize);
    xml->addTag("explanationFontSize");
    xml->setValue("explanationFontSize",s.explanationFontSize);
    xml->addTag("questionFontSize");
    xml->setValue("questionFontSize",s.questionFontSize);
    xml->addTag("chartFontSize");
    xml->setValue("chartFontSize",s.chartFontSize);
    xml->addTag("legendFontSize");
    xml->setValue("legendFontSize",s.legendFontSize);
    xml->addTag("timerSize");
    xml->setValue("timerSize",s.timerSize);
    xml->addTag("resultFontSize");
    xml->setValue("resultFontSize",s.resultFontSize);
    
    xml->addTag("matrixX");
    xml->setValue("matrixX",s.matrixX);
    xml->addTag("matrixY");
    xml->setValue("matrixY",s.matrixY);
    xml->addTag("timerX");
    xml->setValue("timerX",s.timerX);
    xml->addTag("timerY");
    xml->setValue("timerY",s.timerY);
    xml->addTag("legendX");
    xml->setValue("legendX",s.legendX);
    xml->addTag("legendY");
    xml->setValue("legendY",s.legendY);
    xml->addTag("explanationX");
    xml->setValue("explanationX",s.explanationX);
    xml->addTag("explanationY");
    xml->setValue("explanationY",s.explanationY);
    xml->addTag("headlineX");
    xml->setValue("headlineX",s.headlineX);
    xml->addTag("headlineY");
    xml->setValue("headlineY",s.headlineY);
    xml->addTag("questionX");
    xml->setValue("questionX",s.questionX);
    
    xml->addTag("questionY");
    xml->setValue("questionY",s.questionY);
    xml->addTag("top10chartAssesmentX");
    xml->setValue("top10chartAssesmentX",s.top10chartAssesmentX);
    xml->addTag("top10chartAssesmentY");
    xml->setValue("top10chartAssesmentY",s.top10chartAssesmentY);
    xml->addTag("top10chartQuizX");
    xml->setValue("top10chartQuizX",s.top10chartQuizX);
    xml->addTag("top10chartQuizY");
    xml->setValue("top10chartQuizY",s.top10chartQuizY);
    
    xml->addTag("collumWidth1");
    xml->setValue("collumWidth1",s.collumWidth1);
    xml->addTag("collumWidth2");
    xml->setValue("collumWidth2",s.collumWidth2);
    xml->addTag("collumWidth3");
    xml->setValue("collumWidth3",s.collumWidth3);
    
}

// alinger aligning... updateGlobal -> copy p_sliders to int values. , copy p_sliders to scene. Set p_sliders to scene values.
void ofApp::updateGlobalAligners(){ /// only making global aligners... - these are for all that does not specify othersthings.
    matrixX = p_matrixX;
    matrixY= p_matrixY;
    
    timerX=p_timerX;
    timerY=p_timerY;
    
    timerSize=p_timerSize;
    liveResultFontSize=p_liveResultFontSize;
    explanationFontSize=p_explanationFontSize;
    chartFontSize=p_chartFontSize;
    legendFontSize=p_legendFontSize;
    resultFontSize=p_resultFontSize;
    questionFontSize=p_questionFontSize;
    headLineFontSize=p_headLineFontSize;
    
    liveResultX=p_liveResultX;
    liveResultY=p_liveResultY;
    
    legendX=p_legendX;
    legendY=p_legendY;
    
    explanationX=p_explanationX; //text2
    explanationY=p_explanationY;
    
    collumWidth1 = p_collumWidth1;
    collumWidth2 = p_collumWidth2;
    collumWidth3 = p_collumWidth3;
    
    headlineX=p_headlineX; //text1
    headlineY=p_headlineY;
    
    chartFontSize=p_chartFontSize;
    
    top10chartAssesmentX=p_top10chartAssesmentX;
    top10chartAssesmentY=p_top10chartAssesmentY;
    
    top10chartQuizX=p_top10chartQuizX;
    top10chartQuizY=p_top10chartQuizY;
    
    questionX=p_questionX;
    questionY=p_questionY;
}

void ofApp::updateLocalAligners(Scene* s){
    s->matrixX = p_matrixX;
    s->matrixY= p_matrixY;
    
    s->timerX=p_timerX;
    s->timerY=p_timerY;
    
    s->timerSize=p_timerSize;
    s->liveResultFontSize=p_liveResultFontSize;
    s->explanationFontSize=p_explanationFontSize;
    s->chartFontSize=p_chartFontSize;
    s->legendFontSize=p_legendFontSize;
    s->resultFontSize=p_resultFontSize;
    s->questionFontSize=p_questionFontSize;
    s->headLineFontSize=p_headLineFontSize;
    
    s->liveResultX=p_liveResultX;
    s->liveResultY=p_liveResultY;
    
    s->legendX=p_legendX;
    s->legendY=p_legendY;
    
    s->explanationX=p_explanationX; //text2
    s->explanationY=p_explanationY;
    
    s->headlineX=p_headlineX; //text1
    s->headlineY=p_headlineY;
    
    s->chartFontSize=p_chartFontSize;
    
    s->top10chartAssesmentX=p_top10chartAssesmentX;
    s->top10chartAssesmentY=p_top10chartAssesmentY;
    
    s->top10chartQuizX=p_top10chartQuizX;
    s->top10chartQuizY=p_top10chartQuizY;
    
    s->questionX=p_questionX;
    s->questionY=p_questionY;
    
    s->collumWidth1 = p_collumWidth1;
    s->collumWidth2 = p_collumWidth2;
    s->collumWidth3 = p_collumWidth3;
}

void ofApp::updateGuiToSceneAligners(Scene s){
    if(!s.useGlobalAligners){
        p_matrixX = s.matrixX;
        p_matrixY= s.matrixY;
        
        p_timerX=s.timerX;
        p_timerY=s.timerY;
        
        p_timerSize=s.timerSize;
        p_liveResultFontSize=s.liveResultFontSize;
        p_explanationFontSize=s.explanationFontSize;
        p_chartFontSize=s.chartFontSize;
        p_legendFontSize=s.legendFontSize;
        p_resultFontSize=s.resultFontSize;
        p_questionFontSize=s.questionFontSize;
        p_headLineFontSize=s.headLineFontSize;
        
        p_liveResultX=s.liveResultX;
        p_liveResultY=s.liveResultY;
        
        p_legendX=s.legendX;
        p_legendY=s.legendY;
        
        p_explanationX=s.explanationX; //text2
        p_explanationY=s.explanationY;
        
        p_collumWidth1 = s.collumWidth1;
        p_collumWidth2 = s.collumWidth2;
        p_collumWidth3 = s.collumWidth3;
        
        p_headlineX=s.headlineX; //text1
        p_headlineY=s.headlineY;
        
        p_chartFontSize=s.chartFontSize;
        
        p_top10chartAssesmentX=s.top10chartAssesmentX;
        p_top10chartAssesmentY=s.top10chartAssesmentY;
        
        p_top10chartQuizX=s.top10chartQuizX;
        p_top10chartQuizY=s.top10chartQuizY;
        
        p_questionX=s.questionX;
        p_questionY=s.questionY;
    }
    else{
        p_matrixX = matrixX;
        p_matrixY= matrixY;
        
        p_timerX=timerX;
        p_timerY=timerY;
        
        p_timerSize=timerSize;
        p_liveResultFontSize=liveResultFontSize;
        p_explanationFontSize=explanationFontSize;
        p_chartFontSize=chartFontSize;
        p_legendFontSize=legendFontSize;
        p_resultFontSize=resultFontSize;
        p_questionFontSize=questionFontSize;
        p_headLineFontSize=headLineFontSize;
        
        p_liveResultX=liveResultX;
        p_liveResultY=liveResultY;
        
        p_legendX=legendX;
        p_legendY=legendY;
        
        p_explanationX=explanationX; //text2
        p_explanationY=explanationY;
        
        p_collumWidth1 = collumWidth1;
        p_collumWidth2 = collumWidth2;
        p_collumWidth3 = collumWidth3;
        
        p_headlineX=headlineX; //text1
        p_headlineY=headlineY;
        
        p_chartFontSize=chartFontSize;
        
        p_top10chartAssesmentX=top10chartAssesmentX;
        p_top10chartAssesmentY=top10chartAssesmentY;
        
        p_top10chartQuizX=top10chartQuizX;
        p_top10chartQuizY=top10chartQuizY;
        
        p_questionX=questionX;
        p_questionY=questionY;
        
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
