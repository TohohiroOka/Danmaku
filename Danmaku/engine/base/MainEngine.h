#pragma once
#include "SceneManager.h"
#include "FrameRateKeep.h"
#include "CubeMap.h"

//�|�X�g�G�t�F�N�g�n
#include "MainRenderTarget.h"
#include "SubRenderTarget.h"
#include "MainPostEffect.h"

#include <sstream>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class WindowApp;

class MainEngine
{
private:

	struct RENDER_INFO {
		//�������ރ����_�[�^�[�Q�b�g�̖��O
		std::string renderName;
		//�������ރ����_�[�^�[�Q�b�g
		std::unique_ptr<SubRenderTarget> renderTarget;
	};

public:

	MainEngine() = default;
	~MainEngine();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize(ID3D12Device* _device);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <returns>�Q�[���𑱂��邩</returns>
	bool Update();

	/// <summary>
	/// �`��
	/// </summary>
	void PostEffectDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// �t���[�����[�g�L�[�v
	/// </summary>
	void FrameControl();

private:

	//GameScene�̃C���X�^���X
	std::unique_ptr<SceneManager> scene = nullptr;
	//Fps�Œ�p�N���X�̃C���X�^���X
	std::unique_ptr<FrameRateKeep> fps = nullptr;
	//�L���[�u�}�b�v
	//std::unique_ptr<CubeMap> cubemap;
	//�����\���f�o�b�O�p
	wchar_t str[256] = {};

	///�|�X�g�G�t�F�N�g�n
	//�����_�[�^�[�Q�b�g���擾����p
	std::unique_ptr<MainRenderTarget> mainRenderTarget;
	//�u���[��
	RENDER_INFO bloom;
	//�u���[���`��
	std::unique_ptr<BasePostEffect> bloomDraw;
	//�|�X�g�G�t�F�N�g�����킹��p
	std::unique_ptr<MainPostEffect> mainDraw;

};