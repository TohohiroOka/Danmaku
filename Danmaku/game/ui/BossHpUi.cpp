#include "BossHpUi.h"
#include "GameHelper.h"
#include "WindowApp.h"

using namespace DirectX;

BossHpUi::BossHpUi(const int _maxHp)
{ 
	//ウィンドウの幅
	float windowWidth = float(WindowApp::GetWindowWidth());

	//ゲージの最大サイズ
	XMFLOAT2 inSize = spriteName[SPRITE::BOSS_HP_IN].texSize;
	//このゲージの元になる数値の最大値
	hpGauge.maxNum = float(_maxHp);
	//ゲージ最大サイズ
	hpGauge.maxSize = inSize;

	//ゲージの座標
	hpGauge.pos = { windowWidth / 2.0f - inSize.x / 2.0f,30.0f };

	//バー外枠
	float difference = 3.0f;
	sprite[0] = Sprite::Create(spriteName[SPRITE::BOSS_HP_OUT].name);
	sprite[0]->SetPosition(hpGauge.pos);
	sprite[0]->SetTexSize(spriteName[SPRITE::BOSS_HP_OUT].texSize);
	sprite[0]->SetSize(spriteName[SPRITE::BOSS_HP_OUT].texSize);
	sprite[0]->Update();
	//バー中身
	sprite[1] = Sprite::Create(spriteName[SPRITE::BOSS_HP_IN].name);
	sprite[1]->SetPosition({ hpGauge.pos.x + 3,hpGauge.pos.y + 47.0f });
	sprite[1]->SetTexSize(spriteName[SPRITE::BOSS_HP_IN].texSize);
	sprite[1]->SetSize(hpGauge.maxSize);
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
	//現在のhpと最大hpから割合を出す
	float hpRatio = float(_bossHp) / hpGauge.maxNum;

	//大きさ
	XMFLOAT2 nowSize = { hpGauge.maxSize.x * hpRatio, hpGauge.maxSize.y };
	sprite[1]->SetSize(nowSize);

	//更新
	sprite[1]->Update();
}

void BossHpUi::Draw()
{
	for (auto& i : sprite)
	{
		i->Draw();
	}
}
