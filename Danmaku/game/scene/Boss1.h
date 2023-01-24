#pragma once
#include "InterfaceScene.h"
#include <array>

#include "Player.h"
#include "TerrainBox.h"
#include "BulletManager.h"

class Boss1 : public InterfaceScene
{
private:

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
	void DrawNotPostB() override;

	/// <summary>
	///	�`��
	/// </summary>
	void Draw() override;
	
	/// <summary>
	/// �`��
	/// </summary>
	void DrawNotPostA() override;

	/// <summary>
	///	���
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// imgui�̕\��
	/// </summary>
	void ImguiDraw() override;

private:

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
	
	//light�̈ʒu�p�x
	float lightAngle;


	//�n�`
	std::unique_ptr<TerrainBox> terrain;

	//�e
	std::unique_ptr<BulletManager> bullet;

	//�v���C���[
	std::unique_ptr<Player> player;

};