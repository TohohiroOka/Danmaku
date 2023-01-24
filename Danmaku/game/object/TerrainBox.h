#pragma once
#include "HeightMap.h"
#include "Object3d.h"
#include <unordered_map>

class TerrainBox
{
private:// �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	//����̔�
	struct CUBE_INFO {
		//���f��
		std::array<std::unique_ptr<Model>, 2> model;
		//�`��ݒ�
		std::array<std::unique_ptr<Object3d>, 2> object;
	};

	//�ʂ̏��
	enum class SURFACE_STATE {
		STOP,
		CHANGE_ATTACK,
		CHANGE_DEFENSE,
		ATTACK,
		DEFENSE,
	};

	//�n�`���
	struct TERRAIN_INFO {
		//�n�`���
		std::unique_ptr<TerrainModel> model;
		//�`��ݒ�
		std::unique_ptr<HeightMap> object;
		//���΂̖ʔԍ�
		int reverseSurface;
		//�ύX�n�`�ԍ�
		std::array<int,2> changeModelNum;
		//�ʂ̏��
		SURFACE_STATE state;
		//�Փ˔���̕ύX
		bool isHitChange;
		//�U������
		XMFLOAT3 attackDirection;
	};

	enum class PHASE {
		ATTACK,
		CHANGE_TERRAIN,
	};

public:

	TerrainBox() {};
	~TerrainBox();

	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<TerrainBox> Create();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ����`��
	/// </summary>
	void HitDraw();

private:
	
	/// <summary>
	/// �Փ˔���Z�b�g
	/// </summary>
	void SetCollider(TERRAIN_INFO& _surface);

	/// <summary>
	/// �ύX�ʂ̃Z�b�g
	/// </summary>
	void SetChangeTerrain();

	/// <summary>
	/// �n�`�̕ϑJ
	/// </summary>
	void ChangeTerrain();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private:

	//�t�B�[���h�𕢂��L���[�u��
	static const int cube_num = 2;
	//�ێ����f����
	static const int model_num = 6;
	//�n�`�̐�
	static const int box_surface_num = 6;
	//�n�`�̑傫��
	static const float box_scale;

	enum MODEL_NUMBER {
		FLAT = 0,//���ʔԍ�
		ATTACK = 3,//���̔ԍ��܂ōU���p�n�`
		DEFENSE = model_num,//���̔ԍ��܂Ŗh��n�`
	};

private:

	//�^�C�}�[
	int timer;
	//�t�B�[���h�𕢂��L���[�u
	std::array<CUBE_INFO, cube_num> cube;
	//�ێ����f��
	std::array<std::unique_ptr<TerrainModel>, model_num> terrainModel;
	//�n�`�`��
	std::array<TERRAIN_INFO, box_surface_num> surface;
	//���݂̒n�`�̏��
	PHASE phase;

public:

	static float GetScale() { return box_scale; }

};