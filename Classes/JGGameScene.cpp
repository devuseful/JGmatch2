#include "JGGameScene.h"
#include "JGConstant.h"
#include "JGDialogWindow.h"

#define TOP_NODE 10003
#define CENTER_NODE 10004
#define SCENE_TAG 1
#define SCENE_LAYER_TAG 0
#define SCENE_NODE_TAG 3
#define SHIFT_HEIGHT 700
#define DIALOG_WINDOW_POSX 500
#define DIALOG_WINDOW_POSY 400
#define MAX_POINT_TYPE 12
#define REMOVE_POINT_ORDER 100


CCScene* JGGameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
	scene->setTag(SCENE_TAG);
    
    // 'layer' is an autorelease object
	JGGameScene *layer = JGGameScene::create();

    // add layer as a child to scene
	scene->addChild(layer, 0, SCENE_LAYER_TAG);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JGGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	lastSelectPoint_ = nullptr;
	gridElementSize_ = CCSizeZero;
	stepFall_ = 0;
	goalLeft_ = 0;
	goalLeftMax_ = GOAL_LEFT_MAX;
	goalWin_ = 0;
	goalWinMax_ = GOAL_WIN_MAX;
	movesLeft_ = 0;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCNode *pNode = SceneReader::sharedSceneReader()->createNodeWithSceneFile("scene/game/JGmatch2Game.json"); //cocos2dx 2.2
	this->addChild(pNode, 0, SCENE_NODE_TAG);
	// установка маштаба
	pNode->setScaleY(winSize.height / TOP_EDGE_DESCTOP);
	pNode->setScaleX(winSize.width / RIGHT_EDGE_DESCTOP);

	CCNode *topNode = pNode->getChildByTag(TOP_NODE);
	UILayer *topLayer = static_cast<UILayer*>(topNode->getChildren()->objectAtIndex(0));
	topLayer->getWidgetByName("ButtonBack")->addTouchEventListener(this, toucheventselector(JGGameScene::_onTouchButtonBack));
 
	float delayTime = 0.1f;
	CCSequence* seq = CCSequence::create(CCDelayTime::create(delayTime), CCCallFuncN::create(this, callfuncN_selector(JGGameScene::_callbackStartAnimation)), nullptr);
	this->runAction(seq);

    return true;
}

void JGGameScene::_addGoalsWin() {
	++goalWin_;
	this->_updateGoalsWin();

	if (goalWin_ >= goalWinMax_) {
		std::string levels = CCUserDefault::sharedUserDefault()->getStringForKey("levels", "0000");
		int levelTag = CCUserDefault::sharedUserDefault()->getIntegerForKey("levelTag", 1);
		levels[levelTag - 1] = *"1";
		CCUserDefault::sharedUserDefault()->setStringForKey("levels", levels.c_str());

		this->_createDialodWindows("Yuo win", 1);
	}
}

void JGGameScene::_addGoalsLeft() {
	++goalLeft_;
	this->_updateGoalsLeft();

	if (goalLeft_ >= goalLeftMax_) {
		this->_createDialodWindows("You lose", 1);
	}
}

void JGGameScene::_addMovesLeft() {
	++movesLeft_;
	this->_updateMovesLeft();
}

void JGGameScene::_updateGoalsWin() {
	UILayer *topLayer = _getLayerByNodeTag(TOP_NODE);
	LabelBMFont *labelGoalsWin = static_cast<LabelBMFont *>(topLayer->getWidgetByName("LabelBMFontGoalWinValue"));
	labelGoalsWin->setText(CCString::createWithFormat("%d/%d", goalWin_, goalWinMax_)->getCString());
}

void JGGameScene::_updateGoalsLeft() {
	UILayer *topLayer = _getLayerByNodeTag(TOP_NODE);
	LabelBMFont *labelGoalsLeft = static_cast<LabelBMFont *>(topLayer->getWidgetByName("LabelBMFontGoalLeftValue"));
	labelGoalsLeft->setText(CCString::createWithFormat("%d/%d", goalLeft_, goalLeftMax_)->getCString());
}

void JGGameScene::_updateMovesLeft() {
	UILayer *topLayer = _getLayerByNodeTag(TOP_NODE);
	LabelBMFont *labelMovesLeft = static_cast<LabelBMFont *>(topLayer->getWidgetByName("LabelBMFontMovesLeftValue"));
	labelMovesLeft->setText(CCString::createWithFormat("%d", movesLeft_)->getCString());
}


void JGGameScene::_onTouchButtonBack(CCObject *pSender, TouchEventType type) {
	
	if (type == TOUCH_EVENT_BEGAN) {

	}

	if (type == TOUCH_EVENT_ENDED) {
		this->_createDialodWindows("Quit game?", 0);
	}

	if (type == TOUCH_EVENT_CANCELED) {

	}
}


void JGGameScene::_createDialodWindows(const char *text, int type) {

	JGDialogWindow *window = JGDialogWindow::create(type);
	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelPoints = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));

	UILayer *topLayer = _getLayerByNodeTag(TOP_NODE);
	topLayer->getWidgetByName("ButtonBack")->setEnabled(false);

	window->setPosition(ccp(DIALOG_WINDOW_POSX, DIALOG_WINDOW_POSY));
	window->addWindowsText(text);
	panelPoints->addChild(window, kDialogWindowTag, kDialogWindowTag);
	this->_setPause(true);
	window->setDialogWindowEvent(this, pointDialogWindow_selector(JGGameScene::_callbackTouchDialogButton));

}


void JGGameScene::_setPause(bool isPause) {

	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));
	CCArray *points = panelCenter->getChildren();
	for (size_t i = 0; i < points->count(); i++) {
		JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
		if (point) {
			if (isPause) {
				point->pauseSchedulerAndActions();
			}
			else
				point->resumeSchedulerAndActions();

			point->setTouchEnabled(!isPause);
		}
	}

	if (isPause)
		this->stopAllActions();
	else
		this->resumeSchedulerAndActions();
}


void JGGameScene::_onTouchPoint(CCObject *pSender, TouchEventType type) {
	
	JGPoint *curPoint = static_cast<JGPoint*>(pSender);
	if (type == TOUCH_EVENT_BEGAN) {
		curPoint->setPointSelectState(kJGPointTouchBegan);
	}

	if (type == TOUCH_EVENT_ENDED) {

		curPoint->setPointSelectState(kJGPointTouchEnd);

		if (lastSelectPoint_) {
			lastSelectPoint_->setPointSelectState(kJGPointTouchDisable);
			if (curPoint == lastSelectPoint_) {
				lastSelectPoint_ = nullptr;
				curPoint->setPointSelectState(kJGPointTouchDisable);
			}
		}
		else {
			lastSelectPoint_ = curPoint;
			return;
		}

		this->_swapPoints(lastSelectPoint_, curPoint);

	}
		
	if (type == TOUCH_EVENT_CANCELED) {
		curPoint->setPointSelectState(kJGPointTouchDisable);
	}
}


void JGGameScene::_swapPoints(JGPoint *curPoint, JGPoint *lastPoint) {
	if (!curPoint || !lastPoint || curPoint == lastPoint)
		return;

	lastPoint->setPointSelectState(kJGPointTouchDisable);
	curPoint->setPointSelectState(kJGPointTouchDisable);

	int curPosInGridX = curPoint->getPositionInGridX();
	int curPosInGridY = curPoint->getPositionInGridY();

	int lastPosInGridX = lastPoint->getPositionInGridX();
	int lastPosInGridY = lastPoint->getPositionInGridY();

	int curGrid = map_[curPosInGridX][curPosInGridY];
	int lastGrid = map_[lastPosInGridX][lastPosInGridY];

	map_[curPosInGridX][curPosInGridY] = lastGrid;
	map_[lastPosInGridX][lastPosInGridY] = curGrid;

	curPoint->setPositionInGrid(lastPosInGridX, lastPosInGridY);
	lastPoint->setPositionInGrid(curPosInGridX, curPosInGridY);

	if (this->_isFindMatchInGrid(curPoint, false) || this->_isFindMatchInGrid(lastPoint, false)) {
		lastPoint->movePointToGrid();
		curPoint->movePointToGrid();
	}
	else {
		map_[curPosInGridX][curPosInGridY] = curGrid;
		map_[lastPosInGridX][lastPosInGridY] = lastGrid;

		lastPoint->setPositionInGrid(lastPosInGridX, lastPosInGridY);
		curPoint->setPositionInGrid(curPosInGridX, curPosInGridY);

		float duration = 0.2f;
		CCMoveTo *lastPointMove = CCMoveTo::create(duration, curPoint->getPosition());
		CCMoveTo *curPointMove = CCMoveTo::create(duration, lastPoint->getPosition());
		curPoint->setZOrder(JGPoint::kPointZOrderMoveTag);
		CCSequence *sec = CCSequence::create(curPointMove, lastPointMove, CCCallFuncN::create(this, callfuncN_selector(JGGameScene::_callbackSwapPoints)), nullptr);
		curPoint->runAction(CCEaseBounce::create(sec));
	}

	this->_addMovesLeft();
	lastSelectPoint_ = nullptr;
}

void JGGameScene::_callbackSwapPoints(CCNode *node) {
	JGPoint *curPoint = static_cast<JGPoint *>(node);
	curPoint->setZOrder(JGPoint::kPointZOrderTag);
	this->_addGoalsLeft();
}


void JGGameScene::_callbackStartAnimation(CCNode * node) {

	if (CCDirector::sharedDirector()->getRunningScene()->getTag() != SCENE_TAG) {
		//CCLog("scene not load!!!");
		CCSequence* seq = CCSequence::create(CCDelayTime::create(0.1f), CCCallFuncN::create(this, callfuncN_selector(JGGameScene::_callbackStartAnimation)), NULL);
		this->runAction(seq);
		return;
	}

	this->_updateGoalsWin();
	this->_updateGoalsLeft();
	this->_updateMovesLeft();
	this->_createGrid();
}


void JGGameScene::_createGrid() {

	int levelTag =  CCUserDefault::sharedUserDefault()->getIntegerForKey("levelTag", 1);

	this->_loadMapFromFile(CCString::createWithFormat("levels/level%d.txt", levelTag)->getCString());
	float startPosX = LEFT_EDGE_GRID;
	float startPosY = LOW_EDGE_GRID;

	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelCenter"));

	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {

		for (int j = 0; j < COUNT_MAP_ELEMENT_X; ++j) {
			
			UIImageView *squareImage = UIImageView::create();
			squareImage->loadTexture("images/squareimage.png", UI_TEX_TYPE_PLIST);

			gridElementSize_ = squareImage->getContentSize();
			squareImage->setPosition(ccp(startPosX + j * gridElementSize_.width, startPosY + i * gridElementSize_.height));
			if (map_[j][i]) {
				this->_addNewPoint(j, i);
				panelCenter->addChild(squareImage, kGridElenetTag);
			}
			else {
				map_[j][i] = kHoleTag;
			}
		}
	}
}


void JGGameScene::_createNewPoints() {

	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {

		for (int j = 0; j < COUNT_MAP_ELEMENT_X; ++j) {
			
			if (map_[j][i] == kCreateTag)
				this->_addNewPoint(j, i);
		}
	}
}



void JGGameScene::_addNewPoint(int mapPosX, int mapPosY) {
	
	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));

	float startPosX = LEFT_EDGE_GRID;
	float startPosY = LOW_EDGE_GRID;

	JGPoint *point = JGPoint::create();
	point->setElementSize(gridElementSize_);
	int pointTag = rand() % MAX_POINT_TYPE + 1;
	map_[mapPosX][mapPosY] = pointTag + kCreateTag;
	point->setPointTag(pointTag);
	point->setPosition(ccp(startPosX + mapPosX * gridElementSize_.width, startPosY + mapPosY * gridElementSize_.height + SHIFT_HEIGHT));
	point->setPositionInGrid(mapPosX, mapPosY);
	point->fallPointToGrid();
	panelCenter->addChild(point);
	point->setTouchEnabled(true);
	point->addTouchEventListener(this, toucheventselector(JGGameScene::_onTouchPoint));
	point->setWordAppearEvent(this, pointAppearEvent_selector(JGGameScene::_callbackPointMoveEnd));
}



void JGGameScene::_callbackPointMoveEnd(void *userData) {

	JGPoint *point = static_cast<JGPoint *>(userData);
	int curPosInGridX = point->getPositionInGridX();
	int curPosInGridY = point->getPositionInGridY();
	if (map_[curPosInGridX][curPosInGridY] > kCreateTag)
		map_[curPosInGridX][curPosInGridY] -= kCreateTag;

	stepFall_++;

	float delayTime = 0.01f;
	CCSequence* seq = CCSequence::create(CCDelayTime::create(delayTime), CCCallFuncN::create(this, callfuncN_selector(JGGameScene::_callbackPointMoveDelay)), nullptr);
	this->runAction(seq);
}


void JGGameScene::_callbackTouchDialogButton(void *userData) {

	UIImageView *button = static_cast<UIImageView *>(userData);

	if (button->getTag() == JGDialogWindow::kImageButtonYesOkTag)
		this->menuCloseCallback(this);
	else {
		this->_setPause(false);
		button->getParent()->removeFromParent();

		UILayer *topLayer = _getLayerByNodeTag(TOP_NODE);
		topLayer->getWidgetByName("ButtonBack")->setEnabled(true);
	}
}


void JGGameScene::_callbackPointMoveDelay(CCNode *node) {

	stepFall_--;

	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));
	CCArray *points = panelCenter->getChildren();

	bool isMove = false;

	for (size_t i = 0; i < points->count(); i++) {
		JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
		if (point) {
			if (this->_isFindMatchInGrid(point))
				this->_removeMatchInGrid();
		}
	}


	for (size_t i = 0; i < points->count(); i++) {
		JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
		if (point) {
			if (point->isPointMove() && point->getTag() < kCreateTag)
				isMove = true;
		}
	}

	if (isMove)
		return;

	this->_sortAllFallPoints();

	if (!stepFall_) {
		UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
		UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));
		CCArray *points = panelCenter->getChildren();

		for (size_t i = 0; i < points->count(); i++) {
			JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
			if (point) {
				point->fallPointToGrid();
			}
		}

		for (size_t i = 0; i < points->count(); i++) {
			JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
			if (point) {
				if (point->isPointMove())
					isMove = true;
			}
		}

		if (!isMove)
			this->_createNewPoints();
	}

}



JGPoint *JGGameScene::_getPointOnGridPos(int fildPoinsX, int fildPoinsY) {

	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));
	CCArray *points = panelCenter->getChildren();
	for (size_t i = 0; i < points->count(); i++) {
		JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
		if (point) {
			if (point->isPositionInGrid(fildPoinsX, fildPoinsY))
				return point;
		}
	}
	return  nullptr;
}

bool JGGameScene::_isFindMatchInGrid(JGPoint *point) {
	return _isFindMatchInGrid(point, true);
}


bool JGGameScene::_isFindMatchInGrid(JGPoint *point, bool isChange) {

	bool find = false;
	if (!point)
		return find;

	const int stepCount = 4;

	int pieceX[stepCount] = { -1, 0, 0, 1 };
	int pieceY[stepCount] = { 0, 1, -1, 0 };

	int posX = point->getPositionInGridX();
	int posY = point->getPositionInGridY();

	//if (posX < 0 || posY < 0)
	//	CCLog("!!!");

	int pointTag = map_[posX][posY];
	if (pointTag > kHoleTag)
		return find;

	int i = 0;
	while (i < stepCount) {
		int x = posX + pieceX[i];
		int y = posY + pieceY[i];

		if (x >= 0 && y >= 0 && x < COUNT_MAP_ELEMENT_X && y < COUNT_MAP_ELEMENT_Y) {

			if (map_[x][y] == pointTag) {

				if (isChange) {
					map_[x][y] = kMatchTag;
					map_[posX][posY] = kMatchTag;
				}
				find = true;
			}
		}
		++i;
	}

	return find;
}


void JGGameScene::_sortAllFallPoints() {

	for (int i = 0; i < COUNT_MAP_ELEMENT_Y; ++i) {

		for (int j = 0; j < COUNT_MAP_ELEMENT_X; ++j) {

			if (map_[j][i] == kMatchTag) {
				map_[j][i] = kCreateTag;
			}
		}
	}

	for (int emptyX = 0; emptyX < COUNT_MAP_ELEMENT_X; emptyX++) {

		int emptyY = 0;
		for (int j = emptyY + 1; j < COUNT_MAP_ELEMENT_Y; j++) {

			if (map_[emptyX][j] != kHoleTag) {

				if (map_[emptyX][emptyY] == kCreateTag && emptyY < j) {
					int temp = map_[emptyX][j];
					map_[emptyX][j] = map_[emptyX][emptyY];
					map_[emptyX][emptyY] = temp;

					JGPoint *point = _getPointOnGridPos(emptyX, j);
					if (point) {
						point->setPositionInGrid(emptyX, emptyY);
					}
				}
				else if (map_[emptyX][j - 1] == kCreateTag && emptyY < j) {
					emptyY = j - 1;

					int temp = map_[emptyX][j];
					map_[emptyX][j] = map_[emptyX][emptyY];
					map_[emptyX][emptyY] = temp;

					JGPoint *point = _getPointOnGridPos(emptyX, j);
					if (point) {
						point->setPositionInGrid(emptyX, emptyY);
					}
				}

				++emptyY;
			}
		}
	}
}


void JGGameScene::_removeMatchInGrid() {

	std::vector<pair <int, int> >fallPoins;
	std::vector<JGPoint *> pointsRemove;

	UILayer *centerLayer = _getLayerByNodeTag(CENTER_NODE);
	UIPanel *panelCenter = static_cast<UIPanel *>(centerLayer->getWidgetByName("PanelPoints"));
	CCArray *points = panelCenter->getChildren();
	for (size_t i = 0; i < points->count(); i++) {

		JGPoint *point = dynamic_cast<JGPoint*>(points->objectAtIndex(i));
		if (point) {

			int posX = point->getPositionInGridX();
			int posY = point->getPositionInGridY();
			if (map_[posX][posY] == kMatchTag) {

				this->_playRemoveAmination(point);
				point->removeFromParent();

				map_[posX][posY] = kCreateTag;
				fallPoins.push_back(pair <int, int>(posX, posY));
				--i;
			}
		}
	}

}


void JGGameScene::_playRemoveAmination(JGPoint *point) {
	float duration = 0.15f;

	UIImageView *removePoint = UIImageView::create();
	removePoint->loadTexture("images/pointimage.png", UI_TEX_TYPE_PLIST);
	removePoint->setColor(point->getColor());

	point->getParent()->addChild(removePoint, REMOVE_POINT_ORDER);
	removePoint->setPosition(point->getPosition());

	CCSpawn *spawn = CCSpawn::create(CCScaleTo::create(duration, 5), CCFadeOut::create(duration), nullptr);
	CCSequence *seq = CCSequence::create(spawn, CCRemoveSelf::create(), nullptr);
	removePoint->runAction(seq);

	this->_addGoalsWin();

	lastSelectPoint_ = nullptr;
}


void JGGameScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}



// функция загрузки уровней из текстовых файлов
void JGGameScene::_loadMapFromFile(const char* file) {
	unsigned long size = 0;
	unsigned char *pData = CCFileUtils::sharedFileUtils()->getFileData(file, "r", &size);

	int j = 0;
	int i = 0;
	for (size_t k = 0; k < size; ++k, ++i)
	{
		if (pData[k] == '\n') {
			i = -1;
			++j;
		}
		else
			map_[i][j] = pData[k] - '0';// == 0 ? kCellFreeTag : kCellNotFreeTag;
	}

	delete[] pData;
	pData = NULL;
}


// функция возвращает слой по тегу
UILayer *JGGameScene::_getLayerByNodeTag(int nodeTag) {

	if (CCDirector::sharedDirector()->getRunningScene()->getTag() != SCENE_TAG)
		return nullptr;

	CCNode *sceneLayer = CCDirector::sharedDirector()->getRunningScene()->getChildByTag(SCENE_LAYER_TAG);
	if (!sceneLayer)
		return nullptr;

	CCNode *node = sceneLayer->getChildByTag(SCENE_NODE_TAG)->getChildByTag(nodeTag);
	if (!node)
		return nullptr;

	return static_cast<UILayer *>(node->getChildren()->objectAtIndex(0));
}