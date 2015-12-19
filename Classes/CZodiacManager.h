/*
************************************************************************
*
*	CZodiacManager.h
*	Author: 杜星飞
*   describe:管理精灵类的产生，交换，消除 以及 游戏状态
*
************************************************************************
*/
#ifndef __CZodiacManager_H__
#define __CZodiacManager_H__

#include "cocos2d.h"
#include "CZodiac.h"
#include "CScore.h"
#include "ScoreLevel.h"
#include "ui/cocosGUI.h"
#include <vector>

USING_NS_CC;
using namespace ui;
using namespace std;


const int Grid_Width = 40;
const int Move_Speed = 0.2;
const int First_Zodiac_Id = 1;
//const int Second_Zodiac_Id = 3;
//const int Third_Zodiac_Id = 5;
const int Easy_Zodiac_Id = 6;
const int Normal_Zodiac_Id = 9;
const int Hard_Zodiac_Id = 12;


class CZodiacManager : public Node
{
public:

	static CZodiacManager* create(int row, int col);
	bool init(int row, int col);

	//更新 生肖精灵组
	void updateZodiacGroup();

	//清除 生肖精灵组
	void deleteZodiacGroup();

	//判断 生肖精灵组 是否 已不能移动
	bool isDeadZodiacGroup();

	//获取行列
	int getRow()
	{
		return m_row;
	}

	int getCol()
	{
		return m_col;
	}

private:
	/////////////////////创建生肖精灵/////////////////////////

	//创建生肖精灵
	CZodiac* createZodiac(int row, int col);

	//判断所创建生肖精灵是否合法
	bool isWellZodiac(CZodiac* zodiac, int row, int col);

	//设置生肖精灵的位置
	void setZodiacPos(CZodiac* zodiac, float row, float col);


	////////////////////操作生肖精灵////////////////////////////////

	//交换生肖精灵位置
	void swapZodiacPos(CZodiac* lhs, CZodiac* rhs);

	//移动生肖精灵位置
	void moveZodiacToNewPos(CZodiac* zodiac);

	//刷新生肖精灵组
	void refreshZodiacGrid();

	//按 列 刷新生肖精灵组
	void refreshZodiacByCol(int col);

public:
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchMoved(Touch* touch, Event* unused_event);

private:

	/////////////////消除生肖精灵////////////////////////

	//判断是否能够消除
	bool isZodiacDestory();

	//消除
	void zodiacDestory();

	/////////////////游戏状态////////////////////////
	
	//正在交换生肖精灵
	void onZodiacSwaping(float dt);

	//交换生肖精灵完毕
	void onZodiacSwapingBack(float dt);

	//正在消除生肖精灵
	void onZodiacDestory(float dt);

	//消除&之后刷新
	void onZodiacRefreshing(float dt);

public:
	/////////////获取游戏进度/////////////////////

	int getReducing();


	/////////////设定游戏进度/////////////////////
	void addReducing();

	/////////////本局游戏进度条/////////////////////
	void onReducingBonus(float dt);

	////////////////游戏控制、道具使用//////////////////
	void onPauseItemCallback(Ref* ref);	//暂停游戏

	void onDelayItemCallback(Ref* ref);	//使用道具 延时

	void onHintItemCallback(Ref* ref);	//使用道具 提示

	void useDelayProp();
	void useHintProp();

private:
	int m_row;
	int m_col;

	LoadingBar* m_bar;	//进度条

	CZodiac* m_zodiacSelected;
	CZodiac* m_zodiacSwaped;

	vector<vector<CZodiac*>> m_zodiacBox;
	Vector<CZodiac*> m_destoryZodiacBox;
	Vector<CZodiac*> m_newZodiacBox;

	ScoreLevel* m_scoreLevel;	//分数和关卡

	MenuItemSprite* delayItem;	//延时
	Label* m_labelDelay;	//延时道具数量

	MenuItemSprite* hintItem;	//提示
	Label* m_labelHint;	//提示道具数量

	int m_delayNum;
	int m_hintNum;
};

#endif