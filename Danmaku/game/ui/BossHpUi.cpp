#include "BossHpUi.h"
#include "GameHelper.h"
#include "WindowApp.h"

using namespace DirectX;

BossHpUi::BossHpUi(const int _maxHp)
{ 
	//�E�B���h�E�̕�
	float windowWidth = float(WindowApp::GetWindowWidth());

	//�Q�[�W�̍ő�T�C�Y
	XMFLOAT2 inSize = spriteName[SPRITE::BOSS_HP_IN].texSize;
	//���̃Q�[�W�̌��ɂȂ鐔�l�̍ő�l
	hpGauge.maxNum = float(_maxHp);
	//�Q�[�W�ő�T�C�Y
	hpGauge.maxSize = inSize;

	//�Q�[�W�̍��W
	hpGauge.pos = { windowWidth / 2.0f - inSize.x / 2.0f,30.0f };

	//�o�[�O�g
	float difference = 3.0f;
	sprite[0] = Sprite::Create(spriteName[SPRITE::BOSS_HP_OUT].name);
	sprite[0]->SetPosition(hpGauge.pos);
	sprite[0]->SetTexSize(spriteName[SPRITE::BOSS_HP_OUT].texSize);
	sprite[0]->SetSize(spriteName[SPRITE::BOSS_HP_OUT].texSize);
	sprite[0]->Update();
	//�o�[���g
	sprite[1] = Sprite::Create(spriteName[SPRITE::BOSS_HP_IN].name);
	sprite[1]->SetPosition({ hpGauge.pos.x + 3,hpGauge.pos.y + 47.0f });
	sprite[1]->SetTexSize(spriteName[SPRITE::BOSS_HP_IN].texSize);
	sprite[1]->SetSize(hpGauge.maxSize);
	sprite[1]->Update();
}

std::unique_ptr<BossHpUi> BossHpUi::Create(const int _maxHp)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossHpUi* instance = new BossHpUi(_maxHp);
	if (instance == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<BossHpUi>(instance);
}

void BossHpUi::Update(const int _bossHp)
{
	//���݂�hp�ƍő�hp���犄�����o��
	float hpRatio = float(_bossHp) / hpGauge.maxNum;

	//�傫��
	XMFLOAT2 nowSize = { hpGauge.maxSize.x * hpRatio, hpGauge.maxSize.y };
	sprite[1]->SetSize(nowSize);

	//�X�V
	sprite[1]->Update();
}

void BossHpUi::Draw()
{
	for (auto& i : sprite)
	{
		i->Draw();
	}
}
