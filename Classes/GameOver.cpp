/*
************************************************************************
*
*	GameOver.cpp
*	Author: 杜星飞
*   describe: 游戏结束景
*
************************************************************************
*/

#include "GameOver.h"
#include "GameScene.h"
#include "GameStart.h"
#include "CScore.h"
#include "AudioController.h"
#include "AdHelp.h"

Scene* GameOver::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
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
		AudioController::getInstance()->gameLose();
	}

	auto m_textureCache = TextureCache::getInstance();

	//添加背景
	auto bg = Sprite::createWithTexture(m_textureCache->getTextureForKey("bg_main.jpg"));
	bg->setAnchorPoint(Vec2(0, 0));
	bg->setPosition(Vec2(0, 0));
	addChild(bg);

	//////////标签文本/////////////

	//文本动作
	auto m_tintBy = TintBy::create(1.0f, 0, -255, -255);
	auto m_tintByBack = m_tintBy->reverse();
	auto m_seq = Sequence::createWithTwoActions(m_tintBy, m_tintBy);


	//添加标题
	auto m_labelSprite = Sprite::create("title_gameLose.png");
	m_labelSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height-m_labelSprite->getContentSize().height));
	this->addChild(m_labelSprite);
	m_labelSprite->runAction(RepeatForever::create(m_seq));

	//显示本次分数

	auto m_labelSprite1 = Sprite::create("title_nowScore.png");
	m_labelSprite1->setPosition(Vec2(visibleSize.width *0.3, visibleSize.height - m_labelSprite->getContentSize().height*2));
	m_labelSprite1->setScale(1.5f);
	this->addChild(m_labelSprite1);

	char buf[10];
	int score;

	score= CScore::getInstance()->getCurScore();

	sprintf(buf, "%d",score);

	auto labelscore = Label::createWithCharMap(
		"digital.png",
		20,24,
		'0');
	labelscore->setString(buf);
	labelscore->setPosition(Vec2(visibleSize.width / 3 + m_labelSprite1->getContentSize().width*1.2, visibleSize.height - m_labelSprite->getContentSize().height*2));
	labelscore->setAlignment(TextHAlignment::LEFT);
//	labelscore->enableShadow(Color4B(10, 176, 211, 192));
//	labelscore->enableOutline(Color4B(17, 227, 252, 255));
	labelscore->runAction(Sequence::create(m_seq, DelayTime::create(1.0f), m_seq,nullptr));
//	labelscore->runAction(RepeatForever::create(m_seq));
	this->addChild(labelscore);

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

	CScore::getInstance()->updateRank(score, m_sDegreeName);

	//显示最佳分数
	auto m_labelSprite2 = Sprite::create("title_highScore.png");
	m_labelSprite2->setScale(1.5f);
	m_labelSprite2->setPosition(Vec2(visibleSize.width *0.3, visibleSize.height - m_labelSprite->getContentSize().height*1.5 - m_labelSprite1->getContentSize().height * 4));
	this->addChild(m_labelSprite2);


	score = CScore::getInstance()->getHighScore();

	sprintf(buf, "%d", score);

	auto labelbestscore = Label::createWithCharMap(
		"digital.png",
		20, 24,
		'0');
	labelbestscore->setString(buf);
	labelbestscore->setAlignment(TextHAlignment::LEFT);
	labelbestscore->setPosition(Vec2(visibleSize.width / 3 + m_labelSprite2->getContentSize().width*1.2, visibleSize.height - m_labelSprite->getContentSize().height*1.5 - labelbestscore->getContentSize().height * 4));
	labelbestscore->runAction(Sequence::create(m_seq, DelayTime::create(0.5f), m_seq, nullptr));
	this->addChild(labelbestscore);

	//////////按钮/////////////

	//返回游戏按钮

	auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_again.png"));
	auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_again.png"));
	spriteSel1->setScale(1.2);

	auto backItem = MenuItemSprite::create(
		spriteNor1,
		spriteSel1
		);
	backItem->setCallback(CC_CALLBACK_1(GameOver::onBackItemCallback, this));

	//重新开始按钮
	auto spriteNor = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_restart.png"));
	auto spriteSel = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_restart.png"));
	spriteSel->setScale(1.2);

	auto restartItem = MenuItemSprite::create(
		spriteNor,
		spriteSel
		);
	restartItem->setCallback(CC_CALLBACK_1(GameOver::onReStartItemCallback, this));


	//返回主界面 按钮
	auto spriteNor3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_return.png"));
	auto spriteSel3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_return.png"));
	spriteSel3->setScale(1.2);

	auto mainItem = MenuItemSprite::create(
		spriteNor3,
		spriteSel3
		);
	mainItem->setCallback(CC_CALLBACK_1(GameOver::onMainItemCallback, this));

	//退出游戏按钮
	auto spriteNor2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_quit.png"));
	auto spriteSel2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("button_quit.png"));
	spriteSel2->setScale(1.2);

	auto exitItem = MenuItemSprite::create(
		spriteNor2,
		spriteSel2
		);
	exitItem->setCallback(CC_CALLBACK_1(GameOver::onExitItemCallback, this));

	auto menu = Menu::create(backItem, restartItem, mainItem, exitItem, nullptr);
	menu->alignItemsVerticallyWithPadding(restartItem->getContentSize().height / 2);
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 4));
	addChild(menu);

	//插屏广告
	AdHelp::showAd(1);

	return true;
}

//返回游戏
void GameOver::onBackItemCallback(Ref*)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	CScore::getInstance()->reAgain();
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}


//重新开始
void GameOver::onReStartItemCallback(Ref*)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	CScore::getInstance()->reStart();
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}

//返回主界面
void GameOver::onMainItemCallback(Ref*)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	CScore::getInstance()->reStart();
	auto scene = GameStart::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, scene));
}

//退出游戏
void GameOver::onExitItemCallback(Ref*)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	Director::getInstance()->end();
}

void GameOver::onExit()
{
	this->stopAllActions();
	this->removeAllChildrenWithCleanup(true);
	Layer::onExit();
}