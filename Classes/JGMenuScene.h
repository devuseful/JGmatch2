#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "JGPoint.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

class JGMenuScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
	CREATE_FUNC(JGMenuScene);

private:

	void _callbackStartAnimation(CCNode * node);
	void _onTouchButtonLevel(CCObject *pSender, TouchEventType type);

};

#endif // __MENU_SCENE_H__
