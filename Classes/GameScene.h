/*
************************************************************************
*
*	GameScene.h
*	Author: 杜星飞
*   describe: 游戏主场景
*
************************************************************************
*/
#ifndef __GameScene_H__
#define __GameScene_H__

#include "cocos2d.h"
#include "CZodiacManager.h"
#include "FlowWord.h"
#include "CScore.h"

USING_NS_CC;

class GameScene : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	~GameScene();

private:
	//更新背景
	void updateBG(float dt);

	//显示关卡飘字
	void showLevel();

	//显示目标分数飘字
	void showTargetScore();

	//移除飘字
	void removeFlowWord();

	//创建生肖精灵组
	void newZodiacManager();

	////响应键盘(主要针对Android)
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	//生命周期
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

private:
	FlowWord* m_flowLevel;	//关卡飘字
	FlowWord* m_flowTargetScore;	//目标分数飘字

	Sprite* m_bgSprite;	//背景

	CZodiacManager* m_zodiacManager;	//生肖精灵组
};

#endif