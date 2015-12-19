/*
************************************************************************
*
*	FlowWord.h
*	Author: ¶ÅÐÇ·É
*   describe: Æ®×ÖÌØÐ§
*
************************************************************************
*/
#ifndef __FlowWord_H__
#define __FlowWord_H__

#include "cocos2d.h"

USING_NS_CC;

class FlowWord : public Node
{
public:
	static FlowWord* create(const std::string& word, const int fontSize, Point begin);
	bool init(const std::string& word, const int fontSize, Point begin);
	
	void flowIn(const float delayTime, std::function<void()> callback);
	void flowOut(const float delayTime, std::function<void()> callback);
	void flowInOut(const float speed, const float delay, std::function<void()> callback);

private:
	int m_fontSize;
	Point m_begin;
	Label* m_label;
};

#endif