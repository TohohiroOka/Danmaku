#include "UiManager.h"
#include "GameHelper.h"

using namespace DirectX;

std::unique_ptr<UiManager> UiManager::Create(const int _playerMaxHp, const float _bulletEnergyMax, const int _bossMaxHp)
{
	// 3Dオブジェクトのインスタンスを生成
	UiManager* instance = new UiManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize(_playerMaxHp, _bulletEnergyMax,_bossMaxHp);

	return std::unique_ptr<UiManager>(instance);
}

void UiManager::Initialize(const int _playerMaxHp, const float _bulletEnergyMax, const int _bossMaxHp)
{
	BaseUi::StaticInitialize();
	playerUi = PlayerUi::Create(_playerMaxHp, _bulletEnergyMax);
	bossHp = BossHpUi::Create(_bossMaxHp);
}

void UiManager::Update(const int _playerHp, const float _bulletEnergy, const int _bossHp)
{
	playerUi->Update(_playerHp, _bulletEnergy);
	bossHp->Update(_bossHp);
}

void UiManager::Draw()
{
	playerUi->Draw();
	bossHp->Draw();
}