/*
************************************************************************
*
*	CDegree.cpp
*	Author: 杜星飞
*   describe:选择游戏难度
*
************************************************************************
*/

#include "CDegree.h"
#include "GameStart.h"
#include "CScore.h"
#include "AudioController.h"

Scene* CDegree::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CDegree::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool CDegree::init()
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

	//添加背景

	auto m_textureCache = TextureCache::getInstance();

	auto bg = Sprite::createWithTexture(m_textureCache->getTextureForKey("bg4.jpg")); 
	bg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	this->addChild(bg);

	//添加小熊
	auto bg1 = Sprite::create("koala.png");
	bg1->setPosition(Vec2(visibleSize.width-bg1->getContentSize().width*0.8, visibleSize.height-bg1->getContentSize().height*0.8));
	this->addChild(bg1);

	//返回主界面
	auto spriteNor0 = Sprite::createWithTexture(m_textureCache->getTextureForKey("return.png"));
	auto spriteSel0 = Sprite::createWithTexture(m_textureCache->getTextureForKey("return.png"));
	spriteSel0->setScale(1.2);

	auto backItem = MenuItemSprite::create(
		spriteNor0,
		spriteSel0
		);
	backItem->setCallback(CC_CALLBACK_1(CDegree::onReturnItemCallback, this));
	backItem->setAnchorPoint(Vec2(-0.2,1.2));
	backItem->setPosition(Vec2(0,visibleSize.height));

	auto menu0 = Menu::create(backItem, nullptr);
	menu0->setPosition(Vec2::ZERO);
	this->addChild(menu0);


	//////////游戏难度设置/////////////

	auto degreeSprite = Sprite::create("title_degreeSetting.png");
	degreeSprite->setAnchorPoint(Vec2(0,1));
	degreeSprite->setPosition(0,visibleSize.height * 7 / 8 - backItem->getContentSize().height/2);
	this->addChild(degreeSprite);

	//游戏难度 简单 按钮
	Sprite* spriteNor1; 
	Sprite* spriteSel1;

	if (1==CScore::getInstance()->getGameDegree())
	{
		spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
		spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
	}
	else
	{
		spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
	}

	spriteSel1->setScale(1.2);

	easyItem = MenuItemSprite::create(
		spriteNor1,
		spriteSel1
		);
	easyItem->setCallback(CC_CALLBACK_1(CDegree::onEasyItemCallback, this));

	//游戏难度 正常 按钮
	Sprite* spriteNor2;
	Sprite* spriteSel2;

	if (2==CScore::getInstance()->getGameDegree())
	{
		spriteNor2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal1.png"));
		spriteSel2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal1.png"));
	}
	else
	{
		spriteNor2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		spriteSel2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
	}
	
	spriteSel2->setScale(1.2);

	normalItem = MenuItemSprite::create(
		spriteNor2,
		spriteSel2
		);
	normalItem->setCallback(CC_CALLBACK_1(CDegree::onNormalItemCallback, this));

	//游戏难度 困难 按钮
	Sprite* spriteNor3;
	Sprite* spriteSel3;

	if (3==CScore::getInstance()->getGameDegree())
	{
		spriteNor3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard1.png"));
		spriteSel3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard1.png"));
	}
	else
	{
		spriteNor3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		spriteSel3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
	}

	spriteSel3->setScale(1.2);

	hardItem = MenuItemSprite::create(
		spriteNor3,
		spriteSel3
		);
	hardItem->setCallback(CC_CALLBACK_1(CDegree::onHardItemCallback, this));

	auto menu = Menu::create(easyItem, normalItem, hardItem, nullptr);
	menu->alignItemsVerticallyWithPadding(hardItem->getContentSize().height / 2);
	menu->setPosition(Vec2(visibleSize.width*0.4,visibleSize.height*0.55));
	addChild(menu);

	//////////游戏道具设置/////////////

	auto propSprite = Sprite::create("title_useProp.png");
	propSprite->setAnchorPoint(Vec2(0,1));
	propSprite->setPosition(0, visibleSize.height*0.35);
	this->addChild(propSprite);

	auto m_score = CScore::getInstance();

	//双倍分数 道具
	Sprite* spriteNor4;
	Sprite* spriteSel4;

	if (m_score->getMultScore()>0 && m_score->getIsScore())
	{
		spriteNor4 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score.png"));
		spriteSel4 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score.png"));
	}
	else
	{
		m_score->setIsScore(false);
		spriteNor4 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score1.png"));
		spriteSel4 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score1.png"));
	}

	spriteSel4->setScale(1.2);

	scoreItem = MenuItemSprite::create(
		spriteNor4,
		spriteSel4
		);
	scoreItem->setCallback(CC_CALLBACK_1(CDegree::onScoreItemCallback, this));

	//双倍金币 道具
	Sprite* spriteNor5;
	Sprite* spriteSel5;

	if (m_score->getMultGold()>0 && m_score->getIsGold())
	{
		spriteNor5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold.png"));
		spriteSel5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold.png"));
	}
	else
	{
		m_score->setIsGold(false);
		spriteNor5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold1.png"));
		spriteSel5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold1.png"));
	}

	spriteSel5->setScale(1.2);

	goldItem = MenuItemSprite::create(
		spriteNor5,
		spriteSel5
		);
	goldItem->setCallback(CC_CALLBACK_1(CDegree::onGoldItemCallback, this));


	auto menu1 = Menu::create(scoreItem, goldItem,  nullptr);
	menu1->alignItemsHorizontallyWithPadding(scoreItem->getContentSize().width/2);
	menu1->setPosition(Vec2(visibleSize.width *0.7, visibleSize.height*0.3));
	addChild(menu1);

	
	//////////游戏音效设置/////////////
	auto musicSprite = Sprite::create("title_gameMusic.png");
	musicSprite->setAnchorPoint(Vec2(0, 1));
	musicSprite->setPosition(0, visibleSize.height*0.2);
	this->addChild(musicSprite);

	Sprite* spriteNor6;
	Sprite* spriteSel6;

	if (m_score->getIsMusic())
	{
		spriteNor6 = Sprite::create("menu_sound_on.png");
		spriteSel6 = Sprite::create("menu_sound_on.png");
	}
	else
	{
		spriteNor6 = Sprite::create("menu_sound_off.png");
		spriteSel6 = Sprite::create("menu_sound_off.png");
	}

	spriteSel6->setScale(1.2);

	musicItem = MenuItemSprite::create(
		spriteNor6,
		spriteSel6
		);
	musicItem->setCallback(CC_CALLBACK_1(CDegree::onMusicItemCallback, this));

	auto menu2 = Menu::create(musicItem, nullptr);
	menu2->setPosition(Vec2(visibleSize.width *0.7, visibleSize.height*0.15));
	addChild(menu2);

	return true;
}

void CDegree::onEasyItemCallback(Ref* ref)
{
	if (1 == CScore::getInstance()->getGameDegree())
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	auto m_textureCache = TextureCache::getInstance();

	auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
	auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
	spriteSel1->setScale(1.2);

	easyItem->setNormalImage(spriteNor1);
	easyItem->setSelectedImage(spriteSel1);
/*

	normalItem->setEnabled(false);
	hardItem->setEnabled(false);

*/
	if (2==CScore::getInstance()->getGameDegree())
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		spriteSel1->setScale(1.2);

		normalItem->setNormalImage(spriteNor1);
		normalItem->setSelectedImage(spriteSel1);
	}
	else if (3 == CScore::getInstance()->getGameDegree())
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		spriteSel1->setScale(1.2);

		hardItem->setNormalImage(spriteNor1);
		hardItem->setSelectedImage(spriteSel1);
	}
	CScore::getInstance()->setGameDegree(1);
	CScore::getInstance()->storeGameDegree();
}

void CDegree::onNormalItemCallback(Ref* ref)
{
	if (2 == CScore::getInstance()->getGameDegree())
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	auto m_textureCache = TextureCache::getInstance();

	auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal1.png"));
	auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal1.png"));
	spriteSel1->setScale(1.2);

	normalItem->setNormalImage(spriteNor1);
	normalItem->setSelectedImage(spriteSel1);
/*

	easyItem->setEnabled(false);
	hardItem->setEnabled(false);

*/
	if (1 == CScore::getInstance()->getGameDegree())
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		spriteSel1->setScale(1.2);

		easyItem->setNormalImage(spriteNor1);
		easyItem->setSelectedImage(spriteSel1);
	}
	else if (3 == CScore::getInstance()->getGameDegree())
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		spriteSel1->setScale(1.2);

		hardItem->setNormalImage(spriteNor1);
		hardItem->setSelectedImage(spriteSel1);
	}

	CScore::getInstance()->setGameDegree(2);
	CScore::getInstance()->storeGameDegree();
}

void CDegree::onHardItemCallback(Ref* ref)
{
	if (3 == CScore::getInstance()->getGameDegree())
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	auto m_textureCache = TextureCache::getInstance();

	auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard1.png"));
	auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard1.png"));
	spriteSel1->setScale(1.2);

	hardItem->setNormalImage(spriteNor1);
	hardItem->setSelectedImage(spriteSel1);
/*

	easyItem->setEnabled(false);
	normalItem->setEnabled(false);
*/

	if (1 == CScore::getInstance()->getGameDegree())
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		spriteSel1->setScale(1.2);

		easyItem->setNormalImage(spriteNor1);
		easyItem->setSelectedImage(spriteSel1);
	}
	else if (2 == CScore::getInstance()->getGameDegree())
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		spriteSel1->setScale(1.2);

		normalItem->setNormalImage(spriteNor1);
		normalItem->setSelectedImage(spriteSel1);
	}

	CScore::getInstance()->setGameDegree(3);
	CScore::getInstance()->storeGameDegree();
}

//是否使用道具 双倍分数
void CDegree::onScoreItemCallback(Ref* ref)
{
	auto m_score = CScore::getInstance();

	if (m_score->getMultScore()<=0)
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	auto m_textureCache = TextureCache::getInstance();

	if (m_score->getIsScore())
	{
		m_score->setIsScore(false);
		m_score->storeIsScore();

		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score1.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score1.png"));
		spriteSel1->setScale(1.2);

		scoreItem->setNormalImage(spriteNor1);
		scoreItem->setSelectedImage(spriteSel1);
	}
	else
	{
		m_score->setIsScore(true);
		m_score->storeIsScore();

		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_score.png"));
		spriteSel1->setScale(1.2);

		scoreItem->setNormalImage(spriteNor1);
		scoreItem->setSelectedImage(spriteSel1);
	}
}

//是否使用道具 双倍金币
void CDegree::onGoldItemCallback(Ref* ref)
{
	auto m_score = CScore::getInstance();

	if (m_score->getMultGold() <= 0)
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	auto m_textureCache = TextureCache::getInstance();

	if (m_score->getIsGold())
	{
		m_score->setIsGold(false);
		m_score->storeIsGold();

		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold1.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold1.png"));
		spriteSel1->setScale(1.2);

		goldItem->setNormalImage(spriteNor1);
		goldItem->setSelectedImage(spriteSel1);
	}
	else
	{
		m_score->setIsGold(true);
		m_score->storeIsGold();

		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_gold.png"));
		spriteSel1->setScale(1.2);

		goldItem->setNormalImage(spriteNor1);
		goldItem->setSelectedImage(spriteSel1);
	}
}

//音乐开关
void CDegree::onMusicItemCallback(Ref* ref)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->clickOk();
	}

	auto m_score = CScore::getInstance();

	if (m_score->getIsMusic())
	{
		m_score->setIsMusic(false);
		m_score->storeIsMusic();

		AudioController::getInstance()->musicOff();

		auto spriteNor1 = Sprite::create("menu_sound_off.png");
		auto spriteSel1 = Sprite::create("menu_sound_off.png");
		spriteSel1->setScale(1.2);

		musicItem->setNormalImage(spriteNor1);
		musicItem->setSelectedImage(spriteSel1);
	}
	else
	{
		m_score->setIsMusic(true);
		m_score->storeIsMusic();

		AudioController::getInstance()->musicOpen();

		auto spriteNor1 = Sprite::create("menu_sound_on.png");
		auto spriteSel1 = Sprite::create("menu_sound_on.png");
		spriteSel1->setScale(1.2);

		musicItem->setNormalImage(spriteNor1);
		musicItem->setSelectedImage(spriteSel1);
	}
}

//返回主界面
void CDegree::onReturnItemCallback(Ref* ref)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->changeScene();
	}

	//返回主界面
	auto scene = GameStart::createScene();
	Director::getInstance()->replaceScene(TransitionPageTurn::create(0.5f, scene, true));
}

void CDegree::onExit()
{
	Layer::onExit();
	this->removeAllChildrenWithCleanup(true);
}
