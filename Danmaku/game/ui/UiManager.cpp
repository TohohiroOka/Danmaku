#include "UiManager.h"
#include "GameHelper.h"

using namespace DirectX;

std::unique_ptr<UiManager> UiManager::Create(const int _playerMaxHp, const int _bossMaxHp)
{
	// 3Dオブジェクトのインスタンスを生成
	UiManager* instance = new UiManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize(_playerMaxHp,_bossMaxHp);

	return std::unique_ptr<UiManager>(instance);
}

void UiManager::Initialize(const int _playerMaxHp, const int _bossMaxHp)
{
	BaseUi::StaticInitialize();
	playerUi = PlayerUi::Create(_playerMaxHp);
	bossHp = BossHpUi::Create(_bossMaxHp);
}

void UiManager::Update(const int _playerHp, const int _bossHp)
{
	playerUi->Update(_playerHp);
	bossHp->Update(_bossHp);
}

void UiManager::Draw()
{
	playerUi->Draw();
	bossHp->Draw();
}