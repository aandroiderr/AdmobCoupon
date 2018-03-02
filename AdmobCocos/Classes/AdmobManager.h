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
    /**
        no coupons available to show.
     */
    virtual void AdmobManagerHasNoCouponsAvailable() =0;
    
    /**
        coupons on show.
     */
    virtual void AdmobManagerOnShow() =0;
    
    /**
        user clicked the coupon.
     */
    virtual void AdmobManagerOnClickToUnlock() =0;
    
    /**
        user clicked redeem and redirected to link
     */
    virtual void AdmobManagerOnRedirectToLink() =0;
    
    /**
        user closed the page
     */
    virtual void AdmobManagerOnClose() =0;
    
    /**
        stated loading ads
     */
    virtual void AdmobManagersOnRequest() =0;
};

class AdmobManager {
private:
    AdmobManager();
    static AdmobManager * _instance;
    AdmobManagerDelegate *delegate;
    bool _isInit;
    firebase::admob::InterstitialAd * _interstitialAds;
    
public:
    static AdmobManager * getInstance();
    
    firebase::admob::rewarded_video::Listener * _rewardedVideoListener;
    
    firebase::admob::AdRequest createAdRequest();
    
    void initialize(AdmobManagerDelegate *del = nullptr);
    
    void setDelegate(AdmobManagerDelegate *del);
    
    void showVideoAds();
    
    void showInterstitialAds();
    
};

#endif /* AdmobManager_hpp */
