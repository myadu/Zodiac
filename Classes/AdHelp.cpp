/*
************************************************************************
*
*	AdHelp.cpp
*	Author: 杜星飞
*   describe: 插广告
*
************************************************************************
*/

#include "AdHelp.h"

AdHelp::AdHelp()
{

}

AdHelp::~AdHelp()
{

}

void AdHelp::showAd(int adTag)
{

	//判断当前是否为Android平台 JniMethodInfo showAd;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/cpp/ShowAdHelper", "showMyAd", "(I)V");
	if (!isHave)
	{
		CCLog("jni:showAdStatic is null");
	}
	else
	{
		//调用此函数
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, adTag);
	}
#endif
}
