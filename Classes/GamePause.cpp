/*
************************************************************************
*
*	GamePause.cpp
*	Author: 杜星飞
*   describe: 游戏暂停界面
*
************************************************************************
*/

#include "GamePause.h"
#include "GameStart.h"
#include "AudioController.h"
#include "CScore.h"
#include "AdHelp.h"


// 设置一个静态的场景函数
Scene* GamePause::createScene()
{
	// 给scene创建内容
	Scene *myscene = Scene::create();

	// 创建一个layer
	GamePause *mylayer = GamePause::create();

	// 把这个layer加入到scene中
	myscene->addChild(mylayer);

	// 返回scene
	return myscene;
}
Scene* GamePause::createScene(RenderTexture* sqr)
{
	Scene *myscene = Scene::create();

	Sprite *_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());
	_spr->setPosition(Vec2(160, 270));
	_spr->setFlippedY(true);
	_spr->setColor(Color3B::GRAY);
	myscene->addChild(_spr);

	GamePause* mylayer = GamePause::create();

	myscene->addChild(mylayer);

	return myscene;
}

// 初始化
bool GamePause::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size mysize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->pauseBackgroundMusic();
	}

	//背景
	Sprite *bg = Sprite::create("koala.png");
	bg->setScale(2.0f);
	bg->setPosition(Vec2(mysize.width/2,mysize.height-bg->getContentSize().height));
	this->addChild(bg);

	//继续游戏 按钮
	auto spriteNor3 = Sprite::create("button_goto.png");
	auto spriteSel3 = Sprite::create("button_goto.png");
	spriteSel3->setScale(1.2);

	auto returnItem = MenuItemSprite::create(
		spriteNor3,
		spriteSel3
		);
	returnItem->setCallback(CC_CALLBACK_1(GamePause::returnGame, this));

	//返回主界面 按钮
	auto spriteNor2 = Sprite::create("button_return.png");
	auto spriteSel2 = Sprite::create("button_return.png");
	spriteSel2->setScale(1.2);

	auto mainItem = MenuItemSprite::create(
		spriteNor2,
		spriteSel2
		);
	mainItem->setCallback(CC_CALLBACK_1(GamePause::mianGame, this));

	auto menu = Menu::create(returnItem, mainItem, nullptr);
	menu->alignItemsVerticallyWithPadding(returnItem->getContentSize().height / 2);
	menu->setPosition(Vec2(mysize.width / 2, mysize.height / 4));
	addChild(menu);
	
	//插屏广告
	AdHelp::showAd(1);

	return true;
}

//继续游戏
void GamePause::returnGame(Ref * pSender)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();

		AudioController::getInstance()->resumeBackgroundMusic();
	}

	Director::getInstance()->popScene();
	Director::getInstance()->resume();
}

//返回游戏菜单
void GamePause::mianGame(Ref * pSender)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();

		AudioController::getInstance()->stopBackgroundMusic();
	}

	Director::getInstance()->popScene();
	Director::getInstance()->resume();

	CScore::getInstance()->reStart();

	Scene *gamemenu = GameStart::createScene();
	auto *tt = TransitionPageTurn::create(0.5f, gamemenu, false);
	Director::getInstance()->replaceScene(tt);
}

void GamePause::onExit()
{
	Layer::onExit();
}