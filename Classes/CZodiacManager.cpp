/*
************************************************************************
*
*	CZodiacManager.cpp
*	Author: 杜星飞
*   describe: 管理精灵类的产生，交换，消除 以及 游戏状态
*
************************************************************************
*/

#include "CZodiacManager.h"
#include "AudioController.h"
#include "ZodiacParticle.h"
#include "GamePause.h"

using namespace CocosDenshion;

CZodiacManager* CZodiacManager::create(int row, int col)
{
	auto m_zodiacManager = new CZodiacManager();
	if (m_zodiacManager && m_zodiacManager->init(row,col))
	{
		m_zodiacManager->autorelease();
		return m_zodiacManager;
	}
	else
	{
		CC_SAFE_DELETE(m_zodiacManager);
		return nullptr;
	}
}

bool CZodiacManager::init(int row, int col)
{
	if (!Node::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
/*
	
	this->ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vec2(1,0));
	this->setPosition(Vec2(visibleSize.width/2,0));
*/

	auto m_textureCache = TextureCache::getInstance();

	//初始化行列
	m_row = row;
	m_col = col;

	//初始化选中、交换的生肖精灵为空
	m_zodiacSelected = nullptr;
	m_zodiacSwaped = nullptr;

	//初始化生肖精灵组的大小
	m_zodiacBox.resize(m_row);
	for (auto &vec : m_zodiacBox)
	{
		vec.resize(m_col);
	}

	//创建生肖精灵组
	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col;j++)
		{
			m_zodiacBox[i][j] = createZodiac(i, j);
		}
	}

	//判断生肖精灵组是否合法，不合法的话继续创建
	while (isDeadZodiacGroup())
	{
		updateZodiacGroup();
	}

	//触摸事件响应函数
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(CZodiacManager::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(CZodiacManager::onTouchMoved, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	log("CZodiacManager init.");

	//当前分数和关卡、游戏难度、道具使用
	m_scoreLevel = ScoreLevel::create();
	m_scoreLevel->RefreshData();
	this->addChild(m_scoreLevel);

	//进度条框架
	//倒计时条外框
	auto m_bounusbar_frame = Sprite::createWithTexture(m_textureCache->getTextureForKey("bonusbar.png"));
	m_bounusbar_frame->setPosition(Vec2(visibleSize.width, m_row*Grid_Width));
	m_bounusbar_frame->setAnchorPoint(Vec2(1,0));
//	m_bounusbar_frame->setVisible(false);

	this->addChild(m_bounusbar_frame);

	//进度条初始化
	m_bar = LoadingBar::create("bonusbar_fill.png");
	m_bar->setPercent(100);
	m_bar->setDirection(LoadingBar::Direction::DOWN);
	m_bar->setPosition(m_bounusbar_frame->getPosition());
	m_bar->setAnchorPoint(Vec2(1,0));
	this->addChild(m_bar);

	this->schedule(schedule_selector(CZodiacManager::onReducingBonus), 0.5);

	//暂停游戏
	Sprite* spriteNor4;
	Sprite* spriteSel4;

	spriteNor4 = Sprite::create("pause.png");
	spriteSel4 = Sprite::create("pause.png");

	spriteSel4->setScale(1.2);

	auto pauseItem = MenuItemSprite::create(
		spriteNor4,
		spriteSel4
		);
	pauseItem->setCallback(CC_CALLBACK_1(CZodiacManager::onPauseItemCallback, this));

	////////////使用延时、提示 道具/////////////

	auto m_score = CScore::getInstance();

	//使用道具的数量限制
	m_delayNum = std::min(m_score->getInstance()->getPropDelay(), 5);
	m_hintNum = std::min(m_score->getInstance()->getPropHint(), 5);

	//延时道具
	Sprite* spriteNor5;
	Sprite* spriteSel5;

	if (m_score->getPropDelay() > 0)
	{
		spriteNor5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_delay.png"));
		spriteSel5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_delay.png"));
	}
	else
	{
		spriteNor5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_delay1.png"));
		spriteSel5 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_delay1.png"));
	}

	spriteSel5->setScale(1.2);

	delayItem = MenuItemSprite::create(
		spriteNor5,
		spriteSel5
		);
	delayItem->setCallback(CC_CALLBACK_1(CZodiacManager::onDelayItemCallback, this));

	//延时道具数量
	m_labelDelay = Label::createWithCharMap(
		"digital.png",
		20, 24,
		'0');
	m_labelDelay->setString(String::createWithFormat("%d", m_score->getPropDelay())->_string);
	m_labelDelay->setAlignment(TextHAlignment::LEFT);
	m_labelDelay->setAnchorPoint(Vec2(0, 0));
	m_labelDelay->setPosition(Vec2(delayItem->getContentSize().width*1.5, m_row*Grid_Width));
	this->addChild(m_labelDelay);

	//提示道具
	Sprite* spriteNor6;
	Sprite* spriteSel6;

	if (m_score->getPropHint() > 0)
	{
		spriteNor6 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_hint.png"));
		spriteSel6 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_hint.png"));
	}
	else
	{
		spriteNor6 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_hint1.png"));
		spriteSel6 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_hint1.png"));
	}

	spriteSel6->setScale(1.2);

	hintItem = MenuItemSprite::create(
		spriteNor6,
		spriteSel6
		);
	hintItem->setCallback(CC_CALLBACK_1(CZodiacManager::onHintItemCallback, this));

	//提示道具数量
	m_labelHint = Label::createWithCharMap(
		"digital.png",
		20, 24,
		'0');
	m_labelHint->setString(String::createWithFormat("%d", m_score->getPropHint())->_string);
	m_labelHint->setAlignment(TextHAlignment::LEFT);
	m_labelHint->setAnchorPoint(Vec2(0, 0));
	m_labelHint->setPosition(Vec2(delayItem->getContentSize().width * 3, m_row*Grid_Width));
	this->addChild(m_labelHint);

	auto menu1 = Menu::create(pauseItem, delayItem, hintItem, nullptr);
	menu1->alignItemsHorizontallyWithPadding(hintItem->getContentSize().width / 2);
	menu1->setAnchorPoint(Vec2(0, 0));
	menu1->setPosition(Vec2(hintItem->getContentSize().width * 2, m_row*Grid_Width+m_labelHint->getContentSize().height*2));
	addChild(menu1);

	return true;
}

/////////////获取游戏进度/////////////////////

int CZodiacManager::getReducing()
{
	return m_bar->getPercent();
}

/////////////设定游戏进度/////////////////////
void CZodiacManager::addReducing()
{
	if (m_bar->getPercent()<100)
	{
		m_bar->setPercent(m_bar->getPercent() + 2);
	}
}

//本局游戏进度条
void CZodiacManager::onReducingBonus(float dt)
{
	m_bar->setPercent(m_bar->getPercent() - 1);
	log("Percent %lf", m_bar->getPercent());

	auto m_upScore = CScore::getInstance();
	//倒计时结束，游戏结束，保存游戏分数
	if (m_bar->getPercent() == 0 || m_upScore->getCurScore() >= m_upScore->getNextScore())
	{
		this->unschedule(schedule_selector(CZodiacManager::onReducingBonus));

		log("game over!");
	}
}

//清除 生肖精灵组
void CZodiacManager::deleteZodiacGroup()
{
	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			m_zodiacBox[i][j]->removeFromParent();
		}
	}

	log("delete a CZodiacManager!");
}

//更新 生肖精灵组
void CZodiacManager::updateZodiacGroup()
{
	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			m_zodiacBox[i][j]->removeFromParent();
			m_zodiacBox[i][j] = createZodiac(i, j);
		}
	}
	log("update a new CZodiacManager!");
}

//判断 生肖精灵组 是否 已不能移动
bool CZodiacManager::isDeadZodiacGroup()
{
	bool bRect = true;

	//交换函数 lambda表达式
	auto swap = [](CZodiac** lhs, CZodiac** rhs)
	{
		auto temp = *lhs;
		*lhs = *rhs;
		*rhs = temp;
	};

	for (int i = 0; i < m_row; i++)
	{
		if (isZodiacDestory())
		{
			bRect = false;
			break;;
		}
		
		for (int j = 0; j < m_col; j++)
		{
			//和左边交换
			if (i>0)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i-1][j]);
				if (isZodiacDestory())
				{
					bRect = false;
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i-1][j]);
					break;;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i-1][j]);
			}

			//和右边交换
			if (i<m_col-1)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i+1][j]);
				if (isZodiacDestory())
				{
					bRect = false;
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i+1][j]);
					break;;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i+1][j]);
			}

			//和下边交换
			if (j > 0)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j - 1]);
				if (isZodiacDestory())
				{
					bRect = false;
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j - 1]);
					break;;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j - 1]);
			}

			//和上边交换
			if (j < m_row-1)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j + 1]);
				if (isZodiacDestory())
				{
					bRect = false;
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j + 1]);
					break;;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j + 1]);
			}
		}
	}

	m_destoryZodiacBox.clear();

	return bRect;
}
/////////////////////创建生肖精灵/////////////////////////

//创建生肖精灵
CZodiac* CZodiacManager::createZodiac(int row, int col)
{
	CZodiac* m_zodiac = nullptr;

	//创建一个合法的生肖精灵
	while (1)
	{
		if (1==CScore::getInstance()->getGameDegree())
		{
			m_zodiac = CZodiac::create(cocos2d::random(First_Zodiac_Id, Easy_Zodiac_Id), Vec2(row, col));
		}
		else if (2 == CScore::getInstance()->getGameDegree())
		{
			m_zodiac = CZodiac::create(cocos2d::random(First_Zodiac_Id, Normal_Zodiac_Id), Vec2(row, col));
		}
		else if (3 == CScore::getInstance()->getGameDegree())
		{
			m_zodiac = CZodiac::create(cocos2d::random(First_Zodiac_Id, Hard_Zodiac_Id), Vec2(row, col));
		}
		if (isWellZodiac(m_zodiac,row,col))
		{
			break;
		}
	}

	//设置生肖精灵的位置
	setZodiacPos(m_zodiac, row, col);

	this->addChild(m_zodiac);

	return m_zodiac;
}

//判断所创建生肖精灵是否合法
bool CZodiacManager::isWellZodiac(CZodiac* zodiac, int row, int col)
{
	/***************************************************************************/
	/*根据生肖精灵组的创建顺序，只需判断新创建生肖精灵在 向左、向下方向是否合法*/
	/*即，在x轴、y轴 负方向判断即可                                            */
	/***************************************************************************/

	bool m_bWellRow = true;
	bool m_bWellCol = true;

	if (row>1)
	{
		if ((zodiac->getType() == m_zodiacBox[row - 1][col]->getType()) && (zodiac->getType() == m_zodiacBox[row - 2][col]->getType()))
		{
			m_bWellRow = false;
		}
	}

	if (col>1)
	{
		if ((zodiac->getType() == m_zodiacBox[row][col - 1]->getType()) && (zodiac->getType() == m_zodiacBox[row][col - 2]->getType()))
		{
			m_bWellCol = false;
		}
	}

	return m_bWellCol&&m_bWellRow;
}

//设置生肖精灵的位置
void CZodiacManager::setZodiacPos(CZodiac* zodiac, float row, float col)
{
	zodiac->setPosition(Vec2(row*Grid_Width, col*Grid_Width));
}


////////////////////操作生肖精灵////////////////////////////////

//交换生肖精灵位置
void CZodiacManager::swapZodiacPos(CZodiac* lhs, CZodiac* rhs)
{
	//暂停事件监听
	_eventDispatcher->pauseEventListenersForTarget(this);

	//交换生肖精灵在生肖精灵组的位置
	auto m_temp1 = m_zodiacBox[lhs->getPos().x][lhs->getPos().y];
	m_zodiacBox[lhs->getPos().x][lhs->getPos().y] = m_zodiacBox[rhs->getPos().x][rhs->getPos().y];
	m_zodiacBox[rhs->getPos().x][rhs->getPos().y] = m_temp1;

	//交换两个生肖精灵的位置
	auto m_temp2 = lhs->getPos();
	lhs->setPos(rhs->getPos());
	rhs->setPos(m_temp2);

	//移动生肖精灵的位置
	moveZodiacToNewPos(lhs);
	moveZodiacToNewPos(rhs);
}

//移动生肖精灵位置
void CZodiacManager::moveZodiacToNewPos(CZodiac* zodiac)
{
	zodiac->setSwaping(true);
	auto m_action = MoveTo::create(Move_Speed, Vec2(zodiac->getPos().x*Grid_Width,zodiac->getPos().y*Grid_Width));
	auto m_call = CallFunc::create([zodiac]()
	{
		zodiac->setSwaping(false);
	}
	);

	zodiac->runAction(Sequence::create(m_action, m_call, nullptr));
}

//刷新生肖精灵组
void CZodiacManager::refreshZodiacGrid()
{
	int m_count;
	for (int i = 0; i < m_row; i++)
	{
		m_count = 0;
		for (int j = 0; j < m_col; j++)
		{
			if (!m_zodiacBox[i][j])
			{
				++m_count;
				break;
			}
		}
		if (m_count)
		{
			refreshZodiacByCol(i);
		}
	}
}

//按 列 刷新生肖精灵组
void CZodiacManager::refreshZodiacByCol(int col)
{
	int m_count = 0;	
	auto m_pZodiacBox = &m_zodiacBox;

	//之前的生肖精灵下移
	for (int i = 0; i < m_col; i++)
	{
		auto m_zodiac = m_zodiacBox[col][i];
		if (!m_zodiac)
		{
			++m_count;
		}
		else if (m_count)
		{
			m_zodiac->setPos(Vec2(m_zodiac->getPos().x, m_zodiac->getPos().y - m_count));

			auto m_action = MoveBy::create(Move_Speed, Vec2(0, -m_count*Grid_Width));
			auto m_call = CallFunc::create([m_pZodiacBox,m_zodiac]()
			{
				(*m_pZodiacBox)[m_zodiac->getPos().x][m_zodiac->getPos().y] = m_zodiac;
			}
			);

			m_zodiac->runAction(Sequence::create(m_action,m_call,nullptr));
		}
	}

	//新的生肖精灵下移
	int m_delta = 1;
	for (auto m_zodiac : m_newZodiacBox)
	{
		if (col==m_zodiac->getPos().x)
		{
			m_zodiac->setPos(Vec2(m_zodiac->getPos().x, m_col - m_count));

			auto m_action = MoveBy::create(Move_Speed*m_delta++, Vec2(0,-m_count--*Grid_Width));
			auto m_call = CallFunc::create([m_zodiac,m_pZodiacBox,this]()
			{
				(*m_pZodiacBox)[m_zodiac->getPos().x][m_zodiac->getPos().y] = m_zodiac;
				m_newZodiacBox.eraseObject(m_zodiac);
			}
			);

			m_zodiac->runAction(Sequence::create(DelayTime::create(Move_Speed),m_action, m_call, nullptr));
		}
	}


}

bool CZodiacManager::onTouchBegan(Touch* touch, Event* unused_event)
{
	auto m_beganPos = this->convertToNodeSpace(touch->getLocation());

	//判断 触摸点 是否在生肖精灵组上面
	if (Rect(0,0,m_row*Grid_Width,m_col*Grid_Width).containsPoint(m_beganPos))
	{
		int m_beganPosX = m_beganPos.x / Grid_Width;
		int m_beganPosY = m_beganPos.y / Grid_Width;

		m_zodiacSelected = m_zodiacBox[m_beganPosX][m_beganPosY];

		log("touch coordinate: x=%d,y=%d jewel's type:%d", m_beganPosX, m_beganPosY, m_zodiacSelected->getType());

		return true;
	}
	
	return false;
}

void CZodiacManager::onTouchMoved(Touch* touch, Event* unused_event)
{
	if (!m_zodiacSelected)
	{
		return;
	}

	auto m_beganPos = m_zodiacSelected->getPos();
	auto m_movePos = this->convertToNodeSpace(touch->getLocation());
	int m_movePosX = m_movePos.x / Grid_Width;
	int m_movePosY = m_movePos.y / Grid_Width;
	auto m_touchedPos = Vec2(m_movePosX, m_movePosY);

	//判断 触摸点 是否在 生肖精灵组 内 && 判断 当前触摸点 和 之前触摸点 是否是同一个点
	if (!Rect(0, 0, m_row*Grid_Width, m_col*Grid_Width).containsPoint(m_movePos) || (m_touchedPos == m_beganPos))
	{
		return;
	}

	//判断 当前触摸点 和 之前触摸点 是否同行、列
	if (abs(m_beganPos.x - m_touchedPos.x) + abs(m_beganPos.y - m_touchedPos.y)!=1)
	{
		return;
	}

	m_zodiacSwaped = m_zodiacBox[m_touchedPos.x][m_touchedPos.y];

	//交换生肖精灵
	swapZodiacPos(m_zodiacSelected, m_zodiacSwaped);

	//进入 生肖精灵交换状态
	this->schedule(schedule_selector(CZodiacManager::onZodiacSwaping));
	
}

/////////////////消除生肖精灵////////////////////////

//判断是否能够消除
bool CZodiacManager::isZodiacDestory()
{
	int m_count = 0;
	int i, j;
	CZodiac* m_zodiacFirst = nullptr;
	CZodiac* m_zodiacNext = nullptr;
	//按列遍历
	for (i = 0; i < m_row; i++)
	{
		for (j = 0; j < m_col-2;)
		{
			m_count = 1;
			m_zodiacFirst = m_zodiacBox[i][j];
			m_zodiacNext = m_zodiacBox[i][j+1];
			while (m_zodiacFirst->getType()==m_zodiacNext->getType())
			{
				++m_count;
				if (j+m_count>m_col-1)
				{
					break;
				}

				m_zodiacNext = m_zodiacBox[i][j + m_count];
			}

			if (m_count>=3)
			{
				for (int y = 0; y < m_count;y++)
				{
					auto m_zoidac = m_zodiacBox[i][j + y];
					m_destoryZodiacBox.pushBack(m_zoidac);
				}
			}

			j += m_count;
		}
	}

	//按行遍历
	for (i = 0; i < m_row; i++)
	{
		for (j = 0; j < m_col - 2;)
		{
			m_count = 1;
			m_zodiacFirst = m_zodiacBox[j][i];
			m_zodiacNext = m_zodiacBox[j+1][i];
			while (m_zodiacFirst->getType() == m_zodiacNext->getType())
			{
				++m_count;
				if (j + m_count>m_col - 1)
				{
					break;
				}

				m_zodiacNext = m_zodiacBox[j + m_count][i];
			}

			if (m_count >= 3)
			{
				for (int y = 0; y < m_count; y++)
				{
					auto m_zoidac = m_zodiacBox[j + y][i];

					//防止重复添加
					if (m_destoryZodiacBox.find(m_zoidac) != m_destoryZodiacBox.end())
					{
						continue;
					}

					m_destoryZodiacBox.pushBack(m_zoidac);
				}
			}

			j += m_count;
		}
	}

	if (m_destoryZodiacBox.empty())
	{
		return false;
	}

	return true;

}

//消除
void CZodiacManager::zodiacDestory()
{
	for (auto m_zodiac : m_destoryZodiacBox)
	{
		CZodiac* newZodiac = nullptr;
		if (1 == CScore::getInstance()->getGameDegree())
		{
			newZodiac = CZodiac::create(cocos2d::random(First_Zodiac_Id, Easy_Zodiac_Id), Vec2(m_zodiac->getPos().x, m_col));
		}
		else if (2 == CScore::getInstance()->getGameDegree())
		{
			newZodiac = CZodiac::create(cocos2d::random(First_Zodiac_Id, Normal_Zodiac_Id), Vec2(m_zodiac->getPos().x, m_col));
		}
		else if (3 == CScore::getInstance()->getGameDegree())
		{
			newZodiac = CZodiac::create(cocos2d::random(First_Zodiac_Id, Hard_Zodiac_Id), Vec2(m_zodiac->getPos().x, m_col));
		}
//		auto newZodiac = CZodiac::create(random(First_Zodiac_Id, Last_Zodiac_Id), Vec2(m_zodiac->getPos().x, m_col));
		setZodiacPos(newZodiac, newZodiac->getPos().x, m_col);
		this->addChild(newZodiac);

		m_newZodiacBox.pushBack(newZodiac);

		m_zodiacBox[m_zodiac->getPos().x][m_zodiac->getPos().y] = nullptr;

		//粒子特效
		showParticleEffect(m_zodiac->getPosition(), this);

		//消除
		m_zodiac->fDestory();	
		
		//更新分数
		CScore::getInstance()->addCurScore();

		//更新进度
		this->addReducing();

	}

	//更新分数显示
	m_scoreLevel->RefreshScore();

	//播放声音 成功
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->changeYes();
	}
}

/////////////////游戏状态////////////////////////

//正在交换生肖精灵
void CZodiacManager::onZodiacSwaping(float dt)
{
	//如果 当前选中项 和 交换项 还没交换完毕，则 返回继续交换
	if (m_zodiacSelected->isSwaping() || m_zodiacSwaped->isSwaping())
	{
		return;
	}
	else
	{
		//停止 交换状态
		this->unschedule(schedule_selector(CZodiacManager::onZodiacSwaping));

		log("swap over!");

		//交换完毕之后是否可以消除
		if (isZodiacDestory())
		{
			/////////////消除///////////////

			log("yes,crush!");

			m_zodiacSelected = nullptr;

			//消除 生肖精灵
			zodiacDestory();

			//进入 消除状态
			this->schedule(schedule_selector(CZodiacManager::onZodiacDestory));
		}
		else
		{
			log("no, cant crush!");

			//播放声音 失败
			if (AudioController::getInstance()->isMusicOpen())
			{
				AudioController::getInstance()->changeNo();
			}

			//恢复为 交换之前的状态
			swapZodiacPos(m_zodiacSelected, m_zodiacSwaped);

			//进入 消除状态
			this->schedule(schedule_selector(CZodiacManager::onZodiacSwapingBack));
		}
	}

}

//交换生肖精灵完毕
void CZodiacManager::onZodiacSwapingBack(float dt)
{
	if (m_zodiacSelected->isSwaping() || m_zodiacSwaped->isSwaping())
	{
		return;
	}
	else
	{
		log("swap back!");

		//停止 交换返回状态
		this->unschedule(schedule_selector(CZodiacManager::onZodiacSwapingBack));

		m_zodiacSelected = nullptr;

		//恢复事件监听
		_eventDispatcher->resumeEventListenersForTarget(this);
	}

}

//正在消除生肖精灵
void CZodiacManager::onZodiacDestory(float dt)
{
	//如果有生肖精灵正在消除，则 返回继续消除
	for (auto m_zodiac : m_destoryZodiacBox)
	{
		if (m_zodiac->isDestory())
		{
			return;
		}
	}

	this->unschedule(schedule_selector(CZodiacManager::onZodiacDestory));

	//清空 消除生肖精灵容器
	m_destoryZodiacBox.clear();

	log("crush over!");
	log("begin to refresh!");

	//刷新生肖精灵组
	refreshZodiacGrid();

	//进入刷新状态
	this->schedule(schedule_selector(CZodiacManager::onZodiacRefreshing));
}

//消除之后刷新
void CZodiacManager::onZodiacRefreshing(float dt)
{
	if (0==m_newZodiacBox.size())
	{
		this->unschedule(schedule_selector(CZodiacManager::onZodiacRefreshing));

		if (isZodiacDestory())
		{
			zodiacDestory();

			this->schedule(schedule_selector(CZodiacManager::onZodiacDestory));
		}
		else
		{
			if (isDeadZodiacGroup())
			{
				log("Dead!");

				auto m_pos = Director::getInstance()->getWinSize();

				auto m_label = Label::createWithTTF("Now is a Dead Group, Change...", "fonts/DFPShaoNvW5-GB.ttf", 35);
				m_label->setTextColor(Color4B::BLACK);
				m_label->setPosition(Vec2(m_pos.width/2,m_pos.height/3*2));
				this->getParent()->addChild(m_label);

				auto m_action1 = FadeIn::create(Move_Speed);
				auto m_action2 = FadeOut::create(Move_Speed);

				auto m_call = CallFunc::create([this,m_label]()
				{
					do 
					{
						updateZodiacGroup();
					} while (isDeadZodiacGroup());

					m_label->removeFromParent();

					_eventDispatcher->resumeEventListenersForTarget(this);
				}
				);

				m_label->runAction(Sequence::create(m_action1, m_action2, m_call, nullptr));

			}
			else
			{
				_eventDispatcher->resumeEventListenersForTarget(this);
			}
		}

	}
	else
	{
		return;
	}
}


//暂停游戏
void CZodiacManager::onPauseItemCallback(Ref* ref)
{
	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->changeScene();
	}

	RenderTexture *renderTexture = RenderTexture::create(320, 540);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	//返回主界面
	auto scene = GamePause::createScene(renderTexture);
	Director::getInstance()->pushScene(scene);
}

void CZodiacManager::useDelayProp()
{
	if (m_bar->getPercent() < 100)
	{
		m_bar->setPercent(m_bar->getPercent() + 20);
	}
}


void CZodiacManager::useHintProp()
{
	//交换函数 lambda表达式
	auto swap = [](CZodiac** lhs, CZodiac** rhs)
	{
		auto temp = *lhs;
		*lhs = *rhs;
		*rhs = temp;
	};

	CZodiac* m_zodiac1 = nullptr;
	CZodiac* m_zodiac2 = nullptr;

	for (int i = 0; i < m_row; i++)
	{

		for (int j = 0; j < m_col; j++)
		{
			//和左边交换
			if (i>0)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i - 1][j]);
				if (isZodiacDestory())
				{
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i - 1][j]);

					m_zodiac1 = m_zodiacBox[i][j];
					m_zodiac1->runAction(Blink::create(0.5, 3));

					m_zodiac2 = m_zodiacBox[i - 1][j];
					m_zodiac2->runAction(Blink::create(0.5, 3));

					m_destoryZodiacBox.clear();
					return;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i - 1][j]);
			}

			//和右边交换
			if (i < m_col - 1)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i + 1][j]);
				if (isZodiacDestory())
				{
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i + 1][j]);
					
					m_zodiac1 = m_zodiacBox[i][j];
					m_zodiac1->runAction(Blink::create(0.5, 3));

					m_zodiac2 = m_zodiacBox[i + 1][j];
					m_zodiac2->runAction(Blink::create(0.5, 3));

					m_destoryZodiacBox.clear();

					return;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i + 1][j]);
			}

			//和下边交换
			if (j > 0)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j - 1]);
				if (isZodiacDestory())
				{
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j - 1]);

					m_zodiac1 = m_zodiacBox[i][j];
					m_zodiac1->runAction(Blink::create(0.5, 3));

					m_zodiac2 = m_zodiacBox[i][j-1];
					m_zodiac2->runAction(Blink::create(0.5, 3));

					m_destoryZodiacBox.clear();

					return;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j - 1]);
			}

			//和上边交换
			if (j < m_row - 1)
			{
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j + 1]);
				if (isZodiacDestory())
				{
					swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j + 1]);
					
					m_zodiac1 = m_zodiacBox[i][j];
					m_zodiac1->runAction(Blink::create(0.5, 3));

					m_zodiac2 = m_zodiacBox[i][j+1];
					m_zodiac2->runAction(Blink::create(0.5, 3));

					m_destoryZodiacBox.clear();

					return;
				}
				swap(&m_zodiacBox[i][j], &m_zodiacBox[i][j + 1]);
			}
		}
	}
}

//使用道具 延时
void CZodiacManager::onDelayItemCallback(Ref* ref)
{
	auto m_score = CScore::getInstance();

	auto m_textureCache = TextureCache::getInstance();

	if (m_delayNum <= 0)
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->useProp();
	}

	if (1 == m_delayNum)
	{
		m_score->usePropDelay(1);
		m_score->storePropDelay();

		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_delay1.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_delay1.png"));
		spriteSel1->setScale(1.2);

		delayItem->setNormalImage(spriteNor1);
		delayItem->setSelectedImage(spriteSel1);
	}
	else
	{
		m_score->usePropDelay(1);
		m_score->storePropDelay();
	}

	--m_delayNum;
	m_labelDelay->setString(String::createWithFormat("%d", m_score->getPropDelay())->_string);

	//延时
	useDelayProp();
}

//使用道具 提示
void CZodiacManager::onHintItemCallback(Ref* ref)
{
	auto m_score = CScore::getInstance();

	auto m_textureCache = TextureCache::getInstance();

	if (m_hintNum <= 0)
	{
		if (AudioController::getInstance()->isMusicOpen())
		{
			AudioController::getInstance()->clickNo();
		}

		return;
	}

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->useProp();
	}

	if (1 == m_hintNum)
	{
		m_score->usePropHint(1);
		m_score->storePropHint();

		auto spriteNor1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_hint1.png"));
		auto spriteSel1 = Sprite::createWithTexture(m_textureCache->getTextureForKey("prop_hint1.png"));
		spriteSel1->setScale(1.2);

		hintItem->setNormalImage(spriteNor1);
		hintItem->setSelectedImage(spriteSel1);
	}
	else
	{
		m_score->usePropHint(1);
		m_score->storePropHint();
	}

	--m_hintNum;
	m_labelHint->setString(String::createWithFormat("%d", m_score->getPropHint())->_string);

	//提示
	useHintProp();
}