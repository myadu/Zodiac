/*
************************************************************************
*
*	ZodiacParticle.h
*	Author: ¶ÅÐÇ·É
*   describe: Á£×ÓÌØÐ§
*
************************************************************************
*/

#include "ZodiacParticle.h"

void showParticleEffect(Point position, Node* node){
	ParticleFireworks* _emitter = ParticleFireworks::create();

	std::string m_emitterPNG = String::createWithFormat("a%d.png", random(1, 12))->_string;

	_emitter->setTexture(TextureCache::getInstance()->getTextureForKey(m_emitterPNG));
	_emitter->setTotalParticles(12);
/*
	_emitter->setLife(1.0f);
	_emitter->setLifeVar(0.5f);
*/

	_emitter->setStartColor(Color4F::WHITE);
	_emitter->setStartColorVar(Color4F(0, 0, 0, 1));
	_emitter->setEndColor(Color4F::WHITE);
	_emitter->setEndColorVar(Color4F(0, 0, 0, 1));


//	_emitter->setEmissionRate(3.0f);

	_emitter->setStartSize(35.0f);

	_emitter->setDuration(1.0f);

	_emitter->setPosition(position);

	_emitter->setGravity(Point(0, -300));

	node->addChild(_emitter);

	log("ZodiacParticle");
}