#include "WindowApp.h"
#include "DirectXCommon.h"
#include "MainEngine.h"
#include "SafeDelete.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �E�B���h�E�T�C�Y
	const int window_width = 1280;// ����
	const int window_height = 720;// �c��

	//�Q�[����
	const wchar_t* gameName = L"�_���}�N";

	//�E�B���h�E������
	WindowApp* winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//DirectXCommon�̃C���X�^���X
	DirectXCommon* dXCommon = DirectXCommon::Create();

	//�S�̂̏�����
	MainEngine* engine = new MainEngine();
	engine->Initialize(dXCommon->GetDevice());

	while (true)
	{
		if (engine->Update()|| winApp->Update()) { break; }

		engine->PostEffectDraw(dXCommon->GetCmdList());

		//�`��O�ݒ�
		dXCommon->PreDraw();

		engine->Draw(dXCommon->GetCmdList());

		//�R�}���h���s
		dXCommon->PostDraw();

		//�t���[�����[�g�Ǘ�
		engine->FrameControl();
	}

	//�o�^����
	safe_delete(engine);
	safe_delete(dXCommon);
	winApp->Release();

	return 0;
}