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

// The AdMob app IDs.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdMobAppID = "ca-app-pub-3940256099942544~3347511713";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdMobAppID = "ca-app-pub-3940256099942544~1458002511";
#else
const char* kAdMobAppID = "";
#endif

// These ad units are configured to always serve test ads.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/6300978111";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";
const char* kRewardedVideoAdUnit = "ca-app-pub-3940256099942544/2888167318";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/4411468910";
const char* kRewardedVideoAdUnit = "ca-app-pub-3940256099942544/6386090517";
#else
const char* kAdViewAdUnit = "";
const char* kInterstitialAdUnit = "";
const char* kRewardedVideoAdUnit = "";
#endif

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


static void onRewardedVideoShowAdCompletionCallback(
                                                    const firebase::Future<void>& future, void* userData) {
    HelloWorld* scene = static_cast<HelloWorld*>(userData);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        log("Show rewarded video completed successfully.");
    } else {
        log("Show rewarded video with error code[%d][%s]",future.error(), future.error_message());
    }
}


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
    firebase::admob::rewarded_video::LoadAd(kRewardedVideoAdUnit, createAdRequest());
    rewarded_video::LoadAdLastResult().OnCompletion(onRewardedVideoLoadAdCompletionCallback, this);
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
    //CCLOG("===state[%d]", _state);

    switch(_state){
        case kNextStepLoadAd:
            if(firebase::admob::rewarded_video::InitializeLastResult().status() == firebase::kFutureStatusComplete &&
               firebase::admob::rewarded_video::InitializeLastResult().error() == firebase::admob::kAdMobErrorNone)
            {
                //firebase::admob::rewarded_video::LoadAd("ca-app-pub-4335424038866907/6860135984", my_ad_request);
                firebase::admob::rewarded_video::LoadAd(kRewardedVideoAdUnit, createAdRequest());
                _state = kNextStepShowAd;
            }
            break;

        case kNextStepShowAd:
            if (rewarded_video::LoadAdLastResult().status() ==
                firebase::kFutureStatusComplete &&
                rewarded_video::LoadAdLastResult().error() ==
                firebase::admob::kAdMobErrorNone) {
                rewarded_video::Show(getAdParent());
            }

            if (rewarded_video::ShowLastResult().status() ==
                firebase::kFutureStatusComplete &&
                rewarded_video::ShowLastResult().error() ==
                firebase::admob::kAdMobErrorNone &&
                rewarded_video::presentation_state() ==
                rewarded_video::kPresentationStateHidden) {
                // If the rewarded video has been displayed to the user and
                // the user closed the ad, then reward the user.
                _state = kNextStepRewardUser;
            }
            break;

        default:
            break;
    }
}

void HelloWorld::showVideo(){
    log("===show video");
    rewarded_video::Show(getAdParent());
    rewarded_video::ShowLastResult().OnCompletion(onRewardedVideoShowAdCompletionCallback, this);
}
