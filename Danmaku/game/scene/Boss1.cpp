#include "Boss1.h"
#include "SceneManager.h"
#include "Title.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include "DebugText.h"
#include "Easing.h"
#include "MeshCollider.h"

#include <imgui.h>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <memory>

#include "GameHelper.h"
#include "JsonLoder.h"

using namespace DirectX;

void Boss1::Initialize()
{
	//�J�����̏�����
	initCameraPos = { mapSize / 2.0f,200.0f,mapSize / 2.0f };
	initCameraTarget = { mapSize / 2.0f,900.0f,mapSize / 20.0f };

	cameraTarget.pos = initCameraPos;
	cameraTarget.s_pos = initCameraPos;
	cameraTarget.e_pos = initCameraPos;
	cameraPos.pos = initCameraTarget;
	cameraPos.s_pos = initCameraTarget;
	cameraPos.e_pos = initCameraTarget;

	//�J�����̉�]
	initCameraAngle = { -90,50.0f };
	cameraAngle = initCameraAngle;
	//�^�C�}�[
	timer = 0;
	//�J�����̈�ԉ�
	cameraBack = 4000.0f;

	//light
	lightAngle = 0;

	//�V�[���Z�b�g
	scene = SCENE::SET;

	//�n�`
	terrain = TerrainBox::Create();

	//�e�}�l�[�W���[
	bullet = BulletManager::Create();

	//�v���C���[
	player = Player::Create({ mapSize / 2.0f,200.0f,mapSize / 5.0f }, cameraAngle);
	player->SetMovie();
}

void Boss1::Update()
{
	DirectInput* input = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();

	//------�I�u�W�F�N�g�̍X�V--------//
	//�Z�b�g�V�[��
	if (scene == SCENE::SET)
	{
		player->EndMovie();
		scene = SCENE::PLAY;
	}
	//�v���C��
	else if (scene == SCENE::PLAY) {
		//���C�g�X�V
		{
			lightAngle += 1.0f;
			if (lightAngle > 360) {
				lightAngle = 0;
			}
			float lightRadius = DirectX::XMConvertToRadians(lightAngle);
			light->SetDirLightDir(0, { sinf(lightRadius), cosf(lightRadius), 1.0f, 0 });

			DebugText* text = DebugText::GetInstance();
			std::string strlightAngle = std::to_string(lightAngle);
			text->Print("lightAngle : " + strlightAngle, 100, 100);
			text = nullptr;
		}

		player->Update(cameraAngle);
		XMFLOAT3 playerPos = player->GetPosition();

		//�n�`�X�V
		terrain->Update();

		//�e�X�V
		bullet->Update(playerPos);


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

		//�v���C���[��hp��0�ɂȂ����̂ŏI��
		if (!player->GetIsAlive())
		{
			Title* nextScene = new Title();
			nextScene->Initialize();
			SceneManager::SetNextScene(nextScene);
		}
	}
	input = nullptr;
	xinput = nullptr;
}

void Boss1::DrawNotPostB()
{
}

void Boss1::Draw()
{
	assert(cmdList);

	InterfaceObject3d::SetCmdList(cmdList);

	//HeightMap::PreDraw();
	//terrain->Draw();

	PrimitiveObject3D::PreDraw();
	terrain->HitDraw();

	Object3d::PreDraw();
	player->Draw();

	InterfaceObject3d::ReleaseCmdList();

	ParticleManager::PreDraw(cmdList);
	bullet->EffectDraw();
	ParticleManager::PostDraw();
}

void Boss1::DrawNotPostA()
{
	Sprite::PreDraw(cmdList);
	DebugText::GetInstance()->DrawAll();
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
	camera->SetMatProjection(cameraBack);
	camera->SetTarget(cameraTarget.pos);
	camera->SetEye({cameraPos.pos.x,cameraPos.pos .y,cameraPos.pos .z});
}
