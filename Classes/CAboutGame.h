/*
************************************************************************
*
*	CAboutGame.h
*	Author: 杜星飞
*   describe: 游戏玩法、游戏介绍
*
************************************************************************
*/
#ifndef __CAboutGame_H__
#define __CAboutGame_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

class CAboutGame : public cocos2d::Layer
{
public:
	
	virtual bool init();

	static cocos2d::Scene* createScene();
	
	CREATE_FUNC(CAboutGame);

	//切换到主菜单
	void backToMenu(Ref* pSender, Control::EventType controlEvent);

	//切换页码
	void tbChange0(Node * node, void * a);

	//切换内容
	void tbChange(Node * node, void * a);
	
	//按钮无效化
	void disable();
	//恢复按钮
	void enable();

	//点击事件
	void upInsideAction(Ref * sender, Control::EventType controlEvent);

	//生命周期
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

public:
	Sprite* ptb;
	Sprite* ptc;
};

#endif