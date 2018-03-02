#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "firebase/app.h"
#include "firebase/admob.h"
#include "firebase/admob/types.h"

class HelloWorld : public cocos2d::Scene
{

public :
	enum State {
		kNextStepInitialize = 0,
		kNextStepLoadAd,
		kNextStepShowAd,
		kNextStepRewardUser,
		kNextStepRestartGame
	};
	State _state;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
	
	
private:
	void LoadInterstitial();
	void LoadRewardedVideo();
	
	virtual void update(float delta);
private:
	cocos2d::Label* titleLabel;
    
public:
    firebase::admob::AdRequest createAdRequest();
    void showVideo();
    
};

#endif // __HELLOWORLD_SCENE_H__
