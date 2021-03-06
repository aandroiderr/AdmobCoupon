#ifndef __TestScene_SCENE_H__
#define __TestScene_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "AdmobCouponWrapper.h"

class TestScene : public cocos2d::Scene, public AdmobManagerDelegate
{
private:
    AdmobCouponWrapper * _couponWrapper;
    
public:
  
    static cocos2d::Scene* createScene();
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);
    
    
    //AdmobManagerDelegate
    virtual void AdmobManagerOnInterstitialClose();
    virtual void AdmobManagerOnVideoClose();
};

#endif // __TestScene_SCENE_H__
