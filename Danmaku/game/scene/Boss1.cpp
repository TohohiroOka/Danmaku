#include "Boss1.h"
#include "SceneManager.h"
#include "Title.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include "DebugText.h"
#include "Easing.h"

#include <imgui.h>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <memory>

#include "GameCollision.h"
#include "GameHelper.h"
#include "JsonLoder.h"

using namespace DirectX;

void Boss1::Initialize()
{
	//カメラの初期化
	initCameraPos = { mapSizeX / 2.0f,200.0f,mapSizeZ / 2.0f };
	initCameraTarget = { mapSizeX / 2.0f,900.0f,mapSizeZ / 20.0f };

	cameraTarget.pos = initCameraPos;
	cameraTarget.s_pos = initCameraPos;
	cameraTarget.e_pos = initCameraPos;
	cameraPos.pos = initCameraTarget;
	cameraPos.s_pos = initCameraTarget;
	cameraPos.e_pos = initCameraTarget;

	//シーン
	scene = SCENE::SET;
	//カメラの回転
	initCameraAngle = { -90,50.0f };
	cameraAngle = initCameraAngle;
	//タイマー
	timer = 0;
	//カメラの一番奥
	cameraBack = 4000.0f;

	//Jsonファイル読み込みで使用するオブジェクト名配列
	std::vector<std::string> objectName = {
		"player",
	};

	JsonObjectData* jData = JsonLoder::LoadFile("map2.json", objectName);

	//地形
	ground = Ground::Create();

	//プレイヤー
	player = Player::Create(jData->objects[objectName[0]][0].pos, cameraAngle);

	//弾マネージャー
	bullet = BulletManager::Create();

	//ビームマネージャー
	beam = BeamManager::Create();

	//敵マネージャー
	enemy = EnemyManager::Create();

	//ボス
	boss = BossA::Create();
	terrainChangeNum = 0;

	//UI
	ui = UiManager::Create(player->GetMaxHp(), player->GetBulletEnergyMax(), boss->GetMaxHp());

	player->SetMovie();
	boss->SetMovie();
}

void Boss1::Update()
{
	DirectInput* input = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();

	//------オブジェクトの更新--------//

	player->Update(cameraAngle);
	XMFLOAT3 playerPos = player->GetPosition();

	enemy->Update(playerPos);
	boss->Update();
	ui->Update(player->GetHp(), player->GetBulletEnergy(), boss->GetHp());

	//マップ生成
	if (scene == SCENE::SET)
	{
		timer++;
		const int maxTime = 100;
		float ratio = float(timer) / float(maxTime);
		ground->SetChangeModel(ratio);

		if (ratio >= 1.0f) {
			ground->SetChangeModel(1.0f, true);
			scene = SCENE::SET_FROM_PLAY;
			timer = 0;
		}
	}
	//プレイに移る
	else if (scene == SCENE::SET_FROM_PLAY)
	{
		timer++;
		const float maxTime = 100.0f;
		float ratio = float(timer) / float(maxTime);

		//カメラ
		{
			cameraTarget.pos.x = Easing::OutCubic(cameraTarget.s_pos.x, playerPos.x, ratio);
			cameraTarget.pos.y = Easing::OutCubic(cameraTarget.s_pos.y, playerPos.y, ratio);
			cameraTarget.pos.z = Easing::OutCubic(cameraTarget.s_pos.z, playerPos.z, ratio);

			const float range = 40.0f;
			XMFLOAT2 cameraRadius = {
				DirectX::XMConvertToRadians(cameraAngle.x),
				DirectX::XMConvertToRadians(cameraAngle.y)
			};
			XMFLOAT3 afterEye = {
				cosf(cameraRadius.x) * range + playerPos.x,
				cosf(cameraRadius.y) * range + playerPos.y,
				sinf(cameraRadius.x) * range + playerPos.z };

			cameraPos.pos.x = Easing::OutCubic(cameraPos.s_pos.x, afterEye.x, ratio);
			cameraPos.pos.y = Easing::OutCubic(cameraPos.s_pos.y, afterEye.y, ratio);
			cameraPos.pos.z = Easing::OutCubic(cameraPos.s_pos.z, afterEye.z, ratio);

			float afterCameraBack = 3000.0f;
			float initCameraBack = 4000.0f;
			cameraBack = Easing::OutCubic(initCameraBack, afterCameraBack, ratio);
		}

		if (timer >= maxTime) {
			timer = 0;
			scene = SCENE::PLAY;
			player->EndMovie();
			boss->EndMovie();
		}
	}
	//プレイ
	else if (scene == SCENE::PLAY)
	{
		bool playerDamage = false;

		//弾更新
		bullet->Update(playerPos);
		if (bullet->CheckEnemyBulletToPlayerCollision())
		{
			playerDamage = true;
		}

		//ビームの更新
		beam->Update();
		if (beam->CheckBeamToPlayerCollision(playerPos, player->GetScale().x)) {
			playerDamage = true;
		}

		//敵の更新
		enemy->CheckCollision();

		//ボスとプレイヤー弾の判定
		bool bossDamage = false;
		if (bullet->CheckPlayerBulletToEnemyCollision(boss->GetPosition(), boss->GetScale())) {
			bossDamage = true;
		} 

		for(int i=0;i<8;i++){
			if (bullet->CheckPlayerBulletToEnemyCollision(boss->GetPartsPos(i), boss->GetPartsScale(i))) {
				bossDamage = true;
			}
		}

		if (bossDamage) {
			boss->Damage();
		}

		//最後にプレイヤーへのダメージ
		if (playerDamage) {
			player->Damage();
		}

		bool isTerrainChange = false;
		//一定値以下なら地形変化
		if ((boss->GetHpRatio() < 0.8f && terrainChangeNum == 0) ||
			(boss->GetHpRatio() < 0.6f && terrainChangeNum == 1) ||
			(boss->GetHpRatio() < 0.4f && terrainChangeNum == 2) ||
			(boss->GetHpRatio() < 0.2f && terrainChangeNum == 3)) {
			isTerrainChange = true;
			terrainChangeNum++;
		}

		if (isTerrainChange) {
			scene = SCENE::PLAY_FROM_MAP_CHANGE;
			player->SetMovie();
			boss->SetMovie();
			bullet->Reset();
			beam->Reset();
			cameraTarget.s_pos = cameraTarget.pos;
			cameraPos.s_pos = cameraPos.pos;
			cameraTarget.s_pos = initCameraPos;
			cameraPos.s_pos = initCameraTarget;
			cameraAngle = initCameraAngle;
		}

		//カメラ更新
		{
			if (input->PushKey(DIK_LEFT) || xinput->RightStickX(true)) { cameraAngle.x += 3.0f; }
			if (input->PushKey(DIK_RIGHT) || xinput->RightStickX(false)) { cameraAngle.x -= 3.0f; }
			if (input->PushKey(DIK_DOWN) || xinput->RightStickY(false)) {
				cameraAngle.y -= 3.0f;
				//上昇制限
				if (cameraAngle.y < 50.0f) {
					cameraAngle.y = 50.0f;
				}
			}
			if (input->PushKey(DIK_UP) || xinput->RightStickY(true)) {
				cameraAngle.y += 3.0f;
				//下降制限
				if (cameraAngle.y > 110.0f) {
					cameraAngle.y = 110.0f;
				}
			}

			//プレイヤー座標
			XMFLOAT3 playerPos = player->GetPosition();
			const float range = 40.0f;
			XMFLOAT2 cameraRadius = {
				DirectX::XMConvertToRadians(cameraAngle.x),
				DirectX::XMConvertToRadians(cameraAngle.y)
			};
			cameraTarget.pos = playerPos;

			cameraPos.pos = {
				cosf(cameraRadius.x) * range + playerPos.x,
				cosf(cameraRadius.y) * range + playerPos.y,
				sinf(cameraRadius.x) * range + playerPos.z };

			cameraAngle.x = float(int(cameraAngle.x) % 360);
		}

		//------更新以外の処理--------//

		//ボスが倒されたのでフロア終わり
		if (!boss->GetIsAlive())
		{
			Title* nextScene = new Title();
			nextScene->Initialize();
			SceneManager::SetNextScene(nextScene);
		}

		//プレイヤーのhpが0になったので終了
		if (!player->GetIsAlive())
		{
			Title* nextScene = new Title();
			nextScene->Initialize();
			SceneManager::SetNextScene(nextScene);
		}
	}
	//マップ変化に移る
	else if (scene == SCENE::PLAY_FROM_MAP_CHANGE) {
		timer++;
		const float maxTime = 100.0f;

		//初期位置に戻す
		float ratio = float(timer) / float(maxTime);
		player->SetMoviePos(ratio);

		//カメラ
		{
			cameraTarget.pos.x = Easing::OutCubic(cameraTarget.s_pos.x, cameraTarget.e_pos.x, ratio);
			cameraTarget.pos.y = Easing::OutCubic(cameraTarget.s_pos.y, cameraTarget.e_pos.y, ratio);
			cameraTarget.pos.z = Easing::OutCubic(cameraTarget.s_pos.z, cameraTarget.e_pos.z, ratio);

			cameraPos.pos.x = Easing::OutCubic(cameraPos.s_pos.x, cameraPos.e_pos.x, ratio);
			cameraPos.pos.y = Easing::OutCubic(cameraPos.s_pos.y, cameraPos.e_pos.y, ratio);
			cameraPos.pos.z = Easing::OutCubic(cameraPos.s_pos.z, cameraPos.e_pos.z, ratio);
		}

		if (ratio >= 1.0f) {
			timer = 0;
			scene = SCENE::MAP_CHANGE;
			cameraTarget.s_pos = cameraTarget.pos;
			cameraPos.s_pos = cameraPos.pos;
		}
	}
	//マップ変化
	else if (scene == SCENE::MAP_CHANGE) {
		timer++;

		//マップの変更
		const int maxTime = 50;
		float ratio = float(timer) / float(maxTime);
		ground->SetChangeModel(ratio);
		if (ratio >= 1.0f) {
			timer = 0;
			scene = SCENE::MAP_CHANGE_FROM_PLAY;
			ground->SetChangeModel(ratio, true);
		}
	}
	//プレイに移る
	else if (scene == SCENE::MAP_CHANGE_FROM_PLAY)
	{
		timer++;
		const int maxTime = 50;
		float ratio = float(timer) / float(maxTime);

		//カメラ
		{
			cameraTarget.pos.x = Easing::OutCubic(cameraTarget.s_pos.x, playerPos.x, ratio);
			cameraTarget.pos.y = Easing::OutCubic(cameraTarget.s_pos.y, playerPos.y, ratio);
			cameraTarget.pos.z = Easing::OutCubic(cameraTarget.s_pos.z, playerPos.z, ratio);

			const float range = 40.0f;
			XMFLOAT2 cameraRadius = {
				DirectX::XMConvertToRadians(cameraAngle.x),
				DirectX::XMConvertToRadians(cameraAngle.y)
			};
			XMFLOAT3 afterEye = {
				cosf(cameraRadius.x) * range + playerPos.x,
				cosf(cameraRadius.y) * range + playerPos.y,
				sinf(cameraRadius.x) * range + playerPos.z };

			cameraPos.pos.x = Easing::OutCubic(cameraPos.s_pos.x, afterEye.x, ratio);
			cameraPos.pos.y = Easing::OutCubic(cameraPos.s_pos.y, afterEye.y, ratio);
			cameraPos.pos.z = Easing::OutCubic(cameraPos.s_pos.z, afterEye.z, ratio);
		}

		if (timer >= maxTime) {
			timer = 0;
			scene = SCENE::PLAY;
			player->EndMovie();
			boss->EndMovie();
		}
	}

	//地形更新
	ground->Update();

	input = nullptr;
	xinput = nullptr;
}

void Boss1::DrawNotPostB()
{
	InstanceObject::PreDraw(cmdList);
	bullet->Draw();
	beam->Draw();
	InstanceObject::PostDraw();
}

void Boss1::Draw()
{
	assert(cmdList);

	InterfaceObject3d::SetCmdList(cmdList);

	PrimitiveObject3D::PreDraw();
	//ground->CDraw();
	HeightMap::PreDraw();
	ground->Draw();

	Object3d::PreDraw();
	ground->CubeDraw();
	player->Draw();
	enemy->Draw();
	boss->Draw();

	InterfaceObject3d::ReleaseCmdList();

	ParticleManager::PreDraw(cmdList);
	bullet->EffectDraw();
	ParticleManager::PostDraw();
}

void Boss1::DrawNotPostA()
{
	Sprite::PreDraw(cmdList);
	ui->Draw();
	//DebugText::GetInstance()->DrawAll();
	Sprite::PostDraw();
}

void Boss1::Finalize()
{
}

void Boss1::ImguiDraw()
{
}

void Boss1::CameraUpdate(Camera* camera)
{
	DirectInput* input = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();

	if (scene == SCENE::SET)
	{
		camera->SetMatProjection(cameraBack);
	}
	else if (scene == SCENE::SET_FROM_PLAY)
	{
		camera->SetMatProjection(cameraBack);
	}

	camera->SetTarget(cameraTarget.pos);
	camera->SetEye({cameraPos.pos.x,cameraPos.pos .y,cameraPos.pos .z});
}
