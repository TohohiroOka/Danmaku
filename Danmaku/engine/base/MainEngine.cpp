#include "MainEngine.h"
#include "WindowApp.h"

#include "InstanceObject.h"
#include "DrawLine3D.h"
#include "InterfaceObject3d.h"
#include "Sprite.h"
#include "DebugText.h"
#include "Emitter.h"
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
	mainRenderTarget->Finalize();
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
	GraphicsPipelineManager::StaticInitialize(dXCommon->GetDevice());
	InterfaceObject3d::StaticInitialize(dXCommon->GetDevice());
	Sprite::StaticInitialize(dXCommon->GetDevice());
	DrawLine3D::StaticInitialize(dXCommon->GetDevice());
	ParticleManager::SetDevice(dXCommon->GetDevice());
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	//Fbx::StaticInitialize(dXCommon->GetDevice());
	ComputeShaderManager::StaticInitialize(dXCommon->GetDevice());
	DebugText::GetInstance()->Initialize();
	CubeMap::StaticInitialize(dXCommon->GetDevice());

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

	//縮小バッファ用レンダー
	float size = 256;
	for (int i = 0; i < 5; i++) {
		//縮小バッファ用テクスチャ
		D_shrinkBuffer[i] = std::make_unique<ShrinkBuffer>();
		D_shrinkBuffer[i]->SetPipeline("SHRINK_BUFFER");

		R_shrinkBuffer[i].renderName = "ShrinkBuffer" + std::to_string(size);
		R_shrinkBuffer[i].renderTarget = SubRenderTarget::Create({ size,size }, R_shrinkBuffer[i].renderName);

		D_shrinkBuffer[i]->SetWindowSize(size);
		size /= 4.0f;
	}
	D_shrinkBuffer[0]->SetLuminance(true);


	//ポストエフェクトを合わせる用
	mainDraw = MainPostEffect::Create();
	mainDraw->SetPipeline("POST_EFFECT");
	mainDraw->SetToneMap(true);

	fps = FrameRateKeep::Create();

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
	GraphicsPipelineManager::ResetTopology();

	//描画
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());

	//CubeMap::PreDraw(dXCommon->GetCmdList());
	//cubemap->Draw();
	//CubeMap::PostDraw();

	Sprite::SetCmdList(dXCommon->GetCmdList());
	mainRenderTarget->PreDrawScene();
	scene->Draw(dXCommon->GetCmdList());
	mainRenderTarget->PostDrawScene();

	//ブルーム
	bloom.renderTarget->PreDrawScene(dXCommon->GetCmdList());
	bloomDraw->SetPipeline("BLOOM");
	bloomDraw->Draw({ "NORMAL","BLOOM" });
	bloom.renderTarget->PostDrawScene(dXCommon->GetCmdList());

	//縮小バッファ
	R_shrinkBuffer[0].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[0]->Update();
	D_shrinkBuffer[0]->Draw({bloom.renderName});
	R_shrinkBuffer[0].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//縮小バッファ
	R_shrinkBuffer[1].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[1]->Update();
	D_shrinkBuffer[1]->Draw({ R_shrinkBuffer[0].renderName });
	R_shrinkBuffer[1].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//縮小バッファ
	R_shrinkBuffer[2].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[2]->Update();
	D_shrinkBuffer[2]->Draw({R_shrinkBuffer[1].renderName});
	R_shrinkBuffer[2].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//縮小バッファ
	R_shrinkBuffer[3].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[3]->Update();
	D_shrinkBuffer[3]->Draw({R_shrinkBuffer[2].renderName});
	R_shrinkBuffer[3].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//縮小バッファ
	R_shrinkBuffer[4].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[4]->Update();
	D_shrinkBuffer[4]->Draw({R_shrinkBuffer[3].renderName});
	R_shrinkBuffer[4].renderTarget->PostDrawScene(dXCommon->GetCmdList());

	//描画前設定
	dXCommon->PreDraw();

	mainDraw->Update();
	mainDraw->Draw({ bloom.renderName,"OUTLINE","DEPTH", R_shrinkBuffer[4].renderName });

	//imgui表示
	scene->ImguiDraw();

	//スプライト描画
	scene->DrawSprite(dXCommon->GetCmdList());

	//コマンド実行
	dXCommon->PostDraw();
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