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
	//�J�����̏�����
	initCameraPos = { mapSizeX / 2.0f,200.0f,mapSizeZ / 2.0f };
	initCameraTarget = { mapSizeX / 2.0f,900.0f,mapSizeZ / 20.0f };

	cameraTarget.pos = initCameraPos;
	cameraTarget.s_pos = initCameraPos;
	cameraTarget.e_pos = initCameraPos;
	cameraPos.pos = initCameraTarget;
	cameraPos.s_pos = initCameraTarget;
	cameraPos.e_pos = initCameraTarget;

	//�V�[��
	scene = SCENE::SET;
	//�J�����̉�]
	initCameraAngle = { -90,50.0f };
	cameraAngle = initCameraAngle;
	//�^�C�}�[
	timer = 0;
	//�J�����̈�ԉ�
	cameraBack = 4000.0f;

	//Json�t�@�C���ǂݍ��݂Ŏg�p����I�u�W�F�N�g���z��
	std::vector<std::string> objectName = {
		"player",
	};

	JsonObjectData* jData = JsonLoder::LoadFile("map2.json", objectName);
	JsonMoveData* jBossMoveData = JsonLoder::LoadMoveFile("map2_boss_move.json");

	//�n�`
	ground = Ground::Create();

	//�v���C���[
	player = Player::Create(jData->objects[objectName[0]][0].pos);

	//�e�}�l�[�W���[
	bullet = BulletManager::Create();

	//�r�[���}�l�[�W���[
	beam = BeamManager::Create();

	//�G�}�l�[�W���[
	enemy = EnemyManager::Create();

	//�{�X
	for (auto& i : jBossMoveData->movePoint) {
		boss->SetMoveList(i.pos, i.moveList);
	}
	boss = BossA::Create(jBossMoveData->movePoint[0].pos, 0);
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

	//------�I�u�W�F�N�g�̍X�V--------//

	player->Update(cameraAngle.x);
	XMFLOAT3 playerPos = player->GetPosition();

	enemy->Update(playerPos);
	boss->Update();
	ui->Update(player->GetHp(), player->GetBulletEnergy(), boss->GetHp());

	//�}�b�v����
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
	//�v���C�Ɉڂ�
	else if (scene == SCENE::SET_FROM_PLAY)
	{
		timer++;
		const float maxTime = 100.0f;
		float ratio = float(timer) / float(maxTime);

		//�J����
		{
			cameraTarget.pos.x = Easing::OutCubic(cameraTarget.s_pos.x, playerPos.x, ratio);
			cameraTarget.pos.y = Easing::OutCubic(cameraTarget.s_pos.y, playerPos.y, ratio);
			cameraTarget.pos.z = Easing::OutCubic(cameraTarget.s_pos.z, playerPos.z, ratio);

			const float range = 20.0f;
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
	//�v���C
	else if (scene == SCENE::PLAY)
	{
		bool playerDamage = false;

		//�e�X�V
		bullet->Update(playerPos);
		if (bullet->CheckEnemyBulletToPlayerCollision())
		{
			playerDamage = true;
		}

		//�r�[���̍X�V
		beam->Update();
		if (beam->CheckBeamToPlayerCollision(playerPos, player->GetScale().x)) {
			playerDamage = true;
		}

		//�G�̍X�V
		enemy->CheckCollision();

		//�{�X�ƃv���C���[�e�̔���
		if (bullet->CheckPlayerBulletToEnemyCollision(boss->GetPosition(), boss->GetScale())) {
			boss->Damage();
		}

		//�Ō�Ƀv���C���[�ւ̃_���[�W
		if (playerDamage) {
			player->Damage();
		}

		bool isTerrainChange = false;
		//���l�ȉ��Ȃ�n�`�ω�
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

		//�J�����X�V
		{
			if (input->PushKey(DIK_LEFT) || xinput->RightStickX(true)) { cameraAngle.x += 3.0f; }
			if (input->PushKey(DIK_RIGHT) || xinput->RightStickX(false)) { cameraAngle.x -= 3.0f; }
			if (input->PushKey(DIK_DOWN) || xinput->RightStickY(false)) {
				cameraAngle.y -= 3.0f;
				//�㏸����
				if (cameraAngle.y < 50.0f) {
					cameraAngle.y = 50.0f;
				}
			}
			if (input->PushKey(DIK_UP) || xinput->RightStickY(true)) {
				cameraAngle.y += 3.0f;
				//���~����
				if (cameraAngle.y > 110.0f) {
					cameraAngle.y = 110.0f;
				}
			}

			//�v���C���[���W
			XMFLOAT3 playerPos = player->GetPosition();
			const float range = 20.0f;
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

		//------�X�V�ȊO�̏���--------//

		//�{�X���|���ꂽ�̂Ńt���A�I���
		if (!boss->GetIsAlive())
		{
			Title* nextScene = new Title();
			nextScene->Initialize();
			SceneManager::SetNextScene(nextScene);
		}

		//�v���C���[��hp��0�ɂȂ����̂ŏI��
		if (!player->GetIsAlive())
		{
			Title* nextScene = new Title();
			nextScene->Initialize();
			SceneManager::SetNextScene(nextScene);
		}
	}
	//�}�b�v�ω��Ɉڂ�
	else if (scene == SCENE::PLAY_FROM_MAP_CHANGE) {
		timer++;
		const float maxTime = 100.0f;

		//�����ʒu�ɖ߂�
		float ratio = float(timer) / float(maxTime);
		player->SetMoviePos(ratio);

		//�J����
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
	//�}�b�v�ω�
	else if (scene == SCENE::MAP_CHANGE) {
		timer++;

		//�}�b�v�̕ύX
		const int maxTime = 50;
		float ratio = float(timer) / float(maxTime);
		ground->SetChangeModel(ratio);
		if (ratio >= 1.0f) {
			timer = 0;
			scene = SCENE::MAP_CHANGE_FROM_PLAY;
			ground->SetChangeModel(ratio, true);
		}
	}
	//�v���C�Ɉڂ�
	else if (scene == SCENE::MAP_CHANGE_FROM_PLAY)
	{
		timer++;
		const int maxTime = 50;
		float ratio = float(timer) / float(maxTime);

		//�J����
		{
			cameraTarget.pos.x = Easing::OutCubic(cameraTarget.s_pos.x, playerPos.x, ratio);
			cameraTarget.pos.y = Easing::OutCubic(cameraTarget.s_pos.y, playerPos.y, ratio);
			cameraTarget.pos.z = Easing::OutCubic(cameraTarget.s_pos.z, playerPos.z, ratio);

			const float range = 20.0f;
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

	//�n�`�X�V
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

	HeightMap::PreDraw();
	ground->Draw();

	Object3d::PreDraw();
	ground->CubeDraw();
	player->Draw();
	enemy->Draw();
	boss->Draw();

	//if (scene == SCENE::SET)
	//{
	//	PrimitiveObject3D::PreDraw();
	//	ground->CDraw();
	//}

	InterfaceObject3d::ReleaseCmdList();

	ParticleManager::PreDraw(cmdList);
	bullet->EffectDraw();
	ParticleManager::PostDraw();
}

void Boss1::DrawNotPostA()
{
	Sprite::PreDraw(cmdList);
	ui->Draw();
	DebugText::GetInstance()->DrawAll();
	Sprite::PostDraw();
}

void Boss1::Finalize()
{
}

void Boss1::ImguiDraw()
{
	//float baseColor[3];//�x�[�X�J���\
	//float metalness;//�����x(0 or 1)
	//float specular;//���ʔ��˓x
	//float roughness;//�e��

	//baseColor[0] = anm->GetBaseColor().x;
	//baseColor[1] = anm->GetBaseColor().y;
	//baseColor[2] = anm->GetBaseColor().z;
	//metalness = anm->GetMetalness();
	//specular = anm->GetSpecular();
	//roughness = anm->GetRoughness();

	////���C�gon/off
	//static bool lightAct1 = false;
	//static bool lightAct2 = false;
	//static bool lightAct3 = false;

	//ImGui::Begin("Material");
	//ImGui::SetWindowPos(ImVec2(0, 0));
	//ImGui::SetWindowSize(ImVec2(300, 130));
	//ImGui::ColorEdit3("baseColor", baseColor, ImGuiColorEditFlags_Float);
	//ImGui::SliderFloat("metalness", &metalness, 0, 1);
	//ImGui::SliderFloat("specular", &specular, 0, 1);
	//ImGui::SliderFloat("roughness", &roughness, 0, 1);
	//ImGui::Checkbox("Light1", &lightAct1);
	//ImGui::Checkbox("Light2", &lightAct2);
	//ImGui::Checkbox("Light3", &lightAct3);
	//ImGui::End();

	//anm->SetBaseColor({ baseColor[0],baseColor[1],baseColor[2] });
	//anm->SetMetalness(metalness);
	//anm->SetSpecular(specular);
	//anm->SetRoughness(roughness);

	//light->SetDirLightActive(0, lightAct1);
	//light->SetDirLightActive(1, lightAct2);
	//light->SetDirLightActive(2, lightAct3);
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
	camera->SetEye(cameraPos.pos);
}
