/*
************************************************************************
*
*	GameScene.cpp
*	Author: 杜星飞
*   describe: 游戏主场景
*
************************************************************************
*/

#include "GameScene.h"
#include "GameOver.h"
#include "GameWin.h"
#include "GameStart.h"
#include "AudioController.h"
#include "AdHelp.h"

GameScene::~GameScene()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
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

	auto m_textureCache=TextureCache::getInstance();

	//加载背景
	m_bgSprite = Sprite::createWithTexture(m_textureCache->getTextureForKey("bg1.jpg"));
	m_bgSprite->setTag(100);
	m_bgSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(m_bgSprite);
	
	//生肖精灵组
	m_zodiacManager = nullptr;

	//监听手机键盘
	auto m_listener = EventListenerKeyboard::create();
	m_listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_listener, this);

	//关闭Banner广告
	AdHelp::showAd(3);

	this->showLevel();

	return true;
}

//显示关卡飘字
void GameScene::showLevel()
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->gameStart();
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto m_dictionary = Dictionary::createWithContentsOfFile("fonts/data.xml");

	std::string s;

	s = ((__String*)(m_dictionary->objectForKey("tollgate")))->_string;

	m_flowLevel = FlowWord::create(
		s+String::createWithFormat("%d", CScore::getInstance()->getNextLevel())->_string,
		40, 
		Point(visibleSize.width, visibleSize.height / 3 * 2)
		);
	this->addChild(m_flowLevel, 1);
	m_flowLevel->flowIn(0.5f, CC_CALLBACK_0(GameScene::showTargetScore, this));

	log("Show Level!");
}

//显示目标分数飘字
void GameScene::showTargetScore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto m_dictionary = Dictionary::createWithContentsOfFile("fonts/data.xml");

	std::string s;

	s = ((__String*)(m_dictionary->objectForKey("targetScore")))->_string;

	m_flowTargetScore = FlowWord::create(
		s+String::createWithFormat("%d", CScore::getInstance()->getNextScore())->_string,
		40, 
		Point(visibleSize.width, visibleSize.height / 3)
		);
	this->addChild(m_flowTargetScore, 1);
	m_flowTargetScore->flowIn(0.5f, CC_CALLBACK_0(GameScene::removeFlowWord, this));
}

void GameScene::removeFlowWord()
{
	m_flowLevel->flowOut(0.5f, nullptr);
	m_flowTargetScore->flowOut(0.5f, CC_CALLBACK_0(GameScene::newZodiacManager, this));

	log("Remove Level!");
}

//创建生肖精灵组
void GameScene::newZodiacManager()
{


	if (m_zodiacManager!=nullptr)
	{
		m_zodiacManager->removeAllChildrenWithCleanup(true);
	}


	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_zodiacManager = CZodiacManager::create(8, 8);
	this->addChild(m_zodiacManager);
//	m_zodiacManager->setVisible(false);
//	m_zodiacManager->ignoreAnchorPointForPosition(false);
//	m_zodiacManager->setAnchorPoint(Vec2(1,1));
//	m_zodiacManager->setAnchorPoint(Vec2(0, 0));
//	m_zodiacManager->setPosition(Vec2(0,0));

	log("m_zodiacManager setAnchorPoint %f  %f", m_zodiacManager->getAnchorPoint().x, m_zodiacManager->getAnchorPoint().y);
	log("m_zodiacManager setPosition %f  %f", m_zodiacManager->getPositionX(), m_zodiacManager->getPositionY());

	auto m_score=CScore::getInstance();
	m_score->setPreScore(CScore::getInstance()->getCurScore());

	//更新道具数量(双倍分数、双倍金币)
	if (m_score->getIsScore() && m_score->getMultScore()>0)
	{
		log("MultScore number1 %d",m_score->getMultScore());

		m_score->useMultScore(1);		
		m_score->storeMultScore();

		log("MultScore number2 %d", m_score->getMultScore());
	}

	if (m_score->getIsGold() && m_score->getMultGold() > 0)
	{
		log("MultGold number1 %d", m_score->getMultGold());

		m_score->useMultGold(1);
		m_score->storeMultGold();

		log("MultGold number2 %d", m_score->getMultGold());
	}


	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->playBackgroundMusic();
	}

	this->schedule(schedule_selector(GameScene::updateBG), 1.0);
}

void GameScene::updateBG(float dt)
{
	if (m_zodiacManager==nullptr)
	{
		return;
	}	


	auto m_upScore = CScore::getInstance();

	if (m_zodiacManager->getReducing() <= 0 || m_upScore->getCurScore() >= m_upScore->getNextScore())
	{
		this->stopAllActions();
		this->unschedule(schedule_selector(GameScene::updateBG));


		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->stopBackgroundMusic();
		}

		//储存分数

		//储存当前分数
		

		//储存最高分数
		if (m_upScore->getCurScore()>m_upScore->getHighScore())
		{
			m_upScore->setHighScore(m_upScore->getCurScore());
			m_upScore->storeHighScore();
		}

		//闯关失败
		if (m_upScore->getCurScore() < m_upScore->getNextScore())
		{		
			auto m_scene = GameOver::createScene();
			Director::getInstance()->replaceScene(m_scene);
			log("GameOver");
		}
		else
		{
			//更新、储存金币值
			if (m_zodiacManager->getReducing() > 0)
			{
				m_upScore->addGold(m_zodiacManager->getReducing());
				m_upScore->storeGold();
			}

			//是否通关
			if (m_upScore->getCurLevel()==11)	
			{
				auto m_scene = GameWin::createScene();
				Director::getInstance()->replaceScene(m_scene);

				return;
			}

			//进入下一关卡
			m_upScore->setCurLevel(m_upScore->getCurLevel() + 1);
			this->showLevel();
		}
	}

	auto fadein = FadeIn::create(0.1);
	auto fadeout = FadeOut::create(0.1);
	CallFunc* call = nullptr;
	//根据倒计时条进度刷新背景
	if (m_zodiacManager->getReducing() < 100 && m_zodiacManager->getReducing() > 75)
	{
		if (m_bgSprite->getTag() == 100)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bgSprite->setTexture(TextureCache::getInstance()->getTextureForKey("bg1.jpg"));
			});
			m_bgSprite->setTag(100);
		}
	}

	if (m_zodiacManager->getReducing() < 75 && m_zodiacManager->getReducing() > 50)
	{
		if (m_bgSprite->getTag() == 101)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bgSprite->setTexture(TextureCache::getInstance()->getTextureForKey("bg2.jpg"));
				m_bgSprite->setTag(101);
			});
		}
	}

	if (m_zodiacManager->getReducing() < 50 && m_zodiacManager->getReducing() > 25)
	{
		if (m_bgSprite->getTag() == 102)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bgSprite->setTexture(TextureCache::getInstance()->getTextureForKey("bg3.jpg"));
				m_bgSprite->setTag(102);
			});
		}
	}

	if (m_zodiacManager->getReducing() < 25)
	{
		if (m_bgSprite->getTag() == 103)
			call = nullptr;
		else
		{
			call = CallFunc::create([this](){
				m_bgSprite->setTexture(TextureCache::getInstance()->getTextureForKey("bg4.jpg"));
				m_bgSprite->setTag(103);
			});
		}
	}

	if (call)
	{
		m_bgSprite->runAction(Sequence::create(fadeout, call, fadein, nullptr));
	}
}

//响应键盘(主要针对Android)
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->unschedule(schedule_selector(GameScene::updateBG));

		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->stopBackgroundMusic();

			AudioController::getInstance()->changeScene();
		}
		this->stopAllActions();
		this->removeAllChildrenWithCleanup(true);
		auto menuscene = GameStart::createScene();
//		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuscene));
		Director::getInstance()->replaceScene(TransitionPageTurn::create(0.5f, menuscene, false));
	}
		
}

//生命周期
void GameScene::onEnter()
{
	Layer::onEnter();
}

void GameScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void GameScene::onExit()
{
	Layer::onExit();
}