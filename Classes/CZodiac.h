/*
************************************************************************
*
*	CZodiac.h
*	Author: 杜星飞
*   describe: 基础精灵类，包含精灵类型、位置、状态(是否交换、消除)
*
************************************************************************
*/
#ifndef __CZodiac_H__
#define __CZodiac_H__

#include "cocos2d.h"

USING_NS_CC;

class CZodiac : public Sprite
{
public:
	static CZodiac* create(int type, Vec2 pos);
	bool init(int type, Vec2 pos);

public:
	//获取生肖类型
	int getType()
	{
		return m_type;
	};

	//获取生肖位置
	Vec2 getPos()
	{
		return m_pos;
	};

	//设置生肖位置
	void setPos(Vec2 pos)
	{
		m_pos = pos;
	};

	//判断是否在交换状态
	bool isSwaping()
	{
		return m_bSwap;
	};

	//判断是否在消除状态
	bool isDestory()
	{
		return m_bDestory;
	};

	//设置为交换状态
	void setSwaping(bool state)
	{
		m_bSwap = state;
	};

	//设置为消除状态并消除
	void fDestory();

private:
	int m_type;
	Vec2 m_pos;
	bool m_bSwap;
	bool m_bDestory;
};
#endif