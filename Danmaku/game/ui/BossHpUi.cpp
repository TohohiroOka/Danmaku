#include "BossHpUi.h"
#include "GameHelper.h"
#include "WindowApp.h"

using namespace DirectX;

BossHpUi::BossHpUi(const int _maxHp)
{ 
	//ゲージの最大サイズ
	gaugeSizeMax = spriteName[SPRITE::BOSS_HP_OUT].texSize;
	//サイズ変換時の割合
	gaugeRatioX = gaugeSizeMax.x / float(_maxHp);
	//実際のゲージサイズ
	bossHPGaugeSizeMax = { _maxHp * gaugeRatioX, gaugeSizeMax.y };

	//ウィンドウの幅
	float windowWidth = float(WindowApp::GetWindowWidth());

	//バー外枠
	float difference = 3.0f;
	sprite[0] = Sprite::Create(spriteName[SPRITE::BOSS_HP_OUT].name);
	sprite[0]->SetAnchorpoint({ 0,0 });
	sprite[0]->SetPosition({ windowWidth / 2.0f - gaugeSizeMax.x / 2.0f - difference, 30.0f });
	sprite[0]->SetTexSize(spriteName[SPRITE::BOSS_HP_OUT].texSize);
	sprite[0]->SetSize({ 700, 60.0f });
	sprite[0]->Update();
	//バー中身
	sprite[1] = Sprite::Create(spriteName[SPRITE::BOSS_HP_IN].name);
	sprite[1]->SetAnchorpoint({ 0,0 });
	sprite[1]->SetPosition({ windowWidth / 2.0f - gaugeSizeMax.x / 2, 95.0f });
	sprite[1]->SetTexSize(spriteName[SPRITE::BOSS_HP_IN].texSize);
	sprite[1]->SetSize({ 694,20 });
	sprite[1]->Update();
}

std::unique_ptr<BossHpUi> BossHpUi::Create(const int _maxHp)
{
	// 3Dオブジェクトのインスタンスを生成
	BossHpUi* instance = new BossHpUi(_maxHp);
	if (instance == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<BossHpUi>(instance);
}

void BossHpUi::Update(const int _bossHp)
{
	bossHPGaugeSizeMax.x = float(_bossHp) * gaugeRatioX;
	sprite[1]->SetPosition({ float(WindowApp::GetWindowWidth()) / 2.0f - gaugeSizeMax.x / 2, 95.0f });
	sprite[1]->SetSize({ bossHPGaugeSizeMax.x, 20.0f });
	sprite[1]->Update();
}

void BossHpUi::Draw()
{
	for (auto& i : sprite)
	{
		i->Draw();
	}
}
