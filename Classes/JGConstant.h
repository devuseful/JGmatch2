#ifndef __COSTANT_H__
#define __COSTANT_H__

#define LEFT_EDGE_DESCTOP 0
#define RIGHT_EDGE_DESCTOP 1024
#define LOW_EDGE_DESCTOP 0
#define TOP_EDGE_DESCTOP 768

#define LEFT_EDGE_GRID 250
#define LOW_EDGE_GRID 60

#define GOAL_LEFT_MAX 9
#define GOAL_WIN_MAX 99

enum enumGameScene
{
	kGridElenetTag = 1,
	kDialogWindowTag = 10,

	kHoleTag = 999,
	kMatchTag,
	kCreateTag = 9999,

};

#endif // __COSTANT_H__
