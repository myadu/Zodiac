/*
************************************************************************
*
*	AudioController.h
*	Author: 杜星飞
*   describe: 控制音乐及音效
*
************************************************************************
*/
#ifndef __AudioController_H__
#define __AudioController_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class AudioController 
{
public:

	//获取实例
	static AudioController* getInstance();

	/////////预加载背景音乐及音效///////
	void preLoadingAudio();

	/////////音乐总开关///////
	bool isMusicOpen()
	{
		return m_b;
	}

	void musicOpen()
	{
		m_b = true;
	}

	void musicOff()
	{
		m_b = false;
	}

	/////////播放 & 停止 背景音乐///////

	//播放背景音乐
	void playBackgroundMusic()
	{
		
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/bg.mp3",true);
	}

	//停止播放背景音乐
	void stopBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}

	//暂停播放背景音乐
	void pauseBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	//恢复播放背景音乐
	void resumeBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}


	/////////播放音效///////

	//加载游戏
	void loadingMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/shake.wav");
	}

	//切换场景
	void changeScene()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/pageChange.wav");
	}

	//单击成功
	void clickOk()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click.wav");
	}

	//单机失败
	void clickNo()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/click_error.wav");
	}

	//游戏开始
	void gameStart()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/start.wav");
	}

	//使用道具
	void useProp()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/props.wav");
	}

	//游戏胜利
	void gameWin()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/win.wav");
	}

	//游戏失败
	void gameLose()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/lose.wav");
	}

	//交换成功
	void changeYes()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/destory.mp3");
	}

	//交换失败
	void changeNo()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/swapBack.mp3");
	}

	/////////播放特效音乐///////



private:
	static AudioController* m_audioController;
	bool m_b;
};

#endif