/*
************************************************************************
*
*	GameOver.h
*	Author: 杜星飞
*   describe: 游戏结束场景
*
************************************************************************
*/
#ifndef __GameOver_H__
#define __GameOver_H__

#include "cocos2d.h"

USING_NS_CC;

class GameOver : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOver);

	virtual void onExit();

	void onBackItemCallback(Ref*);	//再来一次
	void onReStartItemCallback(Ref*);	//重新开始
	void onMainItemCallback(Ref*);	//返回主界面
	void onExitItemCallback(Ref*);	//退出游戏
};

#endif