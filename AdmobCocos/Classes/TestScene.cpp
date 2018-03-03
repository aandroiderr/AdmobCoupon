#include "TestScene.h"

USING_NS_CC;

Scene* TestScene::createScene()
{
    return TestScene::create();
}


// on "init" you need to initialize your instance
bool TestScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    addChild(LayerColor::create(Color4B(113,102,225,255)));
    
    _couponWrapper = AdmobCouponWrapper::create();
    _couponWrapper->setDelegate(this);
    addChild(_couponWrapper);
    
    auto btn = ui::Button::create();
    btn->setTitleText("Show Interstitial");
    btn->setTitleFontSize(30);
    btn->setPosition(origin + Vec2(visibleSize.width/2, visibleSize.height/2 ));
    btn->addClickEventListener([this](Ref* obj){
        this->_couponWrapper->showInterstitial();
    });
    addChild(btn);

    
    return true;
}

#pragma mark AdManagerDelegate
void TestScene::AdmobManagerOnVideoClose(){
}

void TestScene::AdmobManagerOnInterstitialClose(){
    _couponWrapper->showCoupon();
}

