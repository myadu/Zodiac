/*
************************************************************************
*
*	CShop.cpp
*	Author: 杜星飞
*   describe:
*
************************************************************************
*/

#include "CShop.h"
#include "GameStart.h"
#include "CScore.h"
#include "AudioController.h"
//#include "AdHelp.h"

Scene* CShop::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = CShop::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool CShop::init()
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

	auto m_textureCache = TextureCache::getInstance();

	//添加背景
	auto bg = Sprite::createWithTexture(m_textureCache->getTextureForKey("select_degree.jpg")); 
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);


	//////////道具种类/////////////

	//双倍分数 图标
	auto scoreSprite = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("prop_score.png"));
	scoreSprite->setPosition(Vec2(visibleSize.width/4,visibleSize.height*0.6));
	this->addChild(scoreSprite);

	//双倍金币 图标
	auto goldSprite = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("prop_gold.png"));
	goldSprite->setPosition(Vec2(visibleSize.width / 4, visibleSize.height*0.6 - scoreSprite->getContentSize().height));
	this->addChild(goldSprite);

	//延时 图标
	auto delaySprite = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("prop_delay.png"));
	delaySprite->setPosition(Vec2(visibleSize.width / 4, visibleSize.height*0.6 - scoreSprite->getContentSize().height * 2));
	this->addChild(delaySprite);

	//提示 图标
	auto hintSprite = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("prop_hint.png"));
	hintSprite->setPosition(Vec2(visibleSize.width / 4, visibleSize.height*0.6 - scoreSprite->getContentSize().height * 3));
	this->addChild(hintSprite);


	//////////玩家当前道具数量/////////////
	std::string num;

	//玩家金币数量

	auto m_goldSprite = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("rest_gold.png"));
	m_goldSprite->setPosition(Vec2(visibleSize.width - m_goldSprite->getContentSize().width*0.6, visibleSize.height - m_goldSprite->getContentSize().height*0.6));
	m_goldSprite->runAction(Sequence::create(JumpTo::create(1.0,m_goldSprite->getPosition(),m_goldSprite->getContentSize().height,2),nullptr));
	this->addChild(m_goldSprite);

	num = String::createWithFormat("%d", CScore::getInstance()->getGold())->_string;
	auto remainLabel = Label::createWithCharMap(
		"digital.png",
		20, 20,
		'0');
	remainLabel->setString(num);
	remainLabel->setScale(1.5);
	remainLabel->setAlignment(TextHAlignment::RIGHT);
	remainLabel->setAnchorPoint(Vec2(1,1));
	remainLabel->setPosition(Vec2(visibleSize.width, visibleSize.height-m_goldSprite->getContentSize().height));
	this->addChild(remainLabel,1,101);
/*

	//金币积分兑换

	auto totalSprite1 = Sprite::create("total_change.png");
	auto totalSprite2 = Sprite::create("total_change.png");
	totalSprite2->setScale(1.2);

	auto totalGoldItem = MenuItemSprite::create(
		totalSprite1,
		totalSprite2
		);
	totalGoldItem->setCallback(CC_CALLBACK_1(CShop::onChangeItemCallback, this));
	totalGoldItem->setPosition(Vec2(
		visibleSize.width - totalGoldItem->getContentSize().width / 2, visibleSize.height - m_goldSprite->getContentSize().height-remainLabel->getContentSize().height*2));
	

	num = String::createWithFormat("%d", AdHelp::queryPoints())->_string;
	totalLabel = Label::createWithCharMap(
		"digital.png",
		20, 20,
		'0');
	totalLabel->setString(num);
	totalLabel->setScale(1.5);
	totalLabel->setAlignment(TextHAlignment::RIGHT);
	totalLabel->setAnchorPoint(Vec2(1, 1));
	totalLabel->setPosition(Vec2(visibleSize.width, visibleSize.height - m_goldSprite->getContentSize().height - remainLabel->getContentSize().height-totalSprite1->getContentSize().height));
	this->addChild(totalLabel, 1,100);
*/

	//返回主界面 按钮
	auto spriteNor0 = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("return.png"));
	auto spriteSel0 = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("return.png"));
	spriteSel0->setScale(1.2);

	auto backItem = MenuItemSprite::create(
		spriteNor0,
		spriteSel0
		);
	backItem->setCallback(CC_CALLBACK_1(CShop::onReturnItemCallback, this));
	backItem->setPosition(Vec2(visibleSize.width / 8, visibleSize.height * 0.8));

//	auto menu0 = Menu::create(totalGoldItem, backItem, nullptr);
	auto menu0 = Menu::create(backItem, nullptr);
	menu0->setPosition(Vec2::ZERO);
	addChild(menu0);

	//双倍分数 道具数量
	num = String::createWithFormat("%d", CScore::getInstance()->getMultScore())->_string;
	auto scoreLabel = Label::createWithCharMap(
		"digital.png",
		20, 20,
		'0');
	scoreLabel->setString(num);
	scoreLabel->setScale(1.5);
	scoreLabel->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height*0.6));
	this->addChild(scoreLabel,1,102);

	//双倍金币
	num = String::createWithFormat("%d", CScore::getInstance()->getMultGold())->_string;
	auto goldLabel = Label::createWithCharMap(
		"digital.png",
		20, 20,
		'0');
	goldLabel->setString(num);
	goldLabel->setScale(1.5);
	goldLabel->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height*0.6 - scoreSprite->getContentSize().height));
	this->addChild(goldLabel,1, 103);

	//延时
	num = String::createWithFormat("%d", CScore::getInstance()->getPropDelay())->_string;
	auto delayLabel = Label::createWithCharMap(
		"digital.png",
		20, 20,
		'0');
	delayLabel->setString(num);
	delayLabel->setScale(1.5);
	delayLabel->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height*0.6 - scoreSprite->getContentSize().height * 2));
	this->addChild(delayLabel, 1, 104);

	//提示
	num = String::createWithFormat("%d", CScore::getInstance()->getPropHint())->_string;
	auto hintLabel = Label::createWithCharMap(
		"digital.png",
		20, 20,
		'0');
	hintLabel->setString(num);
	hintLabel->setScale(1.5);
	hintLabel->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height*0.6 - scoreSprite->getContentSize().height * 3));
	this->addChild(hintLabel, 1, 105);

/*
	auto t = Label::createWithTTF(
		"test",
		"fonts/DFPShaoNvW5-GB.ttf",
		40);
	t->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(t, 1);*/


	//////////购买道具按钮/////////////

	SpriteBatchNode* spriteBatchNode = SpriteBatchNode::create("100.png");
	this->addChild(spriteBatchNode);

	auto spriteNor1 = Sprite::createWithTexture(spriteBatchNode->getTexture());
	auto spriteSel1 = Sprite::createWithTexture(spriteBatchNode->getTexture());
	spriteSel1->setScale(1.2);

//	spriteBatchNode->addChild(spriteNor1);
//	spriteBatchNode->addChild(spriteSel1);

	auto spriteNor2 = Sprite::createWithTexture(spriteBatchNode->getTexture());
	auto spriteSel2 = Sprite::createWithTexture(spriteBatchNode->getTexture());
	spriteSel2->setScale(1.2);
/*

	spriteBatchNode->addChild(spriteNor2);
	spriteBatchNode->addChild(spriteSel2);

*/
	SpriteBatchNode* spriteBatchNode1 = SpriteBatchNode::create("300.png");
	this->addChild(spriteBatchNode1);

	auto spriteNor3 = Sprite::createWithTexture(spriteBatchNode1->getTexture());
	auto spriteSel3 = Sprite::createWithTexture(spriteBatchNode1->getTexture());
	spriteSel3->setScale(1.2);
/*

	spriteBatchNode->addChild(spriteNor3);
	spriteBatchNode->addChild(spriteSel3);
*/

	auto spriteNor4 = Sprite::createWithTexture(spriteBatchNode1->getTexture());
	auto spriteSel4 = Sprite::createWithTexture(spriteBatchNode1->getTexture());
	spriteSel4->setScale(1.2);
/*

	spriteBatchNode->addChild(spriteNor4);
	spriteBatchNode->addChild(spriteSel4);

*/

	//购买道具 分数加倍 按钮
	auto scoreItem = MenuItemSprite::create(
		spriteNor1,
		spriteSel1
		);
	scoreItem->setCallback(CC_CALLBACK_1(CShop::onScoreItemCallback, this));


	//购买道具 金币加倍 按钮
	auto goldItem = MenuItemSprite::create(
		spriteNor2,
		spriteSel2
		);
	goldItem->setCallback(CC_CALLBACK_1(CShop::onGoldItemCallback, this));

	//购买道具 延时 按钮
	auto delayItem = MenuItemSprite::create(
		spriteNor3,
		spriteSel3
		);
	delayItem->setCallback(CC_CALLBACK_1(CShop::onDelayItemCallback, this));

	//购买道具 提示 按钮
	auto hintItem = MenuItemSprite::create(
		spriteNor4,
		spriteSel4
		);
	hintItem->setCallback(CC_CALLBACK_1(CShop::onHintItemCallback, this));

	auto menu = Menu::create(scoreItem, goldItem, delayItem, hintItem, nullptr);
	menu->alignItemsVerticallyWithPadding(scoreItem->getContentSize().height);
	menu->setPosition(Vec2(visibleSize.width *3/ 4, visibleSize.height*0.43));
	this->addChild(menu);

	return true;
}

void CShop::onScoreItemCallback(Ref* pSender)
{
	std::string num;
	Label* tRemain=(Label*)this->getChildByTag(101);
	Label* tScore = (Label*)this->getChildByTag(102);

	auto m_score = CScore::getInstance();

	log("Gold %d", m_score->getGold());

	if (m_score->getGold()>=100 && m_score->getMultScore()<100)
	{
		//点击

		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickOk();
		}

		//金币数量减少
		m_score->useGold(100);
		m_score->storeGold();

		log("Gold use %d", m_score->getGold());

		num = String::createWithFormat("%d", CScore::getInstance()->getGold())->_string;
		tRemain->setString(num);

		//道具数量增加 
		m_score->addMultScore(1);
		m_score->storeMultScore();

		num = String::createWithFormat("%d", CScore::getInstance()->getMultScore())->_string;
		tScore->setString(num);
	}
	else
	{
		//非法点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}
	}
}

void CShop::onGoldItemCallback(Ref* pSender)
{
	std::string num;
	Label* tRemain = (Label*)this->getChildByTag(101);
	Label* tGold = (Label*)this->getChildByTag(103);

	auto m_score = CScore::getInstance();

	log("Gold %d", m_score->getGold());

	if (m_score->getGold() >= 100 && m_score->getMultGold()<100)
	{
		//点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickOk();
		}

		//金币数量减少
		m_score->useGold(100);
		m_score->storeGold();

		log("Gold use %d", m_score->getGold());

		num = String::createWithFormat("%d", CScore::getInstance()->getGold())->_string;
		tRemain->setString(num);

		//道具数量增加 
		m_score->addMultGold(1);
		m_score->storeMultGold();

		num = String::createWithFormat("%d", CScore::getInstance()->getMultGold())->_string;
		tGold->setString(num);
	}
	else
	{
		//非法点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}
	}
}

void CShop::onDelayItemCallback(Ref* pSender)
{
	std::string num;
	Label* tRemain = (Label*)this->getChildByTag(101);
	Label* tDelay = (Label*)this->getChildByTag(104);

	auto m_score = CScore::getInstance();

	log("Gold %d", m_score->getGold());

	if (m_score->getGold() >= 300 && m_score->getPropDelay()<100)
	{
		//点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickOk();
		}

		//金币数量减少
		m_score->useGold(300);
		m_score->storeGold();

		log("Gold use %d", m_score->getGold());

		num = String::createWithFormat("%d", CScore::getInstance()->getGold())->_string;
		tRemain->setString(num);

		//道具数量增加 
		m_score->addPropDelay(1);
		m_score->storePropDelay();

		num = String::createWithFormat("%d", CScore::getInstance()->getPropDelay())->_string;
		tDelay->setString(num);
	}
	else
	{
		//非法点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}
	}
}

void CShop::onHintItemCallback(Ref* pSender)
{
	std::string num;
	Label* tRemain = (Label*)this->getChildByTag(101);
	Label* tHint = (Label*)this->getChildByTag(105);

	auto m_score = CScore::getInstance();

	log("Gold %d", m_score->getGold());

	if (m_score->getGold() >= 300 && m_score->getPropHint()<100)
	{
		//点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickOk();
		}

		//金币数量减少
		m_score->useGold(300);
		m_score->storeGold();

		log("Gold use %d", m_score->getGold());

		num = String::createWithFormat("%d", CScore::getInstance()->getGold())->_string;
		tRemain->setString(num);

		//道具数量增加 
		m_score->addPropHint(1);
		m_score->storePropHint();

		num = String::createWithFormat("%d", CScore::getInstance()->getPropHint())->_string;
		tHint->setString(num);
	}
	else
	{
		//非法点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}
	}
}

//返回主界面
void CShop::onReturnItemCallback(Ref* pSender)
{
	//点击
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->changeScene();
	}

	//返回主界面
	auto scene = GameStart::createScene();
	Director::getInstance()->replaceScene(TransitionPageTurn::create(0.5f, scene,true));
}

/*

//金币和积分兑换
void CShop::onChangeItemCallback(Ref* pSender)
{
	std::string num;
	Label* tRemain = (Label*)this->getChildByTag(101);
	Label* tTotal = (Label*)this->getChildByTag(100);

	auto m_score = CScore::getInstance();

	log("Total %d", AdHelp::queryPoints());

	int m_total = AdHelp::queryPoints();

	if (m_total>0)
	{
		//点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickOk();
		}

		//金币数量增加
		m_score->addGold(m_total);
		m_score->storeGold();

		log("Total -> Gold %d", m_score->getGold());

		num = String::createWithFormat("%d", CScore::getInstance()->getGold())->_string;
		tRemain->setString(num);

		//积分数量减少
		if (!AdHelp::spendPoints(m_total))
		{
			AdHelp::spendPoints(m_total);
		}

		num = String::createWithFormat("%d", AdHelp::queryPoints())->_string;
		tTotal->setString(num);
	}
	else
	{
		//非法点击
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}
	}


}
*/

void CShop::onExit()
{
	Layer::onExit();
	this->removeAllChildrenWithCleanup(true);
}
