#include "HelloWorldScene.h"
#include "GameStart.h"
#include "AudioController.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 3. add your codes below...

	/************************************************************************/
	/* 加载资源                                                             */
	/************************************************************************/

	m_textureNum = 0;
	int* pTempNum = &m_textureNum;

	//加载Texture回调函数
	auto m_call = [pTempNum](Texture2D*)
	{
		(*pTempNum)++;
		log("loading texture %d",*pTempNum);
	};

	//异步加载纹理图片-生肖
	TextureCache::getInstance()->addImageAsync("1.png", m_call);
	TextureCache::getInstance()->addImageAsync("2.png", m_call);
	TextureCache::getInstance()->addImageAsync("3.png", m_call);
	TextureCache::getInstance()->addImageAsync("4.png", m_call);
	TextureCache::getInstance()->addImageAsync("5.png", m_call);
	TextureCache::getInstance()->addImageAsync("6.png", m_call);
	TextureCache::getInstance()->addImageAsync("7.png", m_call);
	TextureCache::getInstance()->addImageAsync("8.png", m_call);
	TextureCache::getInstance()->addImageAsync("9.png", m_call);
	TextureCache::getInstance()->addImageAsync("10.png", m_call);
	TextureCache::getInstance()->addImageAsync("11.png", m_call);
	TextureCache::getInstance()->addImageAsync("12.png", m_call);

	TextureCache::getInstance()->addImageAsync("a1.png", m_call);
	TextureCache::getInstance()->addImageAsync("a2.png", m_call);
	TextureCache::getInstance()->addImageAsync("a3.png", m_call);
	TextureCache::getInstance()->addImageAsync("a4.png", m_call);
	TextureCache::getInstance()->addImageAsync("a5.png", m_call);
	TextureCache::getInstance()->addImageAsync("a6.png", m_call);
	TextureCache::getInstance()->addImageAsync("a7.png", m_call);
	TextureCache::getInstance()->addImageAsync("a8.png", m_call);
	TextureCache::getInstance()->addImageAsync("a9.png", m_call);
	TextureCache::getInstance()->addImageAsync("a10.png", m_call);
	TextureCache::getInstance()->addImageAsync("a11.png", m_call);
	TextureCache::getInstance()->addImageAsync("a12.png", m_call);

	//异步加载纹理图片-背景
	TextureCache::getInstance()->addImageAsync("bg1.jpg", m_call);
	TextureCache::getInstance()->addImageAsync("bg2.jpg", m_call);
	TextureCache::getInstance()->addImageAsync("bg3.jpg", m_call);
	TextureCache::getInstance()->addImageAsync("bg4.jpg", m_call);

	TextureCache::getInstance()->addImageAsync("bg_main.jpg", m_call);
	TextureCache::getInstance()->addImageAsync("select_degree.jpg", m_call);

	//异步加载纹理图片-按钮
	TextureCache::getInstance()->addImageAsync("button_about.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_again.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_degree.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_goto.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_quit.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_rank.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_restart.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_return.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_shop.png", m_call);
	TextureCache::getInstance()->addImageAsync("button_start.png", m_call);
	TextureCache::getInstance()->addImageAsync("rest_gold.png", m_call);

	TextureCache::getInstance()->addImageAsync("return.png", m_call);

	//异步加载纹理图片-计时沙漏
	TextureCache::getInstance()->addImageAsync("bonusbar.png", m_call);
	TextureCache::getInstance()->addImageAsync("bonusbar_fill.png", m_call);

	//异步加载纹理图片-游戏道具、难度设置
	TextureCache::getInstance()->addImageAsync("prop_delay.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_delay1.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_gold.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_gold1.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_hint.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_hint1.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_score.png", m_call);
	TextureCache::getInstance()->addImageAsync("prop_score1.png", m_call);

	TextureCache::getInstance()->addImageAsync("tollgate_easy.png", m_call);
	TextureCache::getInstance()->addImageAsync("tollgate_easy1.png", m_call);
	TextureCache::getInstance()->addImageAsync("tollgate_hard.png", m_call);
	TextureCache::getInstance()->addImageAsync("tollgate_hard1.png", m_call);
	TextureCache::getInstance()->addImageAsync("tollgate_normal.png", m_call);
	TextureCache::getInstance()->addImageAsync("tollgate_normal1.png", m_call);

	//预加载音乐
//	AudioController::getInstance()->preLoadingAudio();

	this->schedule(schedule_selector(HelloWorld::onTextureLoading));

    return true;
}

void HelloWorld::onTextureLoading(float dt)
{
	if (58==m_textureNum)
	{
		this->unschedule(schedule_selector(HelloWorld::onTextureLoading));

		// 摇晃特效
		effectsShank();

		auto m_callFunc = CallFunc::create([]()
		{
			auto m_scene = GameStart::createScene();
			Director::getInstance()->replaceScene(m_scene);
		}
		);

		this->runAction(Sequence::create(DelayTime::create(2.0f),m_callFunc, nullptr));
	}
}

void HelloWorld::effectsShank()
{

	if (AudioController::getInstance()->isMusicOpen())
	{
		AudioController::getInstance()->loadingMusic();
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto m_node = NodeGrid::create();
	this->addChild(m_node);

	auto m_textureCache = TextureCache::getInstance();

	auto m_bg = Sprite::createWithTexture(m_textureCache->getTextureForKey("bg_main.jpg"));
	m_bg->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2));
	m_node->addChild(m_bg);

	int sizeW = visibleSize.width / 6;
	int sizeH = visibleSize.height / 8;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::string m_png = String::createWithFormat("%d.png", j+i*3+1)->_string;
			auto bg = Sprite::createWithTexture(m_textureCache->getTextureForKey(m_png));
			bg->setScale(2);
			bg->setPosition(Vec2(sizeW+j*sizeW*2,sizeH+sizeH*i*2));
			m_node->addChild(bg);
		}
	}

	auto shaky = ShakyTiles3D::create(1, Size(15, 10), 4, false);
	auto shuffle = ShuffleTiles::create(1, Size(32, 54), 3);

	m_node->runAction(Sequence::create(shaky, shuffle, nullptr));

	log("effectsShank");
}