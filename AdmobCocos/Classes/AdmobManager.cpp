//
//  AdmobManager.cpp
//  Components-mobile
//
//  Created by Ahmet Abak on 22.02.2018.
//

#include "AdmobManager.h"
#include "FirebaseHelper.h"
#include "external/json/document.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include <string>
#include <fstream>

namespace rewarded_video = firebase::admob::rewarded_video;

// The AdMob app IDs.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdMobAppID = "ca-app-pub-4335424038866907~2928103176";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdMobAppID = "ca-app-pub-4335424038866907~2928103176";
#else
const char* kAdMobAppID = "";
#endif

// These ad units are configured to always serve test ads.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";
const char* kRewardedVideoAdUnit = "ca-app-pub-4335424038866907/6860135984";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";
const char* kRewardedVideoAdUnit = "ca-app-pub-4335424038866907/6860135984";
#else
const char* kAdViewAdUnit = "";
const char* kInterstitialAdUnit = "";
const char* kRewardedVideoAdUnit = "";
#endif

#pragma VIDEO ADS CALLBACK
static void onRewardedVideoLoadAdCompletionCallback(
                                                    const firebase::Future<void>& future, void* userData) {
    
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        //log("Loading rewarded video completed successfully.");
        rewarded_video::Show(getAdParent());
    } else {
    }
}


class LoggingRewardedVideoListener
: public firebase::admob::rewarded_video::Listener {
public:
    LoggingRewardedVideoListener() {}
    void OnRewarded(firebase::admob::rewarded_video::RewardItem reward) override {
        //log("Rewarding user with %f %s.", reward.amount,
        //    reward.reward_type.c_str());
    }
    void OnPresentationStateChanged(
                                    firebase::admob::rewarded_video::PresentationState state) override {
        //log("Rewarded video PresentationState has changed to %d.", state);
    }
};


#pragma INTERSTITIAL CALLBACK
static void onInterstitialLoadAdCompletionCallback(
                                                    const firebase::Future<void>& future, void* userData) {
    firebase::admob::InterstitialAd * ads = static_cast<firebase::admob::InterstitialAd*>(userData);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        ads->Show();
    } else {
    }
}

class LoggingInterstitialListener : public firebase::admob::InterstitialAd::Listener{
    
    virtual void OnPresentationStateChanged(firebase::admob::InterstitialAd* interstitial_ad,
                                            firebase::admob::InterstitialAd::PresentationState state) {
        
    }
};


#pragma AmobManager

AdmobManager * AdmobManager::_instance = nullptr;

AdmobManager::AdmobManager():
_interstitialAds(nullptr),
_isInit(false),
_rewardedVideoListener(nullptr)
{
}

AdmobManager* AdmobManager::getInstance(){
    if (_instance == nullptr){
        _instance = new AdmobManager();
    }
    return  _instance;
}

void AdmobManager::initialize(AdmobManagerDelegate *del) {
    if (_isInit) return;
    _isInit = true;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Initialize Firebase for Android.
    firebase::App* app = firebase::App::Create(
                                               firebase::AppOptions(), JniHelper::getEnv(), JniHelper::getActivity());
    // Initialize AdMob.
    firebase::admob::Initialize(*app, kAdMobAppID);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // Initialize Firebase for iOS.
    firebase::App* app = firebase::App::Create(firebase::AppOptions());
    firebase::admob::Initialize(*app, kAdMobAppID);
#endif
    

    //init reward video
    firebase::admob::rewarded_video::Initialize();
    
    //init interstitial
    _interstitialAds = new firebase::admob::InterstitialAd();
    _interstitialAds->Initialize(getAdParent(), kInterstitialAdUnit);
}

void AdmobManager::setDelegate(AdmobManagerDelegate *del) {
}

firebase::admob::AdRequest AdmobManager::createAdRequest(){
    firebase::admob::AdRequest my_ad_request = {};
    static const char* kTestDeviceIDs[] = {
        "bc5032b2a871da511332401af3ac6bb0",
        "DD07D83E04F8C06551082531F7E2B8DB"};
    my_ad_request.test_device_id_count = sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
    my_ad_request.test_device_ids = kTestDeviceIDs;
    return my_ad_request;
}

void AdmobManager::showVideoAds(){
    if (!_rewardedVideoListener){
        _rewardedVideoListener = new LoggingRewardedVideoListener();
    }
    rewarded_video::SetListener(_rewardedVideoListener);
    
    firebase::admob::rewarded_video::LoadAd(kRewardedVideoAdUnit, createAdRequest());
    rewarded_video::LoadAdLastResult().OnCompletion(onRewardedVideoLoadAdCompletionCallback, this);
}

void AdmobManager::showInterstitialAds(){
    
    //_interstitialAds->SetListener();
    _interstitialAds->LoadAd(createAdRequest());
    _interstitialAds->LoadAdLastResult().OnCompletion(onInterstitialLoadAdCompletionCallback, _interstitialAds);
}

