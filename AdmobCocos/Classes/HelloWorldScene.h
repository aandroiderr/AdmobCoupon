#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "UnityAdsNativeAPI.h"

namespace firebase
{
	namespace admob
	{
		class InterstitialAd;
	}
}

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
	
	// Unity Ads
    void rewardPlayer(const char *placementId);
    void initUnityAdsFunc();
    void showUnityAdsFunc(Ref* pSender);
	
private:
	void LoadInterstitial();
	void LoadRewardedVideo();
	
	virtual void update(float delta);
private:
	firebase::admob::InterstitialAd* interstitial_ad;
	cocos2d::Label* titleLabel;
};

#endif // __HELLOWORLD_SCENE_H__
