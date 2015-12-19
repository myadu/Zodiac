/*
************************************************************************
*
*	CShop.h
*	Author: 杜星飞
*   describe: 游戏商城，包含 分数加倍、金币加倍、延时、提示
*
************************************************************************
*/
#ifndef __CShop_H__
#define __CShop_H__

#include "cocos2d.h"

USING_NS_CC;

class CShop : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	virtual void onExit();

	// implement the "static create()" method manually
	CREATE_FUNC(CShop);

	void onScoreItemCallback(Ref* pSender);	//选道具 分数加倍
	void onGoldItemCallback(Ref* pSender);	//选道具 金币加倍
	void onDelayItemCallback(Ref* pSender);	//选道具 延时
	void onHintItemCallback(Ref* pSender);	//选道具 提示

//	void onChangeItemCallback(Ref* pSender);	//金币积分兑换

	void onReturnItemCallback(Ref* ref);	//返回主界面

private:
//	Label* totalLabel;
};


#endif