#include "PlayerUi.h"
#include "GameHelper.h"
#include "WindowApp.h"

using namespace DirectX;

PlayerUi::PlayerUi(const int _maxHp)
{
	maxHp = _maxHp;

	//バー外枠
	damageAlpha = 0.0f;
	damage = Sprite::Create(spriteName[SPRITE::PLAYER_HP_DOWN].name);
	damage->SetAnchorpoint({ 0,0 });
	damage->SetTexSize({ 1,1 });
	damage->SetSize(spriteName[SPRITE::PLAYER_HP_DOWN].texSize);
	damage->SetColor({ 1,1,1,damageAlpha });
	damage->Update();
}

std::unique_ptr<PlayerUi> PlayerUi::Create(const int _maxHp)
{
	// 3Dオブジェクトのインスタンスを生成
	PlayerUi* instance = new PlayerUi(_maxHp);
	if (instance == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<PlayerUi>(instance);
}

void PlayerUi::Update(const int _playerHp)
{
	damageAlpha = 1.0f - float(_playerHp) / float(maxHp);
	damage->SetColor({ 1,1,1,damageAlpha });
}

void PlayerUi::Draw()
{
	damage->Draw();
}
