#pragma once
#include "InterfaceScene.h"

#include <array>

//�Q�[���I�u�W�F�N�g�n
#include "Player.h"
#include "Ground.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "BeamManager.h"
#include "BossA.h"
#include "UiManager.h"

class Boss1 : public InterfaceScene
{
private:

	enum class SCENE {
		SET,//����
		SET_FROM_PLAY,//��������Q�[���v���C�܂ł̈ڍs
		PLAY,//�Q�[���v���C
		PLAY_FROM_MAP_CHANGE,//�v���C����}�b�v�̕ύX
		MAP_CHANGE,//�}�b�v�̕ύX
		MAP_CHANGE_FROM_PLAY,//�}�b�v�̕ύX����v���C
		HOME,//�z�[�����
	};

	struct EASING_POS {
		XMFLOAT3 pos = {};
		XMFLOAT3 s_pos = {};
		XMFLOAT3 e_pos = {};
	};

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	///	�X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void CameraUpdate(Camera* camera) override;

	/// <summary>
	/// �`��
	/// </summary>
	void DrawPostEffect() override;

	/// <summary>
	///	�`��
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// �`��
	/// </summary>
	void DrawSprite() override;

	/// <summary>
	///	���
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// imgui�̕\��
	/// </summary>
	void ImguiDraw() override;

private:

	//�V�[��
	SCENE scene;
	//�J�����̍��W
	EASING_POS cameraPos;
	//�J�����̃^�[�Q�b�g
	EASING_POS cameraTarget;
	//�J�����̉�]
	XMFLOAT2 cameraAngle;
	//�����ʒu
	XMFLOAT3 initCameraPos;
	XMFLOAT3 initCameraTarget;
	XMFLOAT2 initCameraAngle;
	//�^�C�}�[
	int timer;
	//�J�����̈�ԉ�
	float cameraBack;

	//�v���C���[
	std::unique_ptr<Player> player;

	//�n�`
	std::unique_ptr<Ground> ground;

	//�e
	std::unique_ptr<BulletManager> bullet;
	
	//�r�[��
	std::unique_ptr<BeamManager> beam;

	//�G
	std::unique_ptr<EnemyManager> enemy;

	//�{�X
	std::unique_ptr<BossA> boss;
	int terrainChangeNum;

	//UI
	std::unique_ptr<UiManager> ui;

};