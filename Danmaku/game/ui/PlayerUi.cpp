#include "PlayerUi.h"
#include "GameHelper.h"
#include "WindowApp.h"

using namespace DirectX;

PlayerUi::PlayerUi(const int _maxHp)
{
	//�E�B���h�E�̕�
	float windowWidth = float(WindowApp::GetWindowWidth());
	float windowHeight = float(WindowApp::GetWindowHeight());

	//�Q�[�W�̍ő�T�C�Y
	XMFLOAT2 inSize = spriteName[SPRITE::PLAYER_HP_IN].texSize;
	//���̃Q�[�W�̌��ɂȂ鐔�l�̍ő�l
	hpGauge.maxNum = float(_maxHp);
	//�Q�[�W�ő�T�C�Y
	hpGauge.maxSize = { 300.0f,30.0f };

	//�Q�[�W�̍��W
	hpGauge.pos = { windowWidth / 15.0f,windowHeight * 0.9f };

	//�o�[�O�g
	hp[0] = Sprite::Create(spriteName[SPRITE::PLAYER_HP_OUT].name);
	hp[0]->SetPosition(hpGauge.pos);
	hp[0]->SetTexSize(spriteName[SPRITE::PLAYER_HP_OUT].texSize);
	hp[0]->SetSize({ hpGauge.maxSize.x, hpGauge.maxSize.y + 1.0f });
	hp[0]->Update();
	//�o�[���g
	hp[1] = Sprite::Create(spriteName[SPRITE::PLAYER_HP_IN].name);
	hp[1]->SetPosition(hpGauge.pos);
	hp[1]->SetTexSize(spriteName[SPRITE::PLAYER_HP_IN].texSize);
	hp[1]->SetSize(hpGauge.maxSize);
	hp[1]->Update();

	//�n�[�g�p�l��
	heartPanel = Sprite::Create(spriteName[SPRITE::PLAYER_HP_PANEL].name);
	heartPanel->SetPosition({ hpGauge.pos.x - 60.0f, hpGauge.pos.y - 10.0f });
	heartPanel->SetTexSize(spriteName[SPRITE::PLAYER_HP_PANEL].texSize);
	heartPanel->SetSize({ 50.0f,50.0f });
	heartPanel->Update();
}

std::unique_ptr<PlayerUi> PlayerUi::Create(const int _maxHp)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PlayerUi* instance = new PlayerUi(_maxHp);
	if (instance == nullptr) {
		return nullptr;
	}

	return std::unique_ptr<PlayerUi>(instance);
}

void PlayerUi::Update(const int _playerHp)
{
	//���݂�hp�ƍő�hp���犄�����o��
	float hpRatio = float(_playerHp) / hpGauge.maxNum;

	if (_playerHp < 0) {
		hpRatio = 0;
	}

	//�傫��
	XMFLOAT2 nowSize = { hpGauge.maxSize.x * hpRatio, hpGauge.maxSize.y };
	hp[1]->SetSize(nowSize);

	//�X�V
	hp[1]->Update();
}

void PlayerUi::Draw()
{
	for (auto& i : hp)
	{
		i->Draw();
	}
	heartPanel->Draw();
}
