#include "PlayerUi.h"
#include "GameHelper.h"
#include "WindowApp.h"

using namespace DirectX;

PlayerUi::PlayerUi(const int _maxHp, const float _bulletEnergyMax)
{
	//�E�B���h�E�̕�
	float windowWidth = float(WindowApp::GetWindowWidth());
	float windowHeight = float(WindowApp::GetWindowHeight());

	//hp�Q�[�W
	{
		//�Q�[�W�̍ő�T�C�Y
		XMFLOAT2 inSize = spriteName[SPRITE::PLAYER_HP_IN].texSize;
		//���̃Q�[�W�̌��ɂȂ鐔�l�̍ő�l
		hpGauge.maxNum = float(_maxHp);
		//�Q�[�W�ő�T�C�Y
		hpGauge.maxSize = { 300.0f,30.0f };

		//�Q�[�W�̍��W
		hpGauge.pos = { windowWidth / 15.0f,windowHeight / 10.0f * 8.0f };

		//�o�[�O�g
		hpGauge.gaugeTex[0] = Sprite::Create(spriteName[SPRITE::PLAYER_HP_OUT].name);
		hpGauge.gaugeTex[0]->SetPosition(hpGauge.pos);
		hpGauge.gaugeTex[0]->SetTexSize(spriteName[SPRITE::PLAYER_HP_OUT].texSize);
		hpGauge.gaugeTex[0]->SetSize({ hpGauge.maxSize.x, hpGauge.maxSize.y + 1.0f });
		hpGauge.gaugeTex[0]->Update();
		//�o�[���g
		hpGauge.gaugeTex[1] = Sprite::Create(spriteName[SPRITE::PLAYER_HP_IN].name);
		hpGauge.gaugeTex[1]->SetPosition(hpGauge.pos);
		hpGauge.gaugeTex[1]->SetTexSize(spriteName[SPRITE::PLAYER_HP_IN].texSize);
		hpGauge.gaugeTex[1]->SetSize(hpGauge.maxSize);
		hpGauge.gaugeTex[1]->Update();

		//�n�[�g�p�l��
		hpGauge.panelTex = Sprite::Create(spriteName[SPRITE::PLAYER_HP_PANEL].name);
		hpGauge.panelTex->SetPosition({ hpGauge.pos.x - 60.0f, hpGauge.pos.y - 10.0f });
		hpGauge.panelTex->SetTexSize(spriteName[SPRITE::PLAYER_HP_PANEL].texSize);
		hpGauge.panelTex->SetSize({ 40.0f,40.0f });
		hpGauge.panelTex->Update();
	}
	//energy�Q�[�W
	{
		//�Q�[�W�̍ő�T�C�Y
		XMFLOAT2 inSize = spriteName[SPRITE::PLAYER_HP_IN].texSize;
		//���̃Q�[�W�̌��ɂȂ鐔�l�̍ő�l
		energyGauge.maxNum = float(_bulletEnergyMax);
		//�Q�[�W�ő�T�C�Y
		energyGauge.maxSize = { 300.0f,30.0f };

		//�Q�[�W�̍��W
		energyGauge.pos = { windowWidth / 15.0f,windowHeight / 10.0f * 9.0f };

		//�o�[�O�g
		energyGauge.gaugeTex[0] = Sprite::Create(spriteName[SPRITE::PLAYER_HP_OUT].name);
		energyGauge.gaugeTex[0]->SetPosition(energyGauge.pos);
		energyGauge.gaugeTex[0]->SetTexSize(spriteName[SPRITE::PLAYER_HP_OUT].texSize);
		energyGauge.gaugeTex[0]->SetSize({ energyGauge.maxSize.x, energyGauge.maxSize.y + 1.0f });
		energyGauge.gaugeTex[0]->Update();
		//�o�[���g
		energyGauge.gaugeTex[1] = Sprite::Create(spriteName[SPRITE::PLAYER_HP_IN].name);
		energyGauge.gaugeTex[1]->SetPosition(energyGauge.pos);
		energyGauge.gaugeTex[1]->SetTexSize(spriteName[SPRITE::PLAYER_HP_IN].texSize);
		energyGauge.gaugeTex[1]->SetSize(energyGauge.maxSize);
		energyGauge.gaugeTex[1]->Update();

		//�n�[�g�p�l��
		energyGauge.panelTex = Sprite::Create(spriteName[SPRITE::PLAYER_BULLET_ENERGY].name);
		energyGauge.panelTex->SetPosition({ energyGauge.pos.x - 60.0f, energyGauge.pos.y - 10.0f });
		energyGauge.panelTex->SetTexSize(spriteName[SPRITE::PLAYER_BULLET_ENERGY].texSize);
		energyGauge.panelTex->SetSize({ 40.0f,40.0f });
		energyGauge.panelTex->Update();
	}

}

std::unique_ptr<PlayerUi> PlayerUi::Create(const int _maxHp, const float _bulletEnergyMax)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PlayerUi* instance = new PlayerUi(_maxHp, _bulletEnergyMax);
	if (instance == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<PlayerUi>(instance);
}

void PlayerUi::Update(const int _playerHp, const float _bulletEnergy)
{
	const float errorX = 10.0f;
	{
		//���݂�hp�ƍő�hp���犄�����o��
		float ratio = float(_playerHp) / hpGauge.maxNum;

		//�傫��
		XMFLOAT2 nowSize = { hpGauge.maxSize.x * ratio, hpGauge.maxSize.y };
		hpGauge.gaugeTex[1]->SetSize(nowSize);

		//���W
		XMFLOAT2 nowPos = { hpGauge.pos.x + errorX * (1.0f - ratio),hpGauge.pos.y };
		hpGauge.gaugeTex[1]->SetPosition(nowPos);

		//�X�V
		hpGauge.gaugeTex[1]->Update();
	}

	{
		//���݂�hp�ƍő�hp���犄�����o��
		float ratio = float(_bulletEnergy) / energyGauge.maxNum;

		//�傫��
		XMFLOAT2 nowSize = { energyGauge.maxSize.x * ratio, energyGauge.maxSize.y };
		energyGauge.gaugeTex[1]->SetSize(nowSize);

		//���W
		XMFLOAT2 nowPos = { energyGauge.pos.x + errorX * (1.0f - ratio),energyGauge.pos.y };
		energyGauge.gaugeTex[1]->SetPosition(nowPos);

		//�X�V
		energyGauge.gaugeTex[1]->Update();
	}
}

void PlayerUi::Draw()
{
	for (auto& i : hpGauge.gaugeTex)
	{
		i->Draw();
	}
	hpGauge.panelTex->Draw();

	for (auto& i : energyGauge.gaugeTex)
	{
		i->Draw();
	}
	energyGauge.panelTex->Draw();

}
