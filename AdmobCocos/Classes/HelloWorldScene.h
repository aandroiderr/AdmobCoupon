#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <ui/UIWebView.h>
#include "cocos2d.h"
#include "AdmobManager.h"

class HelloWorld : public cocos2d::Scene, public AdmobManagerDelegate
{
public:
    static cocos2d::Scene* createScene();
	void callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer);
    virtual bool init();
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    cocos2d::experimental::ui::WebView *_webView;
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void redeemClick(std::string url);
     void adCouponHasNoCouponsAvailable();
     void adCouponOnShow();
    void adCouponOnClickToUnlock();
     void adCouponOnRedirectToLink();
     void adCouponOnClose();
     void adCouponsOnRequest();
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    
    virtual void AdmobManagerOnInterstitialClose(){cocos2d::log("===interstitial close");};
    virtual void AdmobManagerOnVideoClose(){cocos2d::log("===video close");};
};

#endif // __HELLOWORLD_SCENE_H__
