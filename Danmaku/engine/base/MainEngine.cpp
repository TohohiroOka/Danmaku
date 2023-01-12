#include "MainEngine.h"
//#include "DrawLine.h"
#include "InstanceObject.h"
#include "DrawLine3D.h"
#include "InterfaceObject3d.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Emitter.h"
//#include "Fbx.h"
#include "SafeDelete.h"
#include "ComputeShaderManager.h"
#include "GraphicsPipelineManager.h"
#include "Texture.h"
#include "HeightMap.h"

using namespace DirectX;
using namespace Microsoft::WRL;

MainEngine::~MainEngine()
{
	DebugText::Finalize();
	scene.reset();
	//DrawLine::Finalize();
	Sprite::Finalize();
	//Fbx::Finalize();
	CubeMap::Finalize();
	ParticleManager::Finalize();
	postEffect->Finalize();
	ComputeShaderManager::Finalize();
	DescriptorHeapManager::Finalize();
}

void MainEngine::Initialize()
{
	//directX初期化
	dXCommon = DirectXCommon::Create();

	//key
	input = DirectInput::GetInstance();
	input->Initialize();

	//パッド
	xinput = XInputManager::GetInstance();
	xinput->Initialize();

	//Object系の初期化
	InstanceObject::StaticInitialize(dXCommon->GetDevice());
	Texture::StaticInitialize(dXCommon->GetDevice());
	GraphicsPipelineManager::SetDevice(dXCommon->GetDevice());
	InterfaceObject3d::StaticInitialize(dXCommon->GetDevice());
	Sprite::StaticInitialize(dXCommon->GetDevice());
	DrawLine3D::StaticInitialize(dXCommon->GetDevice());
	ParticleManager::SetDevice(dXCommon->GetDevice());
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	//Fbx::StaticInitialize(dXCommon->GetDevice());
	PostEffect::StaticInitialize();
	ComputeShaderManager::StaticInitialize(dXCommon->GetDevice());
	DebugText::GetInstance()->Initialize();
	CubeMap::StaticInitialize(dXCommon->GetDevice());
	scene = SceneManager::Create();

	postEffect = PostEffect::Create();

	fps = FrameRateKeep::Create();

	postEffect->SetFog(true);

	//cubemap = CubeMap::Create(dXCommon->GetCmdList());
}

bool MainEngine::Update()
{
	input->Update();
	xinput->Update();

	//エスケープか×が押されたときゲーム終了
	if (input->PushKey(DIK_ESCAPE)) { return true; }

	//更新
	//Fbx::SetCubeTex(cubemap->SetTexture());
	scene->Update();
	//cubemap->Update();

	return false;
}

void MainEngine::Draw()
{
	//描画
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());

	postEffect->PreDrawScene(dXCommon->GetCmdList());
	//CubeMap::PreDraw(dXCommon->GetCmdList());
	//cubemap->Draw();
	//CubeMap::PostDraw();

	scene->Draw(dXCommon->GetCmdList());
	postEffect->PostDrawScene(dXCommon->GetCmdList());
	scene->DrawNotPostB(dXCommon->GetCmdList());

	//描画前設定
	dXCommon->PreDraw();

	//imgui表示
	scene->ImguiDraw();

	{
		//XMFLOAT2 toneColor = postEffect->GetToe();
		//XMFLOAT2 linearColor = postEffect->GetLinear();
		//XMFLOAT2 shoulderColor = postEffect->GetShoulder();

		//ImGui::Begin("shader");
		//ImGui::SetWindowPos(ImVec2(0, 0));
		//ImGui::SetWindowSize(ImVec2(300, 130));
		//ImGui::SliderFloat("toe : x", &toneColor.x, 0, 0.4f);
		//ImGui::SliderFloat("toe : y", &toneColor.y, 0, 0.4f);
		//ImGui::SliderFloat("linear : x", &linearColor.x, 0.4f, 5);
		//ImGui::SliderFloat("linear : y", &linearColor.y, 0.4f, 0.8f);
		//ImGui::SliderFloat("shoulder : x", &shoulderColor.x, 5, 20);
		//ImGui::SliderFloat("shoulder : y", &shoulderColor.y, 0.8f, 1);
		//ImGui::End();

		//postEffect->SetToe(toneColor);
		//postEffect->SetLinear(linearColor);
		//postEffect->SetShoulder(shoulderColor);
	}

	//ポストエフェクト描画
	postEffect->Draw(dXCommon->GetCmdList());

	scene->DrawNotPostA(dXCommon->GetCmdList());

	//コマンド実行
	dXCommon->PostDraw();
}

void MainEngine::FrameControl()
{
	fps->FixedFps();
}