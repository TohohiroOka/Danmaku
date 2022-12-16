#pragma once
#include "BaseEnemy.h"
#include <array>

class BossA : public BaseEnemy
{
private:

	//�������̍U��
	enum class BULLET_KIND_CENTER
	{
		NONE,//�����Ȃ�
		CIRCLE,//�~��ɏo��
		LATTICE_BAEM_SET_X,//�i�q��r�[���Z�b�gyz����
		LATTICE_BAEM_SET_Y,//�i�q��r�[���Z�b�gxz����
		LATTICE_BAEM_SET_Z,//�i�q��r�[���Z�b�gxy����
		FIREWORKE,//�ԉΓI�Ȃ���
		HOMING,//�z�[�~���O
		SNAKE,//���˂��˓���
		SHOCK_WAVE,//�Ռ��g
		CIRCLE_ROTATE,//�~��ɏo��
		BOMB_HOMING,//���j���Ă���
		SIZE,//���v
	};

	enum class BULLET_KIND_SPLIT
	{
		NONE,//�����Ȃ�
		HOMING_LINE,//�Ǐ]��(�~�`�ɏo��)
		SIZE,//���v
	};

	//�ʒu
	enum class POS_STATE
	{
		CENTER,//����
		SPLIT,//����
		//ROTATION,//��]
		SIZE,//���v
	};

	//��񕪂̍U�����
	struct BULLET_INFO
	{
		//�e�̎��
		int kind;
		//1�O�t���[���̒e�̎��
		int oldKind;
		//��]��
		XMFLOAT3 rota = { 0,0,0 };
		//�r�[���̃Z�b�g��
		int beamSetNum = 0;
		//�i�q�r�[���̍��W
		std::array<int,2> lattice_beam_pos = { 0,0 };
	};

	//parts���Ƃ̏��
	struct PARTS_INFO {
		std::unique_ptr<Object3d> instance;
		//�p�x�z�[�~���O���p�p�x
		XMFLOAT2 radiun;
		//HOMING_LINE�p�̍��W
		XMFLOAT3 HOMING_LINEpos;
	};

	static const int lattice_beam_side_num = 10;

public:

	BossA();
	~BossA() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<BossA> Create();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

private:

	//���ɂł���U���̎��
	static const int kindNum = 1;
	//�{�X�̍ő�̗�
	const int maxHp = 200;
	//�ړ��o�H
	static std::vector<MOVE_LIST> moveList;
	//parts�̐�
	static const int partsNum = 8;
	//���S����̋���
	static const std::array<XMFLOAT3, partsNum> partsPos;
	//���S����̋���
	static const std::array<XMFLOAT3, partsNum> partsSplitPos;

private:

	//����̃I�u�W�F�N�g
	std::array<PARTS_INFO, partsNum> parts;
	//�^�C�}�[
	int timer;
	//���ɏo���e�̗�
	const int bulletNum = 36;
	//��񕪂̍U�����
	std::array<BULLET_INFO, kindNum> attack;
	//�n�`�ύX����move,�Փ˔�����o���Ȃ�����
	bool isMovie;
	//�{�X�̈ʒu
	POS_STATE posState;
	//�{�X�̑O��̈ʒu
	POS_STATE oldPosState;
	//�ړ��^�C�}�[
	int moveTimer;

public:

	int GetMaxHp() { return maxHp; }
	int GetHp() { return hp; }
	float GetHpRatio() { return float(hp) / (maxHp); }
	void SetMovie() {
		isMovie = true;
	};

	void EndMovie() {
		isMovie = false;
	}

};
