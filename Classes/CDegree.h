/*
************************************************************************
*
*	CDegree.h
*	Author: 杜星飞
*   describe:选择游戏难度
*
************************************************************************
*/
#ifndef __CDegree_H__
#define __CDegree_H__

#include "cocos2d.h"

USING_NS_CC;

class CDegree : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	virtual void onExit();

	// implement the "static create()" method manually
	CREATE_FUNC(CDegree);

	void onEasyItemCallback(Ref* ref);	//选关卡 简单
	void onNormalItemCallback(Ref* ref);	//选关卡 一般
	void onHardItemCallback(Ref* ref);	//选关卡 困难

	void onScoreItemCallback(Ref* ref);	//使用道具 双倍分数
	void onGoldItemCallback(Ref* ref);	//使用道具 双倍金币
	void onMusicItemCallback(Ref* ref);	//音乐开关

	void onReturnItemCallback(Ref* ref);	//返回主界面

private:
	MenuItemSprite* easyItem;
	MenuItemSprite* normalItem;
	MenuItemSprite* hardItem;

	MenuItemSprite* scoreItem;
	MenuItemSprite* goldItem;
	MenuItemSprite* musicItem;
};

#endif