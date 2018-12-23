/*
 * JGDialogWindow.cpp
 *
 *  Created on: 23.12.2018
 *      Author: User
 *
 */

#include "JGDialogWindow.h"

#define FNTFONT_PATH "fonts/play_100_0.fnt"

JGDialogWindow::JGDialogWindow():
dialogWindowSelector_(nullptr),
dialogWindowTarget_(nullptr),
buyTag_(0)
{
	//CCLog("JGDialogWindow::JGDialogWindow()");
}


// создание класса для клетки
JGDialogWindow* JGDialogWindow::create(int type) {

	JGDialogWindow* windowSprite = new JGDialogWindow();
	if (windowSprite && windowSprite->init())
	{
		windowSprite->autorelease();
		windowSprite->loadTexture("images/windowdialog.png", UI_TEX_TYPE_PLIST);

		windowSprite->_addAllElements(type);
		return windowSprite;
	}
	CC_SAFE_DELETE(windowSprite);
	return NULL;
}


JGDialogWindow::~JGDialogWindow() {
	//CCLog("JGDialogWindow::~JGDialogWindow()");
}


void JGDialogWindow::addWindowsText(const char *text) {

	if (this->getNodeByTag(kWindowsTextTag))
		this->getNodeByTag(kWindowsTextTag)->removeFromParent();

	float posX = 0;
	float posY = 50;

	CCLabelBMFont *windowText = CCLabelBMFont::create(text, FNTFONT_PATH);
	windowText->setPosition(ccp(posX, posY));
	windowText->setAnchorPoint(ccp(0.5f, 0.5f));
	windowText->setScale(0.8f);
	windowText->setColor(ccBLACK);
	this->addNode(windowText, 1, kWindowsTextTag);

}


void JGDialogWindow::_addAllElements(int type) {

	if (type == 0) {
		float posX = 110;
		float posY = 50;
		UIImageView *yesImage = UIImageView::create();
		yesImage->loadTexture("images/yesbutton.png", UI_TEX_TYPE_PLIST);
		yesImage->setPosition(ccp(-posX, -posY));
		this->addChild(yesImage, 1, kImageButtonYesOkTag);
		yesImage->setTouchEnabled(true);
		yesImage->addTouchEventListener(this, toucheventselector(JGDialogWindow::_onTouchButton));

		UIImageView *noImage = UIImageView::create();
		noImage->loadTexture("images/nobutton.png", UI_TEX_TYPE_PLIST);
		noImage->setPosition(ccp(posX, -posY));
		this->addChild(noImage, 1, kImageButtonNoTag);
		noImage->setTouchEnabled(true);
		noImage->addTouchEventListener(this, toucheventselector(JGDialogWindow::_onTouchButton));
	}
	else {
		float posX = 0;
		float posY = 50;
		UIImageView *yesImage = UIImageView::create();
		yesImage->loadTexture("images/okbutton.png", UI_TEX_TYPE_PLIST);
		yesImage->setPosition(ccp(-posX, -posY));
		this->addChild(yesImage, 1, kImageButtonYesOkTag);
		yesImage->setTouchEnabled(true);
		yesImage->addTouchEventListener(this, toucheventselector(JGDialogWindow::_onTouchButton));
	}
}

void JGDialogWindow::_onTouchButton(CCObject *pSender, TouchEventType type) {

	UIImageView *image = static_cast<UIImageView *>(pSender);

	if (type == TOUCH_EVENT_BEGAN) {
		image->setScale(1.05f);
	}

	if (type == TOUCH_EVENT_ENDED) {
		image->setScale(1);
		if (dialogWindowTarget_ && dialogWindowSelector_)
			(dialogWindowTarget_->*dialogWindowSelector_)(image);
	}

	if (type == TOUCH_EVENT_CANCELED) {
		image->setScale(1);
	}

}
