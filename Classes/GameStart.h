/*
************************************************************************
*
*	GameStart.h
*	Author: 杜星飞
*   describe: 游戏开始界面
*
************************************************************************
*/
#ifndef __GameStart_H__
#define __GameStart_H__

#include "cocos2d.h"

USING_NS_CC;

class GameStart : public cocos2d::Layer
{
public:
	~GameStart();

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameStart);

public:
	void moveWithBezier(Node* node,Vec2 point1, Vec2 point2);

	//开始游戏
	void startGame(Ref* pSender);

	//游戏说明
	void aboutGame(Ref* pSender);

	//难度设置
	void degreeUtil(Ref* pSender);
/*

	//音乐设置
	void musicUtil(Ref* pSender);

*/
	//退出游戏
	void menuCloseCallback(cocos2d::Ref* pSender);

	//获取金币
//	void buyGold(Ref* pSender);

	//排行榜
	void highScore(Ref* pSender);

	//游戏商城
	void gameShoping(Ref* pSender);

	//响应键盘(主要针对Android)
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	virtual void onEnterTransitionDidFinish();

	virtual void onExit();
private:
	EventListenerKeyboard* m_listener;
};
#endif