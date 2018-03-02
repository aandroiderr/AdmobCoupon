#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "network/HttpClient.h"
#include "extensions/cocos-ext.h"
#include "ui/UIWebView.h"
#include <string>
#include <algorithm>    // copy
#include <iterator>     // back_inserter
#include <regex>
USING_NS_CC;

using namespace std;

AdmobCouponWrapper::~AdmobCouponWrapper(){
    AdmobManager::getInstance()->setDelegate(nullptr);
}

void AdmobCouponWrapper::callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer) {
    std::string response = answer;
    CCLOG("%s",response.c_str());
    if (response.find("backbutton.com") != std::string::npos) {
        this->_webView->removeFromParentAndCleanup(true);
    }else if (response.find("redeem.com") != std::string::npos) {
        
        std::vector<std::string> vcListVector;
        std::string delimiter="redeem.com";
        std::string strTmp = response;
        size_t cutAt;
        cutAt = strTmp.find(delimiter);
        
        if(cutAt > 0)
        {
            vcListVector.push_back(strTmp.substr(cutAt+delimiter.length()));
        }
        
        
        if(vcListVector.size()>0) {
            CCLOG("%s","DONEDONE");
            redeemClick( urlDecode(vcListVector.at(0)));
        }
        
    }else{
        adCouponOnClickToUnlock();
    }
}
void AdmobCouponWrapper::redeemClick(std::string url){
    CCLOG("%s","DONEDDONE");
    Application::getInstance()->openURL(url);
}

void AdmobCouponWrapper::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE||keyCode==EventKeyboard::KeyCode::KEY_BACK||keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
    {
        if( this->_webView->getParent()){
            event->stopPropagation();
            this->_webView->removeFromParentAndCleanup(true);
            
        }else{
            Director::getInstance()->end();
        }
    }
}
// on "init" you need to initialize your instance
bool AdmobCouponWrapper::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    auto touchListener = EventListenerKeyboard::create();
    touchListener->onKeyReleased =CC_CALLBACK_2(AdmobCouponWrapper::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);

    //init admob
    AdmobManager::getInstance()->initialize(this);
    
    return true;
}

void AdmobCouponWrapper::adCouponOnClickToUnlock() {
    log("coupon on click to unlock!");
    AdmobManager::getInstance()->showVideoAds();
}

void AdmobCouponWrapper::inititial(){
}

void AdmobCouponWrapper::showCoupon(){
    _webView = experimental::ui::WebView::create();
    _webView->setContentSize(Size(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height));
    Point origin=Director::getInstance()->getVisibleOrigin();
    _webView->setAnchorPoint(Vec2(0,0));
    _webView->setPosition(origin);
    _webView->setJavascriptInterfaceScheme("cocos2dx");
    _webView->loadURL("https://norahabsentia.github.io/coupons-webview-webpage/index.html");
    _webView->setOnJSCallback(CC_CALLBACK_2(AdmobCouponWrapper::callbackFromJS,this));
    _webView->setFocusEnabled(false);
    _webView->setFocused(false);
    this->addChild(_webView);
}

void AdmobCouponWrapper::showInterstitial(){
    AdmobManager::getInstance()->showInterstitialAds();
}

#pragma mark AdManagerDelegate
void AdmobCouponWrapper::AdmobManagerOnVideoClose(){
    cocos2d::log("===video close");
}

void AdmobCouponWrapper::AdmobManagerOnInterstitialClose(){
    cocos2d::log("===interstitial close");
    showCoupon();
}
