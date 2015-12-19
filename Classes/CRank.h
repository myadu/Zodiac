/*
************************************************************************
*
*	CRank.h
*	Author: 杜星飞
*   describe: 游戏结果排行榜 可查看在 简单、正常、困难 模式下的排名
*
************************************************************************
*/
#ifndef __CRank_H__
#define __CRank_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

class CRank : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(CRank);

	//处理触摸事件，可以计算点击的是哪一个子项
	void tableCellTouched(TableView* table, TableViewCell* cell)
	{

	};

	//生成列表每一项的内容
	TableViewCell * tableCellAtIndex(TableView *table, ssize_t idx);

	//一共多少项
	ssize_t numberOfCellsInTableView(TableView *table);

	//每一项的宽度和高度
	Size cellSizeForTable(TableView *table);

	Size tableCellSizeForIndex(TableView *table, ssize_t idx);

/*
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view)
	{

	};
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)
	{

	}; */
	virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override{}
	virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) override{}

	//返回游戏
	void backToMenu(Ref* pSender, Control::EventType controlEvent);

	//显示排行榜 简单
	void onEasyItemCallback(Ref* ref);	

	//显示排行榜 一般
	void onNormalItemCallback(Ref* ref);	

	//显示排行榜 困难
	void onHardItemCallback(Ref* ref);	

	//生命周期
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

private:
	//显示排名
	std::string m_sDegreeName;
	int m_nowDegree;

	MenuItemSprite* easyItem;
	MenuItemSprite* normalItem;
	MenuItemSprite* hardItem;
};
#endif