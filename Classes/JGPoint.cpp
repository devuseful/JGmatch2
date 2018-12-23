/*
* GJPoint.cpp
*
*  Created on: 22.12..2018
*      Author: User
*/

#include "JGPoint.h"
#include "JGConstant.h"

#define SELL_OPACITY_TAG 150
#define ATLASFONT_PATH "fonts/play_100_atlas_bold.png"


JGPoint::JGPoint() :
pointTag_(0),
posInGridX_(-1),
posInGridY_(-1),
isMove_(false),
pointAppearSelector_(nullptr),
pointAppearTarget_(nullptr)
{
	// TODO Auto-generated constructor stub
	//CCLog("JGPoint()");
	gridElementSize_ = CCSizeZero;
}



JGPoint::~JGPoint() {
	// TODO Auto-generated destructor stub
	//CCLog("~JGPoint()");
}

JGPoint* JGPoint::create()
{
	JGPoint* pointSprite = new JGPoint();
	if (pointSprite && pointSprite->init())
	{
		pointSprite->autorelease();
		pointSprite->loadTexture("images/pointimage.png", UI_TEX_TYPE_PLIST);
		pointSprite->setColor(ccWHITE);
		pointSprite->setTouchEnabled(true);
		pointSprite->setZOrder(kPointZOrderTag);
		return pointSprite;
	}

	CC_SAFE_DELETE(pointSprite);
	return NULL;
}


void JGPoint::setPointTag(int pointTag) {
	pointTag_ = pointTag;
	this->_addLabelPointTag();
	this->_updatePointColor();
}


int JGPoint::getPointTag() {
	return pointTag_;
}


void JGPoint::setPositionInGrid(int posInGridX, int posInGridY) {
	posInGridX_ = posInGridX;
	posInGridY_ = posInGridY;

}

void JGPoint::setElementSize(CCSize gridElementSize) {
	gridElementSize_ = gridElementSize;
}

bool JGPoint::isPositionInGrid(int posInGridX, int posInGridY) {
	return (posInGridX_ == posInGridX && posInGridY_ == posInGridY);
}

int JGPoint::getPositionInGridX() {
	return posInGridX_;
}

int JGPoint::getPositionInGridY() {
	return posInGridY_;
}


void JGPoint::setPointSelectState(JGPointState pointState) {
	
	switch (pointState)
	{
	case kJGPointTouchBegan:
		this->_setSellImageColor(ccYELLOW);
		break;
	case kJGPointTouchEnd:
		this->_setSellImageColor(ccGREEN);
		break;
	default:
		this->_removeSellImage();
		break;
	}
}


bool JGPoint::isPointSelect() {
	return this->_isSellImage();
}


void JGPoint::_updatePointColor() {

	ccColor3B pointColor;
	switch (pointTag_)
	{
	case 1:
		pointColor = ccWHITE;
		break;
	case 2:
		pointColor = ccYELLOW;
		break;
	case 3:
		pointColor = ccBLUE;
		break;
	case 4:
		pointColor = ccRED;
		break;
	case 5:
		pointColor = ccMAGENTA;
		break;
	case 6:
		pointColor = ccORANGE;
		break;
	case 7:
		pointColor = ccGRAY;
		break;
	case 8:
		pointColor = { 255, 204, 170 };
		break;
	case 9:
		pointColor = { 255, 179, 128 };
		break;
	case 10:
		pointColor = { 255, 153, 85 };
		break;
	case 11:
		pointColor = { 255, 127, 42 };
		break;
	case 12:
		pointColor = { 255, 179, 128 };
		break;

	default:
		pointColor = ccWHITE;
		break;
	}
	this->setColor(pointColor);
}

void JGPoint::_addSellImage() {
	UIImageView *cellImage = UIImageView::create();
	cellImage->loadTexture("images/cellimage.png", UI_TEX_TYPE_PLIST);
	cellImage->setOpacity(SELL_OPACITY_TAG);
	this->addChild(cellImage, -kCellImageZOrderTag, kCellImageTag);
}


void JGPoint::_setSellImageColor(const ccColor3B &color) {
	if (!this->_isSellImage())
		this->_addSellImage();
	UIImageView *cellImage = static_cast<UIImageView*>(this->getChildByTag(kCellImageTag));
	if (cellImage)
		cellImage->setColor(color);
}


void JGPoint::_removeSellImage() {
	if (this->getChildByTag(kCellImageTag))
		this->removeChildByTag(kCellImageTag, true);
}


bool JGPoint::_isSellImage() {
	return (this->getChildByTag(kCellImageTag) != nullptr);
}


void JGPoint::_addLabelPointTag() {
	// Create power the label atlas
	UILabelAtlas* label = UILabelAtlas::create();
	label->setProperty(CCString::createWithFormat("%d", pointTag_)->getCString(), ATLASFONT_PATH, 50, 60, ".");
	label->setPosition(ccp(0, 1));
	label->setAnchorPoint(ccp(0.5f, 0.5f));
	label->setScale(0.5f);
	this->addChild(label, kPointLabelTag, kPointLabelTag);
}

bool JGPoint::isPointMove() {
	return isMove_;
}

void JGPoint::setPointMove(bool isMove) {
	isMove_ = isMove;
}


void JGPoint::movePointToGrid() {

	if (isMove_)
		return;

	this->setZOrder(kPointZOrderMoveTag);

	float startPosX = LEFT_EDGE_GRID;
	float startPosY = LOW_EDGE_GRID;

	float newPosX = startPosX + posInGridX_ *gridElementSize_.width;
	float newPosY = startPosY + posInGridY_ * gridElementSize_.height;

	float curPosY = this->getPositionY();
	float curPosX = this->getPositionX();

	float duration = fabs(0.002f * pow(pow(curPosX - newPosX, 2) + pow(curPosY - newPosY, 2.0f), 0.5));

	CCMoveTo *move = CCMoveTo::create(duration, ccp(newPosX, newPosY));
	CCSequence *sec = CCSequence::create(move, CCCallFuncN::create(this, callfuncN_selector(JGPoint::_callbackMovePoints)), nullptr);
	this->runAction(sec);
	isMove_ = true;
}


void JGPoint::fallPointToGrid() {

	if (isMove_)
		return;

	this->setZOrder(kPointZOrderMoveTag);

	float startPosX = LEFT_EDGE_GRID;
	float startPosY = LOW_EDGE_GRID;

	float newPosX = startPosX + posInGridX_ *gridElementSize_.width;
	float newPosY = startPosY + posInGridY_ * gridElementSize_.height;

	float curPosY = this->getPositionY();
	float curPosX = this->getPositionX();

	if (curPosY == newPosY || curPosX != newPosX)
		return;

	float duration = fabs(0.002f * (curPosY - newPosY));

	CCMoveTo *move = CCMoveTo::create(duration, ccp(newPosX, newPosY));
	CCSequence *sec = CCSequence::create(move, CCScaleTo::create(duration / 20, 1, 0.8f), CCScaleTo::create(duration / 10, 1, 1), CCCallFuncN::create(this, callfuncN_selector(JGPoint::_callbackMovePoints)), nullptr);
	this->runAction(sec);
	isMove_ = true;
}



void JGPoint::_callbackMovePoints(CCNode *node) {
	//CCLog("_callbackMovePoints");
	isMove_ = false;
	this->setZOrder(kPointZOrderTag);

	if (pointAppearTarget_ && pointAppearSelector_)
		(pointAppearTarget_->*pointAppearSelector_)(this);

}