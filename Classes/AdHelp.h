/*
************************************************************************
*
*	AdHelp.h
*	Author: 杜星飞
*   describe: 插广告
*
************************************************************************
*/

#ifndef __AdHelp__H__
#define __AdHelp__H__

#include <cocos2d.h>
using namespace cocos2d;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#endif

class AdHelp
{
public:
	AdHelp();
	virtual ~AdHelp();
	//通过JNI调用JAVA静态函数，实现展示AD
	static void showAd(int adTag);

};

#endif