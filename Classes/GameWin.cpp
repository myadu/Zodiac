/*
************************************************************************
*
*	GameWin.cpp
*	Author: 杜星飞
*   describe: 游戏通关场景
*
************************************************************************
*/

#include "GameWin.h"
#include "GameStart.h"
#include "CScore.h"
#include "AudioController.h"
#include "AdHelp.h"

Scene* GameWin::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameWin::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameWin::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 3. add your codes below...

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->gameWin();
	}

	auto m_textureCache = TextureCache::getInstance();

	//添加背景
	auto bg = Sprite::createWithTexture(m_textureCache->getTextureForKey("bg_main.jpg"));
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	this->addChild(bg);

	//添加标题
	auto m_labelSprite = Sprite::create("title_gameOK.png");
	m_labelSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 4 / 5));
	this->addChild(m_labelSprite);

	//储存本次分数
	std::string m_sDegreeName;

	if (1 == CScore::getInstance()->getGameDegree())
	{
		m_sDegreeName = "EasyNum";
	}
	else if (2 == CScore::getInstance()->getGameDegree())
	{
		m_sDegreeName = "NormalNum";
	}
	else if (3 == CScore::getInstance()->getGameDegree())
	{
		m_sDegreeName = "HardNum";
	}

	CScore::getInstance()->updateRank(CScore::getInstance()->getCurScore(), m_sDegreeName);


	//返回主界面按钮

	auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_return.png"));
	auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_return.png"));
	
	spriteSel1->setScale(1.2);

	auto returnItem = MenuItemSprite::create(
		spriteNor1,
		spriteSel1
		);
	returnItem->setCallback(CC_CALLBACK_1(GameWin::onMainItemCallback, this));

	//退出游戏按钮
	auto spriteNor2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_quit.png"));
	auto spriteSel2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_quit.png"));

	spriteSel2->setScale(1.2);

	auto exitItem = MenuItemSprite::create(
		spriteNor2,
		spriteSel2
		);
	exitItem->setCallback(CC_CALLBACK_1(GameWin::onExitItemCallback, this));


	auto menu = Menu::create(returnItem, exitItem, nullptr);
	menu->alignItemsVerticallyWithPadding(returnItem->getContentSize().height);
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 3));
	addChild(menu);

	//插屏广告
	AdHelp::showAd(1);

	return true;
}

void GameWin::onMainItemCallback(Ref*)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	//返回主界面
	CScore::getInstance()->reStart();
	auto scene = GameStart::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void GameWin::onExitItemCallback(Ref*)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	//退出游戏
	Director::getInstance()->end();
}