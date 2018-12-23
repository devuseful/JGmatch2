#include "JGMenuScene.h"
#include "JGConstant.h"
#include "JGGameScene.h"

#define NODE 10003
#define SCENE_TAG 0
#define SCENE_LAYER_TAG 0
#define SCENE_NODE_TAG 3
#define COUNT_MAP_ELEMENT_X 8
#define COUNT_MAP_ELEMENT_Y 8
#define ATLASFONT_PATH "fonts/play_100_atlas_bold.png"

CCScene* JGMenuScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	scene->setTag(SCENE_TAG);
    
    // 'layer' is an autorelease object
	JGMenuScene *layer = JGMenuScene::create();

    // add layer as a child to scene
	scene->addChild(layer, 0, SCENE_LAYER_TAG);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JGMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("graphics/Elements0.plist");

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCNode *pNode = SceneReader::sharedSceneReader()->createNodeWithSceneFile("scene/menu/JGmatch2Menu.json"); //cocos2dx 2.2
	this->addChild(pNode, 0, SCENE_NODE_TAG);
	// установка маштаба
	pNode->setScaleY(winSize.height / TOP_EDGE_DESCTOP);
	pNode->setScaleX(winSize.width / RIGHT_EDGE_DESCTOP);


	std::string levels = CCUserDefault::sharedUserDefault()->getStringForKey("levels", "0000");

	CCNode *menuNode = pNode->getChildByTag(NODE);
	UILayer *menuLayer = static_cast<UILayer*>(menuNode->getChildren()->objectAtIndex(0));
	for (size_t i = 0; i < 4; i++) {

		CCString *numberLevel = CCString::createWithFormat("ImageViewLevel%d", i + 1);
		UIImageView *imageLevel = static_cast<UIImageView*>(menuLayer->getWidgetByName(numberLevel->getCString()));
		imageLevel->addTouchEventListener(this, toucheventselector(JGMenuScene::_onTouchButtonLevel));

		UILabelAtlas* label = UILabelAtlas::create();
		label->setProperty(numberLevel->getCString(), ATLASFONT_PATH, 50, 60, ".");
		label->setProperty(CCString::createWithFormat("%d", i + 1)->getCString(), ATLASFONT_PATH, 50, 60, ".");
		label->setPosition(ccp(0, 0));
		label->setAnchorPoint(ccp(0.5f, 0.5f));
		label->setScale(0.5f);
		imageLevel->addChild(label, 10);

		bool isCheck = (levels[i] - *"0") != 0;
		imageLevel->getChildByTag(1)->setVisible(isCheck);
	}


	float delayTime = 0.1f;
	CCSequence* seq = CCSequence::create(CCDelayTime::create(delayTime), CCCallFuncN::create(this, callfuncN_selector(JGMenuScene::_callbackStartAnimation)), nullptr);
	this->runAction(seq);

    return true;
}



void JGMenuScene::_callbackStartAnimation(CCNode * node) {

}


void JGMenuScene::_onTouchButtonLevel(CCObject *pSender, TouchEventType type) {

	UIImageView *imageLevel = static_cast<UIImageView*>(pSender);
	if (type == TOUCH_EVENT_BEGAN) {
		imageLevel->setScale(1.05f);
	}

	if (type == TOUCH_EVENT_ENDED) {
		imageLevel->setScale(1);

		CCUserDefault::sharedUserDefault()->setIntegerForKey("levelTag", imageLevel->getTag());
		CCScene *scene = JGGameScene::scene();
		//scene->setTag(1);
		CCScene* pScene = CCTransitionFade::create(0.1f, scene);
		if (pScene) {
			CCDirector::sharedDirector()->replaceScene(pScene);
		}

	}

	if (type == TOUCH_EVENT_CANCELED) {
		imageLevel->setScale(1);
	}
}