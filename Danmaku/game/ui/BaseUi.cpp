#include "BaseUi.h"
#include "GameHelper.h"

using namespace DirectX;

std::map<BaseUi::SPRITE, BaseUi::SPRITE_INFO> BaseUi::spriteName;

void BaseUi::KeepName(const std::string& _name, const SPRITE _sprite,const XMFLOAT2& _texSize)
{
	spriteName[_sprite].name = _name;
	spriteName[_sprite].texSize = _texSize;
	Sprite::LoadTexture(spriteName[_sprite].name, spriteName[_sprite].name, true);
}

void BaseUi::StaticInitialize()
{
	KeepName("Resources/Sprite/bossHPGaugeIn.png", SPRITE::BOSS_HP_IN, { 694,20 });
	KeepName("Resources/Sprite/bossHPGaugeOut.png", SPRITE::BOSS_HP_OUT, { 700,40 });
	KeepName("Resources/Sprite/PlayerHpDown.png", SPRITE::PLAYER_HP_DOWN, { 1280,720 });
}