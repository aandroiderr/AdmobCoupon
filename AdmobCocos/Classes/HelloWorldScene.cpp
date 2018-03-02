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
cocos2d::Label *notificationLabel;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void HelloWorld::callbackFromJS(cocos2d::experimental::ui::WebView* webview, const std::string &answer) {
	std::string response = answer;

    if (response.find("backbutton.com") != std::string::npos) {

            this->_webView->removeFromParentAndCleanup(true);


    }else
    if (response.find("redeem.com") != std::string::npos) {

    size_t pos = 0;
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
    redeemClick( vcListVector.at(0));


}}else{
    adCouponOnClickToUnlock();
}}
void HelloWorld::redeemClick(std::string url){
Application::getInstance()->openURL(url);
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE||keyCode==EventKeyboard::KeyCode::KEY_BACK||keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
    {
        if( this->_webView->getParent()){event->stopPropagation();
            this->_webView->removeFromParentAndCleanup(true);

        }else{  Director::getInstance()->end();}
    }
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    addChild(LayerColor::create(Color4B(225,225,225,255), visibleSize.width, visibleSize.height));
    
    
    notificationLabel = Label::createWithTTF("--", "res/adcoupon_assets/SourceSansPro-Bold.ttf", 30);
    notificationLabel->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.25));
    notificationLabel->setTextColor(Color4B::BLUE);
    addChild(notificationLabel);




    auto touchListener = EventListenerKeyboard::create();
    touchListener->onKeyReleased =CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,this);


    ui::Button *close = ui::Button::create();
    close->setTitleFontName("res/adcoupon_assets/SourceSansPro-Bold.ttf");
    close->setContentSize(Size(300, 60));
    close->setTitleText("Open Offer Wall");
    close->setTitleColor(Color3B::YELLOW);
    close->setPressedActionEnabled(true);
    close->setTitleFontSize(25);
    close->setPosition(getContentSize() * 0.5);
    close->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType t){
        if (t == ui::Widget::TouchEventType::ENDED) {
        //    AdCoupon::showCoupons();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            
            _webView = experimental::ui::WebView::create();
            _webView->setContentSize(Size(Director::getInstance()->getVisibleSize().width, Director::getInstance()->getVisibleSize().height));
            _webView->setAnchorPoint(Vec2(0,0));
            _webView->setPosition(origin);
            _webView->setJavascriptInterfaceScheme("cocos2dx");
            _webView->loadURL("https://norahabsentia.github.io/coupons-webview-webpage/index.html");
            _webView->setOnJSCallback(CC_CALLBACK_2(HelloWorld::callbackFromJS,this));
            _webView->setFocusEnabled(false);
            _webView->setFocused(false);
            //_webView->setScalesPageToFit(true);
            this->addChild(_webView);
        }
    });
    
    addChild(close);
    
    //Kietle
    AdmobManager::getInstance()->initialize(this);
    
    auto btnInterstitial = ui::Button::create();
    btnInterstitial->setTitleText("Show Interstitial");
    btnInterstitial->setTitleFontSize(30);
    btnInterstitial->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 100));
    btnInterstitial->addClickEventListener([](Ref* obj){
        AdmobManager::getInstance()->showInterstitialAds();
        //AdmobManager::getInstance()->showVideoAds();
    });
    addChild(btnInterstitial);
    
    btnInterstitial = ui::Button::create();
    btnInterstitial->setTitleText("Show Video");
    btnInterstitial->setTitleFontSize(30);
    btnInterstitial->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 160));
    btnInterstitial->addClickEventListener([](Ref* obj){
        AdmobManager::getInstance()->showVideoAds();
    });
    addChild(btnInterstitial);
    
    return true;
}

#pragma mark -AdCouponDelegate
void HelloWorld::adCouponHasNoCouponsAvailable() {
    log("no coupons available!");
    notificationLabel->setString("no coupons available!");
}
void HelloWorld::adCouponOnShow() {
    log("coupons on show!");
    notificationLabel->setString("coupons on show!");
}
void HelloWorld::adCouponOnClickToUnlock() {
    CCLOG("COUPONS");
    log("coupon on click to unlock!");
    notificationLabel->setString("coupon on click to unlock!");
    
    ui::Button *close = ui::Button::create();
    close->setTitleFontName("res/adcoupon_assets/SourceSansPro-Bold.ttf");
    close->setContentSize(Size(300, 60));
    close->setName("i_watched_ad_button");
    close->setTitleText("I watched rewarded video!");
    close->setTitleColor(Color3B::GREEN);
    close->setPressedActionEnabled(true);
    close->setTitleFontSize(25);
    close->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.1));
    close->addTouchEventListener([this](Ref *ref, ui::Widget::TouchEventType t){
        if (t == ui::Widget::TouchEventType::ENDED) {
            //AdCoupon::showRedeemCoupon();
            auto bt = dynamic_cast<cocos2d::ui::Button *>(ref);
            bt->removeFromParent();
        }
    });
    
    this->addChild(close,5);close->setGlobalZOrder(10);
}
void HelloWorld::adCouponOnRedirectToLink() {
    log("coupon on redirect to link");
    notificationLabel->setString("coupon on redirect to link");
}
void HelloWorld::adCouponOnClose() {
    log("coupon on close");
    notificationLabel->setString("coupon on close!");
    
    auto node = getChildByName("i_watched_ad_button");
    if (node) {
        node->removeFromParentAndCleanup(true);
    }
}
void HelloWorld::adCouponsOnRequest() {
    log("coupons requested");
    notificationLabel->setString("coupons on request!");
}
