/*
 * JGDialogWindow.h
 *
 *  Created on: 23.12.2018
 *      Author: User
 */


#ifndef __JGDialogWindow_H__
#define __JGDialogWindow_H__


#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace gui;


typedef void (cocos2d::CCObject::*SEL_DialogWindowEvent)(void *userData);
#define pointDialogWindow_selector(_SELECTOR) (SEL_DialogWindowEvent)(&_SELECTOR)


class JGDialogWindow : public UIImageView
{
public:

	enum enumDialogWindow
	{
		kImageButtonYesOkTag = 1,
		kImageButtonNoTag,
		kWindowsTextTag,
	};

	JGDialogWindow();
	static JGDialogWindow* create(int type);

	void addWindowsText(const char *text);

	void setDialogWindowEvent(CCObject* target, SEL_DialogWindowEvent selector) {
		dialogWindowTarget_ = target;
		dialogWindowSelector_ = selector;
	}

	virtual ~JGDialogWindow();

protected:

	void _addAllElements(int type);
	void _callbackImageAction(UIImageView* imageTouch, int eventTag);

	void _onTouchButton(CCObject *pSender, TouchEventType type);

	SEL_DialogWindowEvent dialogWindowSelector_;
	CCObject *dialogWindowTarget_;

	int buyTag_;
};



#endif // __JGDialogWindow_H__
