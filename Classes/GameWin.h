/*
************************************************************************
*
*	GameWin.h
*	Author: 杜星飞
*   describe: 游戏通关场景
*
************************************************************************
*/
#ifndef __GameWin_H__
#define __GameWin_H__

#include "cocos2d.h"

USING_NS_CC;

class GameWin : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameWin);

	void onMainItemCallback(Ref*);	//返回主界面
	void onExitItemCallback(Ref*);	//退出游戏
	
};

#endif