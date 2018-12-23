#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "JGPoint.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;

#define COUNT_MAP_ELEMENT_X 8
#define COUNT_MAP_ELEMENT_Y 8


class JGGameScene : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
	CREATE_FUNC(JGGameScene);

private:
	void _addGoalsWin();
	void _addGoalsLeft();
	void _addMovesLeft();

	void _updateGoalsWin();
	void _updateGoalsLeft();
	void _updateMovesLeft();

	void _createDialodWindows(const char *text, int type);

	void _onTouchButtonBack(CCObject *pSender, TouchEventType type);
	void _onTouchPoint(CCObject *pSender, TouchEventType type);

	void _setPause(bool isPause);

	void _swapPoints(JGPoint *curPoint, JGPoint *lastPoint);
	
	void _callbackSwapPoints(CCNode *node);
	void _callbackStartAnimation(CCNode *node);
	void _createGrid();

	void _createNewPoints();
	void _addNewPoint(int mapPosX, int mapPosY);

	void _callbackPointMoveEnd(void *userData);
	void _callbackTouchDialogButton(void *userData);
	void _callbackPointMoveDelay(CCNode *node);

	void _sortAllFallPoints();
	JGPoint *_getPointOnGridPos(int fildPoinsX, int fildPoinsY);

	bool _isFindMatchInGrid(JGPoint *point);
	bool _isFindMatchInGrid(JGPoint *point, bool isChange);
	void _removeMatchInGrid();
	void _playRemoveAmination(JGPoint *point);

	void _loadMapFromFile(const char* file);
	UILayer *_getLayerByNodeTag(int nodeTag);

	int map_[COUNT_MAP_ELEMENT_X][COUNT_MAP_ELEMENT_Y];
	JGPoint *lastSelectPoint_;
	CCSize gridElementSize_;

	//bool isFall_;
	int stepFall_;

	int goalLeft_;
	int goalLeftMax_;

	int goalWin_;
	int goalWinMax_;

	int movesLeft_;

};

#endif // __HELLOWORLD_SCENE_H__
