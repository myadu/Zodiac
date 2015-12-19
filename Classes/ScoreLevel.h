/*
************************************************************************
*
*	ScoreLevel.h
*	Author: 杜星飞
*   describe: 显示分数、关卡、金币等信息
*
************************************************************************
*/
#ifndef __ScoreLevel_H__
#define __ScoreLevel_H__

#include "cocos2d.h"

USING_NS_CC;

class ScoreLevel : public Node
{
public:
	virtual bool init();

	CREATE_FUNC(ScoreLevel);

	void RefreshData();

	void RefreshScore();

private:
	Label* m_labelTollgate;	//关卡标签

	Label* m_labelGold;	//金币标签

	Label* m_labelScore;	//分数标签
	Label* m_labelTargetScore;	//目标分数标签
	Label* m_labelHighScore;	//最高分数标签
};

#endif