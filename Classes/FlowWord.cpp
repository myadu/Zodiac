/*
************************************************************************
*
*	FlowWord.h
*	Author: ¶ÅÐÇ·É
*   describe: Æ®×ÖÌØÐ§
*
************************************************************************
*/

#include "FlowWord.h"

FlowWord* FlowWord::create(const std::string& word, const int fontSize, Point begin){
	FlowWord* ret = new FlowWord();
	if (ret && ret->init(word, fontSize, begin)){
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool FlowWord::init(const std::string& word, const int fontSize, Point begin){
	if (!Node::init()){
		return false;
	}
	m_begin = begin;
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_label = Label::create(word, "fonts/DFPShaoNvW5-GB.ttf", fontSize);
	m_label->setPosition(begin);
	m_label->setTextColor(Color4B(random(200, 255), random(1, 100), random(1, 100), 255));
	this->addChild(m_label);

	return true;
}

void FlowWord::flowIn(const float delay, std::function<void()> callback){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveTo* move = MoveTo::create(delay, Point(visibleSize.width / 2, m_begin.y));
	CallFunc* call = CallFunc::create(callback);
	Sequence* action = Sequence::create(move, call, NULL);
	m_label->runAction(action);
}

void FlowWord::flowOut(const float delay, std::function<void()> callback){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	ScaleTo* move1 = ScaleTo::create(delay, 0.5);
	SkewTo* move2 = SkewTo::create(delay, 67.2f,-67.2f);
	auto move = Spawn::createWithTwoActions(move1, move2);

	CallFunc* call = CallFunc::create(callback);
	CallFunc* removeC = CallFunc::create([this](){
		this->removeFromParentAndCleanup(true);
	});
	Sequence* action = Sequence::create(move, call, removeC, NULL);
	m_label->runAction(action);
}

void FlowWord::flowInOut(const float speed, const float delayTime, std::function<void()> callback){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	MoveTo* moveIn = MoveTo::create(speed, Point(visibleSize.width / 2, m_begin.y));
	MoveTo* moveOut = MoveTo::create(speed, Point(0, m_begin.y));
	CallFunc* removeC = CallFunc::create([this](){
		this->removeFromParentAndCleanup(true);
	});
	Sequence* action = Sequence::create(moveIn, DelayTime::create(delayTime), moveOut, removeC, CallFunc::create(callback), NULL);
	m_label->runAction(action);
}