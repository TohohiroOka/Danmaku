#include "MainEngine.h"
#include "WindowApp.h"
#include "DirectInput.h"
#include "XInputManager.h"

#include "InstanceObject.h"
#include "InterfaceObject3d.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Emitter.h"
#include "SafeDelete.h"
#include "ComputeShaderManager.h"
#include "GraphicsPipelineManager.h"
#include "Texture.h"
#include "HeightMap.h"

#include <imgui.h>


using namespace DirectX;
using namespace Microsoft::WRL;

MainEngine::~MainEngine()
{
	mainRenderTarget.reset();
	bloom.renderTarget.reset();
	bloomDraw.reset();
	mainDraw.reset();
	fps.reset();

	DirectInput::Destroy();
	XInputManager::Destroy();
	DebugText::Destroy();
	scene.reset();
	Sprite::Finalize();
	//Fbx::Finalize();
	CubeMap::Finalize();
	ParticleManager::Finalize();
	ComputeShaderManager::Finalize();
	DescriptorHeapManager::Finalize();
	GraphicsPipelineManager::Destroy();
}

void MainEngine::Initialize(ID3D12Device* _device)
{
	//key
	DirectInput::CreateInstance();
	DirectInput::GetInstance()->Initialize();

	//パッド
	XInputManager::CreateInstance();

	//パイプライン系
	GraphicsPipelineManager::CreateInstance();
	GraphicsPipelineManager::GetInstance()->StaticInitialize(_device);;

	//Object系の初期化
	InstanceObject::StaticInitialize(_device);
	Texture::StaticInitialize(_device);
	InterfaceObject3d::StaticInitialize(_device);
	Sprite::StaticInitialize(_device);
	ParticleManager::SetDevice(_device);
	LightGroup::StaticInitialize(_device);
	//Fbx::StaticInitialize(_device);
	ComputeShaderManager::StaticInitialize(_device);

	CubeMap::StaticInitialize(_device);

	//デバッグテキスト
	DebugText::CreateInstance();
	DebugText::GetInstance()->Initialize();

	//シーン生成
	scene = SceneManager::Create();

	//Windowsサイズ
	const XMFLOAT2 windowSize = { float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) };

	//レンダーターゲット
	mainRenderTarget = MainRenderTarget::Create();

	//ブルーム
	bloom.renderName = "Bloom";
	bloom.renderTarget = SubRenderTarget::Create(windowSize, bloom.renderName);

	//ブルーム描画
	bloomDraw = std::make_unique<BasePostEffect>();

	//ポストエフェクトを合わせる用
	mainDraw = MainPostEffect::Create();
	mainDraw->SetPipeline("POST_EFFECT");
	mainDraw->SetToneMap(true);
	mainDraw->SetFog(false);

	fps = FrameRateKeep::Create();

	//cubemap = CubeMap::Create(dXCommon->GetCmdList());
}

bool MainEngine::Update()
{
	DirectInput* input = DirectInput::GetInstance();
	XInputManager* xinput = XInputManager::GetInstance();

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

void MainEngine::PostEffectDraw(ID3D12GraphicsCommandList* _cmdList)
{
	GraphicsPipelineManager::GetInstance()->ResetTopology();

	//描画
	DescriptorHeapManager::PreDraw(_cmdList);

	//CubeMap::PreDraw(_cmdList);
	//cubemap->Draw();
	//CubeMap::PostDraw();

	Sprite::SetCmdList(_cmdList);
	mainRenderTarget->PreDrawScene();
	scene->Draw(_cmdList);
	mainRenderTarget->PostDrawScene();

	//ブルーム
	bloom.renderTarget->PreDrawScene(_cmdList);
	bloomDraw->SetPipeline("BLOOM");
	bloomDraw->Draw({ "NORMAL","BLOOM" });
	bloom.renderTarget->PostDrawScene(_cmdList);
}

void MainEngine::Draw(ID3D12GraphicsCommandList* _cmdList)
{
	DescriptorHeapManager::PreDraw(_cmdList);

	mainDraw->Update();
	mainDraw->Draw({ bloom.renderName,"OUTLINE","DEPTH" });

	//imgui表示
	scene->ImguiDraw();

	XMFLOAT2 toneColor = mainDraw->GetToeColor();
	XMFLOAT2 linearColor = mainDraw->GetLinearColor();

	ImGui::Begin("shader");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 130));
	ImGui::SliderFloat("toe : x", &toneColor.x, 0, 5);
	ImGui::SliderFloat("toe : y", &toneColor.y, 0, 5);
	ImGui::SliderFloat("linear : x", &linearColor.x, 0, 5);
	ImGui::SliderFloat("linear : y", &linearColor.y, 0, 5);
	ImGui::End();

	mainDraw->SetToeColor(toneColor);
	mainDraw->SetLinearColor(linearColor);

	//スプライト描画
	scene->DrawSprite(_cmdList);
}

void MainEngine::FrameControl()
{
	fps->FixedFps();
}

//void MainEngine::debugNum(float x, float y, float z)
//{
//	//数字のデバッグ
//	swprintf_s(str, L"%f,%f,%f\n", x, y, z);
//	OutputDebugString(str);
//}