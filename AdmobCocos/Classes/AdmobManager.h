//
//  AdmobManager.hpp
//  Components-mobile
//
//  Created by KietLe
//

#ifndef AdmobManager_hpp
#define AdmobManager_hpp

#include "cocos2d.h"
#include "firebase/app.h"
#include "firebase/admob.h"
#include "firebase/admob/types.h"


struct AdmobManagerDelegate {
    virtual void AdmobManagerOnInterstitialClose() = 0;
    virtual void AdmobManagerOnVideoClose() = 0;
};

class AdmobManager {
private:
    AdmobManager();
    static AdmobManager * _instance;
    bool _isInit;
    
public:
    AdmobManagerDelegate * _delegate;
    
    //interstitial
    bool _isInterstitialShow;
    firebase::admob::InterstitialAd * _interstitialAds;
    firebase::admob::InterstitialAd * _interstitialAdsToDelete;
    firebase::admob::InterstitialAd::Listener * _interstitialListener;
    
    //video
    bool _isVideoShow;
    firebase::admob::rewarded_video::Listener * _rewardedVideoListener;
    
public:
    static AdmobManager * getInstance();
    
    firebase::admob::AdRequest createAdRequest();
    
    void initialize(AdmobManagerDelegate *del = nullptr);
    
    void setDelegate(AdmobManagerDelegate *del);
    
    void showVideoAds();
    
    void showInterstitialAds();
    
    void resetInterstitialAds();
    
};

#endif /* AdmobManager_hpp */
