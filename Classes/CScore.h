/*
************************************************************************
*
*	CScore.h
*	Author: 杜星飞
*   describe: 分数管理，包括最佳成绩/本局成绩,以及层级管理
*
************************************************************************
*/
#ifndef __CScore_H__
#define __CScore_H__

#include "cocos2d.h"

USING_NS_CC;

class CScore
{
public:
	static CScore* getInstance();

	////////////////////分数操作/////////////////////

	//获取当前分数
	int getCurScore()
	{
		return m_curScore;
	}

	//增加当前分数
	void addCurScore()
	{
		if (getIsScore())
		{
			m_curScore += 16;
		}
		else
		{
			m_curScore += 8;
		}
	}

	//获取之前分数
	int getPreScore()
	{
		return m_preScore;
	}

	//设定之前分数
	void setPreScore(int preScore)
	{
		m_preScore = preScore;
	}

	//获取最高分数
	int getHighScore()
	{
		return m_highScore;
	}

	//设定最高分数
	void setHighScore(int highScore)
	{
		if (highScore < 0)
		{
			return;
		}
		m_highScore=highScore;
	}

	//储存最高分数
	void storeHighScore()
	{
		UserDefault::getInstance()->setIntegerForKey("highestScore", getHighScore());
	}

	//获取下一关卡分数
	int getNextScore()
	{
		return m_nextScore;
	}

	////////////////////关卡层级操作/////////////////////

	//获取当前关卡
	int getCurLevel()
	{
		return m_curLevel;
	}

	//获取下一关卡
	int getNextLevel()
	{
		return m_nextLevel;
	}

	//设定当前关卡
	void setCurLevel(int level);

	//通过关卡来设定分数
	int setScoreByLevel(int level);

	////////////////////金币操作/////////////////////

	//获取金币数
	int getGold()
	{
		return m_gold;
	}

	//增加金币数
	void addGold(int gold)
	{
		if (getIsGold())
		{
			m_gold += (gold<<1);
		}
		else
		{
			m_gold += gold;
		}
	}

	//使用金币购买物品
	void useGold(int gold)
	{
		if (m_gold < gold)
		{
			return;
		}
		m_gold -= gold;
	}

	//储存金币数
	void storeGold()
	{
		UserDefault::getInstance()->setIntegerForKey("Gold", getGold());
	}

	////////////////////双倍分数道具操作/////////////////////

	//获取 双倍分数 道具数量
	int getMultScore()
	{
		return m_multScore;
	}

	//增加 双倍分数 道具数量
	void addMultScore(int num)
	{
		m_multScore += num;
	}

	//减少 双倍分数 道具数量
	void useMultScore(int num)
	{
		if (m_multScore< num)
		{
			return;
		}

		m_multScore -= num;
	}

	//储存 双倍分数 道具数量
	void storeMultScore()
	{
		UserDefault::getInstance()->setIntegerForKey("MultScore", getMultScore());
	}

	//获取 是否使用双倍分数 道具
	bool getIsScore()
	{
		return m_bScore;
	}

	//设置 是否使用双倍分数 道具
	void setIsScore(bool isScore)
	{
		m_bScore = isScore;
	}

	//储存 是否使用双倍分数 道具
	void storeIsScore()
	{
		UserDefault::getInstance()->setBoolForKey("IsScore", getIsScore());
	}


	////////////////////双倍金币道具操作/////////////////////
	//获取 双倍金币 道具数量
	int getMultGold()
	{
		return m_multGold;
	}

	//增加 双倍金币 道具数量
	void addMultGold(int num)
	{
		m_multGold += num;
	}

	//减少 双倍金币 道具数量
	void useMultGold(int num)
	{
		if (m_multGold < num)
		{
			return;
		}

		m_multGold -= num;
	}

	//储存 双倍金币 道具数量
	void storeMultGold()
	{
		UserDefault::getInstance()->setIntegerForKey("MultGold", getMultGold());
	}

	//获取 是否使用双倍金币 道具
	bool getIsGold()
	{
		return m_bGold;
	}

	//设置 是否使用双倍金币 道具
	void setIsGold(bool isGold)
	{
		m_bGold = isGold;
	}

	//储存 是否使用双倍金币 道具
	void storeIsGold()
	{
		UserDefault::getInstance()->setBoolForKey("IsGold", getIsGold());
	}


	////////////////////延时道具操作/////////////////////
	//获取 延时道具 数量
	int getPropDelay()
	{
		return m_propDelay;
	}

	//增加 延时道具 数量
	void addPropDelay(int num)
	{
		m_propDelay += num;
	}

	//减少 延时道具 数量
	void usePropDelay(int num)
	{
		if (m_propDelay < num)
		{
			return;
		}

		m_propDelay -= num;
	}

	//储存 延时道具 数量
	void storePropDelay()
	{
		UserDefault::getInstance()->setIntegerForKey("PropDelay", getPropDelay());
	}


	////////////////////提示道具操作/////////////////////
	//获取 提示道具 数量
	int getPropHint()
	{
		return m_propHint;
	}

	//增加 延时道具 道具数量
	void addPropHint(int num)
	{
		m_propHint += num;
	}

	//减少 延时道具 道具数量
	void usePropHint(int num)
	{
		if (m_propHint < num)
		{
			return;
		}

		m_propHint -= num;
	}

	//储存 延时道具 道具数量
	void storePropHint()
	{
		UserDefault::getInstance()->setIntegerForKey("PropHint", getPropHint());
	}

	////////////////////音量大小操作/////////////////////

	//获取 音量大小
	float getMusicVolume()
	{
		return m_volume;
	}

	//设定 音量大小
	void setMusicVolume(float volume)
	{
		m_volume=volume;
	}

	//储存 音量大小 
	void storeMusicVolume()
	{
		UserDefault::getInstance()->setFloatForKey("MusicVolume", getMusicVolume());
	}

	////////////////////音乐开关操作/////////////////////

	//获取 音乐开关
	bool getIsMusic()
	{
		return m_bMusci;
	}

	//设置 音乐开关
	void setIsMusic(bool isMusci)
	{
		m_bMusci=isMusci;
	}

	//储存 音乐开关 
	void storeIsMusic()
	{
		UserDefault::getInstance()->setBoolForKey("IsMusic", getIsMusic());
	}

	////////////////////游戏难度操作/////////////////////

	//获取 游戏难度
	int getGameDegree()
	{
		return m_degree;
	}

	//设定 游戏难度
	void setGameDegree(int degree)
	{
		m_degree = degree;
	}

	//储存 游戏难度
	void storeGameDegree()
	{
		UserDefault::getInstance()->setIntegerForKey("GameDegree", getGameDegree());
	}


	////////////////////再来一次&&重新开始/////////////////////
	void reAgain();
	void reStart();

	////////////////////更新游戏排行/////////////////////
	void updateRank(int score, std::string degreeName);

	static bool fCmp(const int& lhs, const int& rhs)
	{
		return lhs > rhs;
	};
private:
	CScore();
	void init();

private:
	static CScore* m_score;

	int m_curScore;	//当前分数
	int m_preScore;	//之前分数

	int m_nextScore;	//下一关分数(目标分数)
	int m_highScore;	//最高分数
	
	int m_curLevel;	//当前关卡
	int m_nextLevel;	//下一关卡

	int m_gold;	//金币

	int m_multScore; //分数倍数
	bool m_bScore;	//是否使用 分数加倍 道具

	int m_multGold;	//金币倍数
	bool m_bGold;	//是否使用 金币加倍 道具

	int m_propDelay;	//延时
	int m_propHint;	//提示

	float m_volume;	//音量大小
	bool m_bMusci;	//音乐开关

	int m_degree;	//游戏难度等级

	int rk[1000];	//游戏排行榜
};

#endif