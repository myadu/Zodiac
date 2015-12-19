/*
************************************************************************
*
*	CZodiac.cpp
*	Author: 杜星飞
*   describe: 基础精灵类，包含精灵类型、位置、状态(是否交换、消除)
*
************************************************************************
*/

#include "CZodiac.h"

CZodiac* CZodiac::create(int type, Vec2 pos)
{
	CZodiac* m_zodiac = new CZodiac();

	if (m_zodiac && m_zodiac->init(type, pos))
	{
		m_zodiac->autorelease();
		return m_zodiac;
	}
	else
	{
		CC_SAFE_DELETE(m_zodiac);
		return nullptr;
	}
}

bool CZodiac::init(int type, Vec2 pos)
{
	Sprite::init();

	m_type = type;
	m_pos = pos;
	m_bSwap = false;
	m_bDestory = false;

	//////////////////////////////////加载精灵//////////////////////////////////////////
	auto str = __String::createWithFormat("%d.png", m_type);
	this->initWithTexture(TextureCache::getInstance()->getTextureForKey(str->getCString()));

	this->setAnchorPoint(Vec2(0, 0));

	return true;
}

//设置为消除状态并消除
void CZodiac::fDestory()
{
	m_bDestory = true;

	auto action = FadeOut::create(0.2f);
	auto m_call = CallFunc::create([this]()
	{
		this->removeFromParentAndCleanup(true);
		m_bDestory = false;
	}
	);
	this->runAction(Sequence::create(action, m_call, nullptr));
}