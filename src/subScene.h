//
//  subScene.h
//  SceneGui
//
//  Created by Karina Jensen on 13/04/16.
//
//

#ifndef subScene_h
#define subScene_h
#include "defines.h"

class SubScene{
public:
    int mode;
    
    string headline="headline";
    string question="question";
    
    int localTimer;
    int amountAnswerOptions;
    int rightAnswer=0;
    bool useImages;
    bool useGlobalTimer;
   
    vector<int>images;
   
    vector<string>answerOptions; // == MAX_SUB_ANSWER !
    
    void setup(string q, vector<string>a, vector<int>img){
        question = q;
        headline="headline";
        answerOptions.resize(MAX_SUB_ANSWER);
        for(int i = 0; i<a.size();i++){
            setAnswerOption(a[i],i);
            images.push_back(img[i]);
        }
    }
    
    void setAnswerOption(string s, int a){
        if(answerOptions.size()>a)answerOptions[a] = s;
    }
    
    void setQuestion(string s){
        question = s;
    }
    
};

#endif /* subScene_h */
