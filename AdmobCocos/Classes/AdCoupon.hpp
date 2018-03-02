//
//  AdCoupon.hpp
//  Components-mobile
//
//  Created by Ahmet Abak on 22.02.2018.
//

#ifndef AdCoupon_hpp
#define AdCoupon_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

struct AdCouponDelegate {
    /**
        no coupons available to show.
     */
    virtual void adCouponHasNoCouponsAvailable() =0;
    
    /**
        coupons on show.
     */
    virtual void adCouponOnShow() =0;
    
    /**
        user clicked the coupon.
     */
    virtual void adCouponOnClickToUnlock() =0;
    
    /**
        user clicked redeem and redirected to link
     */
    virtual void adCouponOnRedirectToLink() =0;
    
    /**
        user closed the page
     */
    virtual void adCouponOnClose() =0;
    
    /**
        stated loading ads
     */
    virtual void adCouponsOnRequest() =0;
};

class AdCoupon {
private:
    //do not define this methods.
    AdCoupon(const AdCoupon& rhs);
    void operator=(const AdCoupon& rhs);
    AdCoupon() {};
    AdCouponDelegate *delegate;
    static AdCoupon& getInstance() {
        static AdCoupon coupon;
        return coupon;
    }
public:
    /**
        Starts coupons daemon. you can also set AdCouponDelegate in initialize()
    */
    static void initialize(AdCouponDelegate *del = nullptr);
    
    /**
        You can set delegate anytime, by sending nullptr as an argument,
        you can revoke delegate.
    */
    static void setDelegate(AdCouponDelegate *del);
    
    /**
        This will show the coupons page if there are coupons available.
     */
    static void showCoupons();
    
    /**
        Show redeem coupon
     */
    static void showRedeemCoupon();
};

#endif /* AdCoupon_hpp */
