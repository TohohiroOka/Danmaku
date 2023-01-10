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
	KeepName("Resources/Sprite/bossHPGaugeOut.png", SPRITE::BOSS_HP_OUT, { 700,70 });
	KeepName("Resources/Sprite/heartPanel.png", SPRITE::PLAYER_HP_PANEL, { 512,512 });
	KeepName("Resources/Sprite/gaugeIn.png", SPRITE::PLAYER_HP_IN, { 512,128 });
	KeepName("Resources/Sprite/gaugeOut.png", SPRITE::PLAYER_HP_OUT, { 512,128 });
	KeepName("Resources/Sprite/Energy.png", SPRITE::PLAYER_BULLET_ENERGY, { 512,512 });
}