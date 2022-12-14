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

	//�p�b�h
	XInputManager::CreateInstance();

	//�p�C�v���C���n
	GraphicsPipelineManager::CreateInstance();
	GraphicsPipelineManager::GetInstance()->StaticInitialize(_device);;

	//Object�n�̏�����
	InstanceObject::StaticInitialize(_device);
	Texture::StaticInitialize(_device);
	InterfaceObject3d::StaticInitialize(_device);
	Sprite::StaticInitialize(_device);
	ParticleManager::SetDevice(_device);
	LightGroup::StaticInitialize(_device);
	//Fbx::StaticInitialize(_device);
	ComputeShaderManager::StaticInitialize(_device);

	CubeMap::StaticInitialize(_device);

	//�f�o�b�O�e�L�X�g
	DebugText::CreateInstance();
	DebugText::GetInstance()->Initialize();

	//�V�[������
	scene = SceneManager::Create();

	//Windows�T�C�Y
	const XMFLOAT2 windowSize = { float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) };

	//�����_�[�^�[�Q�b�g
	mainRenderTarget = MainRenderTarget::Create();

	//�u���[��
	bloom.renderName = "Bloom";
	bloom.renderTarget = SubRenderTarget::Create(windowSize, bloom.renderName);

	//�u���[���`��
	bloomDraw = std::make_unique<BasePostEffect>();

	//�|�X�g�G�t�F�N�g�����킹��p
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

	//�G�X�P�[�v���~�������ꂽ�Ƃ��Q�[���I��
	if (input->PushKey(DIK_ESCAPE)) { return true; }

	//�X�V
	//Fbx::SetCubeTex(cubemap->SetTexture());
	scene->Update();
	//cubemap->Update();

	return false;
}

void MainEngine::PostEffectDraw(ID3D12GraphicsCommandList* _cmdList)
{
	GraphicsPipelineManager::GetInstance()->ResetTopology();

	//�`��
	DescriptorHeapManager::PreDraw(_cmdList);

	//CubeMap::PreDraw(_cmdList);
	//cubemap->Draw();
	//CubeMap::PostDraw();

	Sprite::SetCmdList(_cmdList);
	mainRenderTarget->PreDrawScene();
	scene->Draw(_cmdList);
	mainRenderTarget->PostDrawScene();

	//�u���[��
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

	//imgui�\��
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

	//�X�v���C�g�`��
	scene->DrawSprite(_cmdList);
}

void MainEngine::FrameControl()
{
	fps->FixedFps();
}

//void MainEngine::debugNum(float x, float y, float z)
//{
//	//�����̃f�o�b�O
//	swprintf_s(str, L"%f,%f,%f\n", x, y, z);
//	OutputDebugString(str);
//}