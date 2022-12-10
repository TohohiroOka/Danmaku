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
	//directX������
	dXCommon = DirectXCommon::Create();

	//key
	input = DirectInput::GetInstance();
	input->Initialize();

	//�p�b�h
	xinput = XInputManager::GetInstance();
	xinput->Initialize();

	//Object�n�̏�����
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

	//Windows�T�C�Y
	const XMFLOAT2 windowSize = { float(WindowApp::GetWindowWidth()),float(WindowApp::GetWindowHeight()) };

	//�����_�[�^�[�Q�b�g
	mainRenderTarget = MainRenderTarget::Create();

	//�u���[��
	bloom.renderName = "Bloom";
	bloom.renderTarget = SubRenderTarget::Create(windowSize, bloom.renderName);

	//�u���[���`��
	bloomDraw = std::make_unique<BasePostEffect>();

	//�k���o�b�t�@�p�����_�[
	float size = 256;
	for (int i = 0; i < 5; i++) {
		//�k���o�b�t�@�p�e�N�X�`��
		D_shrinkBuffer[i] = std::make_unique<ShrinkBuffer>();
		D_shrinkBuffer[i]->SetPipeline("SHRINK_BUFFER");

		R_shrinkBuffer[i].renderName = "ShrinkBuffer" + std::to_string(size);
		R_shrinkBuffer[i].renderTarget = SubRenderTarget::Create({ size,size }, R_shrinkBuffer[i].renderName);

		D_shrinkBuffer[i]->SetWindowSize(size);
		size /= 4.0f;
	}
	D_shrinkBuffer[0]->SetLuminance(true);


	//�|�X�g�G�t�F�N�g�����킹��p
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

	//�G�X�P�[�v���~�������ꂽ�Ƃ��Q�[���I��
	if (input->PushKey(DIK_ESCAPE)) { return true; }

	//�X�V
	//Fbx::SetCubeTex(cubemap->SetTexture());
	scene->Update();
	//cubemap->Update();

	return false;
}

void MainEngine::Draw()
{
	GraphicsPipelineManager::ResetTopology();

	//�`��
	DescriptorHeapManager::PreDraw(dXCommon->GetCmdList());

	//CubeMap::PreDraw(dXCommon->GetCmdList());
	//cubemap->Draw();
	//CubeMap::PostDraw();

	Sprite::SetCmdList(dXCommon->GetCmdList());
	mainRenderTarget->PreDrawScene();
	scene->Draw(dXCommon->GetCmdList());
	mainRenderTarget->PostDrawScene();

	//�u���[��
	bloom.renderTarget->PreDrawScene(dXCommon->GetCmdList());
	bloomDraw->SetPipeline("BLOOM");
	bloomDraw->Draw({ "NORMAL","BLOOM" });
	bloom.renderTarget->PostDrawScene(dXCommon->GetCmdList());

	//�k���o�b�t�@
	R_shrinkBuffer[0].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[0]->Update();
	D_shrinkBuffer[0]->Draw({bloom.renderName});
	R_shrinkBuffer[0].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//�k���o�b�t�@
	R_shrinkBuffer[1].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[1]->Update();
	D_shrinkBuffer[1]->Draw({ R_shrinkBuffer[0].renderName });
	R_shrinkBuffer[1].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//�k���o�b�t�@
	R_shrinkBuffer[2].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[2]->Update();
	D_shrinkBuffer[2]->Draw({R_shrinkBuffer[1].renderName});
	R_shrinkBuffer[2].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//�k���o�b�t�@
	R_shrinkBuffer[3].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[3]->Update();
	D_shrinkBuffer[3]->Draw({R_shrinkBuffer[2].renderName});
	R_shrinkBuffer[3].renderTarget->PostDrawScene(dXCommon->GetCmdList());
	//�k���o�b�t�@
	R_shrinkBuffer[4].renderTarget->PreDrawScene(dXCommon->GetCmdList());
	D_shrinkBuffer[4]->Update();
	D_shrinkBuffer[4]->Draw({R_shrinkBuffer[3].renderName});
	R_shrinkBuffer[4].renderTarget->PostDrawScene(dXCommon->GetCmdList());

	//�`��O�ݒ�
	dXCommon->PreDraw();

	mainDraw->Update();
	mainDraw->Draw({ bloom.renderName,"OUTLINE","DEPTH", R_shrinkBuffer[4].renderName });

	//imgui�\��
	scene->ImguiDraw();

	//�X�v���C�g�`��
	scene->DrawSprite(dXCommon->GetCmdList());

	//�R�}���h���s
	dXCommon->PostDraw();
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