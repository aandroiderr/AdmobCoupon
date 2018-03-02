//
//  AdmobManager.cpp
//  Components-mobile
//
//  Created by Ahmet Abak on 22.02.2018.
//

#include "AdmobManager.h"
#include "FirebaseHelper.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;
namespace rewarded_video = firebase::admob::rewarded_video;

// By Google
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
const char* kAdMobAppID = "ca-app-pub-3940256099942544~3347511713";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
const char* kAdMobAppID = "ca-app-pub-3940256099942544~1458002511";
#else
const char* kAdMobAppID = "";
#endif

//// These ad units are configured to always serve test ads.
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
////const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";//old
//const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/9147645139";//new
//const char* kRewardedVideoAdUnit = "ca-app-pub-4335424038866907/6860135984";
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//const char* kAdViewAdUnit = "ca-app-pub-3940256099942544/2934735716";
////const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/2130553480";//old
//const char* kInterstitialAdUnit = "ca-app-pub-4335424038866907/9147645139";//new
//const char* kRewardedVideoAdUnit = "ca-app-pub-4335424038866907/6860135984";
//#else
//const char* kAdViewAdUnit = "";
//const char* kInterstitialAdUnit = "";
//const char* kRewardedVideoAdUnit = "";
//#endif

#pragma VIDEO ADS CALLBACK
static void onRewardedVideoLoadAdCompletionCallback(
                                                    const firebase::Future<void>& future, void* userData) {
    
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        rewarded_video::Show(getAdParent());
    } else {
    }
}


class LoggingRewardedVideoListener
: public firebase::admob::rewarded_video::Listener {
private:
    AdmobManager * _manager;
    
public:
    LoggingRewardedVideoListener(AdmobManager * man):
    _manager(man)
    {}
    
    void OnRewarded(firebase::admob::rewarded_video::RewardItem reward) override {
        _manager->_isVideoShow = true;
    }
    
    void OnPresentationStateChanged(
                                    firebase::admob::rewarded_video::PresentationState state) override {
        
        if (state == rewarded_video::PresentationState::kPresentationStateHidden){
            if (_manager->_isVideoShow){
                _manager->_isVideoShow = false;
                //video close
                if (_manager->_delegate)
                    _manager->_delegate->AdmobManagerOnVideoClose();

            }
        }
    }
};


#pragma INTERSTITIAL CALLBACK
static void onInterstitialLoadAdCompletionCallback(
                                                    const firebase::Future<void>& future, void* userData) {
    AdmobManager* man = static_cast<AdmobManager*>(userData);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        man->_interstitialAds->Show();
        man->_isInterstitialShow = true;
    } else {
    }
}

class LoggingInterstitialListener : public firebase::admob::InterstitialAd::Listener{
private:
    AdmobManager * _manager;
    
public:
    
    LoggingInterstitialListener(AdmobManager * manager):
    _manager(manager)
    {
    }
    
    virtual void OnPresentationStateChanged(firebase::admob::InterstitialAd* interstitial_ad,
                                            firebase::admob::InterstitialAd::PresentationState state) {
        if (state == firebase::admob::InterstitialAd::PresentationState::kPresentationStateHidden){
            if (_manager->_isInterstitialShow){
                _manager->_isInterstitialShow = false;
                //Interstitial close
                if (_manager->_delegate)
                    _manager->_delegate->AdmobManagerOnInterstitialClose();
                _manager->resetInterstitialAds();
            }
        }
        
    }
};


#pragma AmobManager

AdmobManager * AdmobManager::_instance = nullptr;

AdmobManager::AdmobManager():
_isVideoShow(false),
_isInterstitialShow(false),
_interstitialListener(nullptr),
_interstitialAdsToDelete(nullptr),
_interstitialAds(nullptr),
_isInit(false),
_delegate(nullptr),
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
    
    _delegate = del;
    
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
        _rewardedVideoListener = new LoggingRewardedVideoListener(this);
    }
    rewarded_video::SetListener(_rewardedVideoListener);
    
    firebase::admob::rewarded_video::LoadAd(kRewardedVideoAdUnit, createAdRequest());
    rewarded_video::LoadAdLastResult().OnCompletion(onRewardedVideoLoadAdCompletionCallback, this);
}

void AdmobManager::showInterstitialAds(){
    if (!_interstitialListener){
        _interstitialListener = new LoggingInterstitialListener(this);
    }
    
    if (_interstitialAdsToDelete){
        delete _interstitialAdsToDelete;
        _interstitialAdsToDelete = nullptr;
    }
    
    _interstitialAds->SetListener(_interstitialListener);
    _interstitialAds->LoadAd(createAdRequest());
    _interstitialAds->LoadAdLastResult().OnCompletion(onInterstitialLoadAdCompletionCallback, this);
}

void AdmobManager::resetInterstitialAds(){
    if (_interstitialAdsToDelete){
        delete _interstitialAdsToDelete;
        _interstitialAdsToDelete = nullptr;
    }
    
    if (_interstitialAds){
        _interstitialAdsToDelete = _interstitialAds;
        _interstitialAds = new firebase::admob::InterstitialAd();
        _interstitialAds->Initialize(getAdParent(), kInterstitialAdUnit);
    }
}

