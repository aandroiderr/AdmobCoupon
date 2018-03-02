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

#if defined(__ANDROID__)
const char* kAppID = "ca-app-pub-1303483077276475~2486975815";
const char* kBannerAdUnit = "ca-app-pub-3940256099942544/6300978111";
const char* kInterstitialAdUnit = "ca-app-pub-3940256099942544/1033173712";
const char* kRewardedAdUnit = "ca-app-pub-3940256099942544/1033173712";
#else
const char* kAppID =                "ca-app-pub-1303483077276475~2486975815";
const char* kBannerAdUnit =         "ca-app-pub-1303483077276475/6346329779";
const char* kInterstitialAdUnit =   "ca-app-pub-1303483077276475/5483430970";
const char* kRewardedAdUnit =       "ca-app-pub-1303483077276475/3197422259";
#endif

namespace rewarded_video = firebase::admob::rewarded_video;

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
	interstitial_ad = NULL;
	
	//LoadInterstitial();
	
	//LoadRewardedVideo();
			
	// Schedule updates so that the Cocos2d-x update() method gets called.
	//this->scheduleUpdate();

    //KIETLE
    CCLOG("====%s","Im here");
    _state = kNextStepLoadAd;
    scheduleUpdate();
	
    return true;
}

void HelloWorld::LoadInterstitial()
{
//    #if defined(__ANDROID__)
//    // Android ad unit IDs.
//    const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";
//    #endif
//
//    // Create and initialize banner view.
//
//    interstitial_ad = new firebase::admob::InterstitialAd();
//    // my_ad_parent is a reference to an iOS UIView or an Android Activity.
//    // This is the parent UIView or Activity of the interstitial ad.
//    interstitial_ad->Initialize(getAdParent(), kInterstitialAdUnit);
//    
//    // Initialize all the AdRequest struct member values to zero.
//    firebase::admob::AdRequest my_ad_request = {};
//    interstitial_ad->LoadAd(my_ad_request);
}

void HelloWorld::LoadRewardedVideo()
{		
	if(firebase::admob::rewarded_video::InitializeLastResult().status() == firebase::kFutureStatusComplete &&
	   firebase::admob::rewarded_video::InitializeLastResult().error() == firebase::admob::kAdMobErrorNone)
	{
		firebase::admob::AdRequest my_ad_request = {};
		static const char* kTestDeviceIDs[] = {"bc5032b2a871da511332401af3ac6bb0"};
		//my_ad_request.test_device_id_count = sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
		//my_ad_request.test_device_ids = kTestDeviceIDs;
		//firebase::admob::rewarded_video::LoadAd("ca-app-pub-4335424038866907/6860135984", my_ad_request);
        firebase::admob::rewarded_video::LoadAd(kRewardedAdUnit, my_ad_request);
	}
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

bool g_adShown = false;

void HelloWorld::update(float delta) 
{
	// Uncomment this for interstitial
	/*if (interstitial_ad->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
		interstitial_ad->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) 
			interstitial_ad->Show();*/
	
	// Rewarded video	
//	if (firebase::admob::rewarded_video::LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
//		firebase::admob::rewarded_video::LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone)
//	{
//		if(!g_adShown)
//		{
//			firebase::admob::rewarded_video::Show(getAdParent());
//			g_adShown = true;
//		}
//	}

    CCLOG("===state[%d]", _state);

    switch(_state){
        case kNextStepLoadAd:
            if(firebase::admob::rewarded_video::InitializeLastResult().status() == firebase::kFutureStatusComplete &&
               firebase::admob::rewarded_video::InitializeLastResult().error() == firebase::admob::kAdMobErrorNone)
            {
                firebase::admob::AdRequest my_ad_request = {};
                static const char* kTestDeviceIDs[] = {"bc5032b2a871da511332401af3ac6bb0", "DD07D83E04F8C06551082531F7E2B8DB"};
                my_ad_request.test_device_id_count = sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
                my_ad_request.test_device_ids = kTestDeviceIDs;
                firebase::admob::rewarded_video::LoadAd("ca-app-pub-4335424038866907/6860135984", my_ad_request);
                //firebase::admob::rewarded_video::LoadAd(kRewardedAdUnit, my_ad_request);
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

void HelloWorld::initUnityAdsFunc()
{
    const char* gameId = "1055529"; // for Android

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    gameId = "1076532";
#endif
    
    UnityAdsInit(gameId, false);
}

void HelloWorld::showUnityAdsFunc(Ref* pSender)
{
    const char* zoneString = "rewardedVideo";
    
    if(UnityAdsIsReady(zoneString)) {
        UnityAdsShow(zoneString);
    } else {
        CCLOG("[UnityAds cpp test] yet cannot show");
    }
}

void HelloWorld::rewardPlayer(const char *placementId)
{

    CCLOG("[UnityAds cpp test] rewarded");
    const char* targetStr = "rewardedVideo";
    if(strcmp(placementId, targetStr) == 0){
        if(titleLabel){
            const char* text = "Congrats!";
            titleLabel->setString(text);
        }
    }
}
