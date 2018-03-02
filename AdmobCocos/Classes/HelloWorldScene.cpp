#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "FirebaseHelper.h"

#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/banner_view.h"
#include "firebase/admob/interstitial_ad.h"
#include "firebase/admob/rewarded_video.h"

#include "AdmobManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

//#if defined(__ANDROID__)
//const char* kAppID =                "ca-app-pub-1303483077276475~7139293421";
//const char* kBannerAdUnit =         "ca-app-pub-1303483077276475/2068173417";
//const char* kInterstitialAdUnit =   "ca-app-pub-1303483077276475/2653608726";
//const char* kRewardedAdUnit =       "ca-app-pub-1303483077276475/4106291192";
//#else
//const char* kAppID =                "ca-app-pub-1303483077276475~2486975815";
//const char* kBannerAdUnit =         "ca-app-pub-1303483077276475/6346329779";
//const char* kInterstitialAdUnit =   "ca-app-pub-1303483077276475/5483430970";
//const char* kRewardedAdUnit =       "ca-app-pub-1303483077276475/3197422259";
//#endif

//// The AdMob app IDs.
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//const char* kAdMobAppID = "ca-app-pub-4335424038866907~2928103176";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//const char* kAdMobAppID = "ca-app-pub-4335424038866907~2928103176";
//#else
//const char* kAdMobAppID = "";
//#endif
//
//// These ad units are configured to always serve test ads.
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
//const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";
//const char* kRewardedVideoAdUnit = "ca-app-pub-4335424038866907/6860135984";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
//const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";
//const char* kRewardedVideoAdUnit = "ca-app-pub-4335424038866907/6860135984";
//#else
//const char* kAdViewAdUnit = "";
//const char* kInterstitialAdUnit = "";
//const char* kRewardedVideoAdUnit = "";
//#endif
//
namespace rewarded_video = firebase::admob::rewarded_video;


/// This function is called when the Future for the last call to
/// rewarded_video::LoadAd() method completes.
static void onRewardedVideoLoadAdCompletionCallback(
                                                    const firebase::Future<void>& future, void* userData) {
    HelloWorld* scene = static_cast<HelloWorld*>(userData);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        log("Loading rewarded video completed successfully.");
        scene->showVideo();
    } else {
        log("Loading rewarded video failed.");
        log(
                          "ERROR: Action failed with error code %d and message \"%s\".",
                          future.error(), future.error_message());
        // Rewarded Video returned an error. This might be because the developer did
        // not put their Rewarded Video ad unit into kRewardedVideoAdUnit above.
        log("WARNING: Is your Rewarded Video ad unit ID correct?");
        log(
                          "Ensure kRewardedVideoAdUnit is set to your own Rewarded Video ad unit "
                          "ID.");
    }
}


// A simple listener that logs changes to rewarded video state.
class LoggingRewardedVideoListener
: public firebase::admob::rewarded_video::Listener {
public:
    LoggingRewardedVideoListener() {}
    void OnRewarded(firebase::admob::rewarded_video::RewardItem reward) override {
        log("Rewarding user with %f %s.", reward.amount,
                   reward.reward_type.c_str());
    }
    void OnPresentationStateChanged(
                                    firebase::admob::rewarded_video::PresentationState state) override {
        log("Rewarded video PresentationState has changed to %d.", state);
    }
};


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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    titleLabel = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    titleLabel->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - titleLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(titleLabel, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
		
//    _state = kNextStepLoadAd;
//    scheduleUpdate();
    AdmobManager::getInstance()->initialize();
	
    return true;
}

void HelloWorld::LoadInterstitial()
{
}

void HelloWorld::LoadRewardedVideo()
{
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
//    if (!_rewardedVideoListener){
//        _rewardedVideoListener = new LoggingRewardedVideoListener();
//    }
//    rewarded_video::SetListener(_rewardedVideoListener);
//
//    firebase::admob::rewarded_video::LoadAd("ca-app-pub-4335424038866907/6860135984", createAdRequest());
//    rewarded_video::LoadAdLastResult().OnCompletion(onRewardedVideoLoadAdCompletionCallback, this);
    
    AdmobManager::getInstance()->showVideoAds();
}

firebase::admob::AdRequest HelloWorld::createAdRequest(){
    firebase::admob::AdRequest my_ad_request = {};
    static const char* kTestDeviceIDs[] = {
        "bc5032b2a871da511332401af3ac6bb0",
        "DD07D83E04F8C06551082531F7E2B8DB"};
    my_ad_request.test_device_id_count = sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
    my_ad_request.test_device_ids = kTestDeviceIDs;
    return my_ad_request;
}

void HelloWorld::update(float delta) 
{
}

void HelloWorld::showVideo(){
    log("===show video");
    rewarded_video::Show(getAdParent());
}
