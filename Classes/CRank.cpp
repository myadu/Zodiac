/*
************************************************************************
*
*	CRank.cpp
*	Author: 杜星飞
*   describe: 游戏结果排行榜 可查看在 简单、正常、困难 模式下的排名
*
************************************************************************
*/

#include "CRank.h"
#include "GameStart.h"
#include "CScore.h"
#include "AudioController.h"

// 设置一个静态的场景函数
Scene* CRank::createScene()
{
	// 给scene创建内容
	Scene *myscene = Scene::create();

	// 创建一个layer
	CRank *mylayer = CRank::create();

	// 把这个layer加入到scene中
	myscene->addChild(mylayer);

	// 返回scene
	return myscene;
}

// 初始化
bool CRank::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size mysize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto m_textureCache = TextureCache::getInstance();

	//背景图片
	Sprite *pbg = Sprite::createWithTexture(
		m_textureCache->getTextureForKey("select_degree.jpg"));
	pbg->setPosition(Vec2(origin.x + mysize.width / 2, origin.y + mysize.height / 2));
	this->addChild(pbg);

	//排行
	Sprite *pbg1 = Sprite::create("title_rankRank.png");
	pbg1->setPosition(Vec2(origin.x + mysize.width *0.4, origin.y + mysize.height *0.64));
	this->addChild(pbg1);

	//分数
	Sprite *pbg2 = Sprite::create("title_scoreRank.png");
	pbg2->setPosition(Vec2(origin.x + mysize.width *0.7, origin.y + mysize.height *0.64));
	this->addChild(pbg2);

	//返回按钮
	Scale9Sprite *spback = Scale9Sprite::create("return.png");
	ControlButton *pbtbk = ControlButton::create(spback);
	pbtbk->setPosition(Vec2(origin.x + mysize.width/8, origin.y + mysize.height*7/8));
	pbtbk->setPreferredSize(Size(64, 54));
	pbtbk->addTargetWithActionForControlEvents(
		this,
		cccontrol_selector(CRank::backToMenu),
		Control::EventType::TOUCH_UP_INSIDE);
	this->addChild(pbtbk);

	///////////////简单、正常、困难 排行榜///////////////
	//显示排行榜 简单 按钮
	Sprite* spriteNor1;
	Sprite* spriteSel1;

	if (1 == CScore::getInstance()->getGameDegree())
	{
		m_nowDegree = 1001;
		m_sDegreeName = "EasyNum";

		spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));  
		spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
	}
	else
	{
		spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
	}

	spriteNor1->setScale(0.8);
//	spriteSel1->setScale(1.2);

	easyItem = MenuItemSprite::create(
		spriteNor1,
		spriteSel1
		);
	easyItem->setCallback(CC_CALLBACK_1(CRank::onEasyItemCallback, this));

	//游戏难度 正常 按钮
	Sprite* spriteNor2;
	Sprite* spriteSel2;

	if (2 == CScore::getInstance()->getGameDegree())
	{
		m_nowDegree = 1002;
		m_sDegreeName = "NormalNum";

		spriteNor2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal1.png"));
		spriteSel2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal1.png"));
	}
	else
	{
		spriteNor2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		spriteSel2 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
	}

	spriteNor2->setScale(0.8);
//	spriteSel2->setScale(1.2);

	normalItem = MenuItemSprite::create(
		spriteNor2,
		spriteSel2
		);
	normalItem->setCallback(CC_CALLBACK_1(CRank::onNormalItemCallback, this));

	//游戏难度 困难 按钮
	Sprite* spriteNor3;
	Sprite* spriteSel3;

	if (3 == CScore::getInstance()->getGameDegree())
	{
		m_nowDegree = 1003;
		m_sDegreeName = "HardNum";

		spriteNor3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard1.png"));
		spriteSel3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard1.png"));
	}
	else
	{
		spriteNor3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		spriteSel3 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
	}

	spriteNor3->setScale(0.8);
//	spriteSel3->setScale(1.2);

	hardItem = MenuItemSprite::create(
		spriteNor3,
		spriteSel3
		);
	hardItem->setCallback(CC_CALLBACK_1(CRank::onHardItemCallback, this));

	auto menu = Menu::create(easyItem, normalItem, hardItem, nullptr);
	menu->alignItemsVerticallyWithPadding(hardItem->getContentSize().height);
//	menu->setAnchorPoint(Vec2(1,1));
	menu->setPosition(Vec2(hardItem->getContentSize().width/2, mysize.height*0.4));
	this->addChild(menu);



	TableView * tbeasy = TableView::create(this, Size(mysize.width*0.9,mysize.height*0.5));
	tbeasy->setDirection(ScrollView::Direction::VERTICAL);
	tbeasy->setPosition(Vec2(mysize.width*0.1, mysize.height*0.1));
//	tbeasy->setPosition(Vec2::ZERO);
	tbeasy->setDelegate(this);
	tbeasy->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	this->addChild(tbeasy, 1, 3005);

	return true;
}

//显示排行榜 简单
void CRank::onEasyItemCallback(Ref* ref)
{
	if (1001 == m_nowDegree)
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

	//取消灰显
	auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
	auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy1.png"));
	spriteNor1->setScale(0.8);

	easyItem->setNormalImage(spriteNor1);
	easyItem->setSelectedImage(spriteSel1);
	/*

	normalItem->setEnabled(false);
	hardItem->setEnabled(false);

	*/
	if (1002 == m_nowDegree)
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		spriteNor1->setScale(0.8);

		normalItem->setNormalImage(spriteNor1);
		normalItem->setSelectedImage(spriteSel1);
	}
	else if (1003 == m_nowDegree)
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		spriteNor1->setScale(0.8);

		hardItem->setNormalImage(spriteNor1);
		hardItem->setSelectedImage(spriteSel1);
	}

	m_nowDegree = 1001;

	m_sDegreeName = "EasyNum";

	TableView *tb = (TableView *)this->getChildByTag(3005);
	tb->reloadData();
}

//显示排行榜 一般
void CRank::onNormalItemCallback(Ref* ref)
{
	if (1002 == m_nowDegree)
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
	spriteNor1->setScale(0.8);

	normalItem->setNormalImage(spriteNor1);
	normalItem->setSelectedImage(spriteSel1);
	/*

	easyItem->setEnabled(false);
	hardItem->setEnabled(false);

	*/
	if (1001 == m_nowDegree)
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		spriteNor1->setScale(0.8);

		easyItem->setNormalImage(spriteNor1);
		easyItem->setSelectedImage(spriteSel1);
	}
	else if (1003 == m_nowDegree)
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_hard.png"));
		spriteNor1->setScale(0.8);

		hardItem->setNormalImage(spriteNor1);
		hardItem->setSelectedImage(spriteSel1);
	}

	m_nowDegree = 1002;

	m_sDegreeName = "NormalNum";

	TableView *tb = (TableView *)this->getChildByTag(3005);
	tb->reloadData();
}

//显示排行榜 困难
void CRank::onHardItemCallback(Ref* ref)
{
	if (1003 == m_nowDegree)
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
	spriteNor1->setScale(0.8);

	hardItem->setNormalImage(spriteNor1);
	hardItem->setSelectedImage(spriteSel1);
	/*

	easyItem->setEnabled(false);
	normalItem->setEnabled(false);
	*/

	if (1001 == m_nowDegree)
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_easy.png"));
		spriteNor1->setScale(0.8);

		easyItem->setNormalImage(spriteNor1);
		easyItem->setSelectedImage(spriteSel1);
	}
	else if (1002 == m_nowDegree)
	{
		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("tollgate_normal.png"));
		spriteNor1->setScale(0.8);

		normalItem->setNormalImage(spriteNor1);
		normalItem->setSelectedImage(spriteSel1);
	}

	m_nowDegree = 1003;

	m_sDegreeName = "HardNum";

	TableView *tb = (TableView *)this->getChildByTag(3005);
	tb->reloadData();
}

//一共多少项
ssize_t CRank::numberOfCellsInTableView(TableView *table)
{
	int cnt = UserDefault::getInstance()->getIntegerForKey(m_sDegreeName.c_str(), 0);
	return std::max(cnt, 6);
}
//生成列表每一项的内容
TableViewCell* CRank::tableCellAtIndex(TableView *table, ssize_t idx)
{
	Size mysize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//读取数据
	std::string str = m_sDegreeName + String::createWithFormat("%d", idx + 1)->_string;

	TableViewCell* cell = table->cellAtIndex(idx);
	if (!cell)
	{
		cell = new TableViewCell();
		cell->autorelease();
/*
		Sprite *item = Sprite::create("tb.png");
		item->setPosition(Vec2(origin.x + 180, 15));
		cell->addChild(item);
*/

		int data = UserDefault::getInstance()->getIntegerForKey(str.c_str(), 0);

		auto item0 = Label::createWithCharMap(
			"digital.png",
			20, 24,
			'0');
		item0->setString(String::createWithFormat("%d", idx+1)->_string);
		item0->setAlignment(TextHAlignment::LEFT);
		item0->setPosition(Vec2(origin.x + mysize.width/3, 25));
		cell->addChild(item0);

		auto item1 = Label::createWithCharMap(
			"digital.png",
			20, 24,
			'0');
		item1->setString(String::createWithFormat("%d", data)->_string);
		item1->setAlignment(TextHAlignment::LEFT);

		item1->setPosition(Vec2(mysize.width *0.6, 25));
		cell->addChild(item1);
	}
	return cell;
}

//每一项的宽度和高度
Size CRank::cellSizeForTable(TableView *table)
{
	return Size(225, 30);
}

Size CRank::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
	return Size(225, 35);
}

//菜单回调
void CRank::backToMenu(Ref* pSender, Control::EventType controlEvent)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->changeScene();
	}

	Scene *menuscene = GameStart::createScene();
	auto *tt = TransitionPageTurn::create(0.5f, menuscene, true);
	Director::getInstance()->replaceScene(tt);
}

//生命周期
void CRank::onEnter()
{
	Layer::onEnter();
}

void CRank::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void CRank::onExit()
{
	Layer::onExit();
	this->removeAllChildrenWithCleanup(true);
}
