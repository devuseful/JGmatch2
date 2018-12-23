/*
* GJPoint.h
*
*  Created on: 22.12..2018
*      Author: User
*/


#ifndef __JGPOINT_H__
#define __JGPOINT_H__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace gui;
using namespace std;


typedef void (cocos2d::CCObject::*SEL_PointAppearEvent)(void *userData);
#define pointAppearEvent_selector(_SELECTOR) (SEL_PointAppearEvent)(&_SELECTOR)


typedef enum tagJGPointState
{
	kJGPointTouchDisable,
	kJGPointTouchBegan,
	kJGPointTouchEnd,
} JGPointState;


class JGPoint : public UIImageView {

public:

	enum enumPointZOrder
	{
		kPointZOrderTag = 1,
		kPointZOrderMoveTag,
	};

	JGPoint();

	static JGPoint *create();

	void setPointTag(int pointTag);
	int JGPoint::getPointTag();
	void setPositionInGrid(int posX, int posY);
	void setElementSize(CCSize gridElementSize);
	bool isPositionInGrid(int posInGridX, int posInGridY);
	int getPositionInGridX();
	int getPositionInGridY();
	void setPointSelectState(JGPointState pointState);
	bool isPointSelect();
	
	bool isPointMove();
	void setPointMove(bool isMove);

	void movePointToGrid();
	void fallPointToGrid();

	void setWordAppearEvent(cocos2d::CCObject* target, SEL_PointAppearEvent selector) {
		pointAppearTarget_ = target;
		pointAppearSelector_ = selector;
	}

	virtual ~JGPoint();

protected:

	void _updatePointColor();
	void _addSellImage();
	void _setSellImageColor(const ccColor3B &color);
	void _removeSellImage();
	bool _isSellImage();

	void _addLabelPointTag();
	void _callbackMovePoints(CCNode *node);


	int pointTag_;
	int posInGridX_;
	int posInGridY_;

	bool isMove_;
	CCSize gridElementSize_;


	SEL_PointAppearEvent pointAppearSelector_;
	CCObject* pointAppearTarget_;

	enum enumPointImage
	{
		kCellImageTag = 1,
		kCellImageZOrderTag = 10,
		kPointLabelTag,
	};

};

#endif // __JGPOINT_H__
