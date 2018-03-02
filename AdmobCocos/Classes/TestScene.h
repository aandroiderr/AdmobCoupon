#ifndef __TestScene_SCENE_H__
#define __TestScene_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"

class TestScene : public cocos2d::Scene
{
private:
    HelloWorld * _couponWrapper;
    
public:
  
    static cocos2d::Scene* createScene();
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(TestScene);
};

#endif // __TestScene_SCENE_H__
