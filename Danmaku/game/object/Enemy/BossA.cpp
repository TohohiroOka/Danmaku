#include "BossA.h"
#include "BulletManager.h"
#include "DebugText.h"
#include "GameHelper.h"
#include "EnemyManager.h"
#include "Easing.h"
#include "BeamManager.h"

using namespace DirectX;

std::vector<BaseEnemy::MOVE_LIST> BossA::moveList;
const std::array<XMFLOAT3, BossA::partsNum> BossA::partsPos = {
	XMFLOAT3{-10,-10,10},{-10,-10,-10},//����
	{-10,10,10},{-10,10,-10},//����
	{10,-10,10},{10,-10,-10},//�E��
	{10,10,10},{10,10,-10},//�E��
};
const std::array<XMFLOAT3, BossA::partsNum> BossA::partsSplitPos={
	XMFLOAT3{ 50.0f,            400.0f, 50.0f,           },//����
			{ 150.0f,            400.0f, 150.0f,           },//������
			{ mapSizeX - 50.0f, 400.0f, 50.0f,           },//����
			{ mapSizeX - 150.0f, 400.0f, 150.0f,           },//���㒆
			{ 50.0f,            400.0f, mapSizeZ - 50.0f },//�E��
			{ 150.0f,            400.0f, mapSizeZ - 150.0f },//�E����
			{ mapSizeX - 50.0f, 400.0f, mapSizeZ - 50.0f },//�E��
			{ mapSizeX - 150.0f, 400.0f, mapSizeZ - 150.0f } //�E�㒆
};


XMFLOAT3 SetColor(const int timer) {
	std::array<bool, 2> colorFlag = { rand() % 2 == 0,rand() % 2 == 0 };
	XMFLOAT3 color = { 0.0f,0.0f,0.0f };
	const float setColorNum = 0.5f;
	if (timer % 3 == 0) {
		color.x = setColorNum;
		if (!colorFlag[0]) { return color; }
		if (colorFlag[1]) {
			color.y = setColorNum;
		} else {
			color.z = setColorNum;
		}
	} else if (timer % 3 == 1) {
		color.y = setColorNum;
		if (!colorFlag[0]) { return color; }
		if (colorFlag[1]) {
			color.x = setColorNum;
		} else {
			color.z = setColorNum;
		}

	} else if (timer % 3 == 2) {
		color.z = setColorNum;
		if (!colorFlag[0]) { return color; }
		if (colorFlag[1]) {
			color.x = setColorNum;
		} else {
			color.y = setColorNum;
		}
	}

	return color;
}

BossA::BossA()
{
	//�^�C�}�[
	timer = 0;
	//HP
	hp = maxHp;
	//�n�ʂɂ��Ă��邩
	onGround = false;
	//�ړ����x
	speed = 1.0f;
	//�I�u�W�F�N�g�̐���
	object = Object3d::Create(bossModel[0].get());
	//���W�Z�b�g�����ɔz�u
	pos = { mapSizeX / 2.0f,250.0f ,mapSizeZ / 2.0f };
	object->SetPosition(pos);

	//�傫���Z�b�g
	scale = 3.0f;
	object->SetScale({ scale ,scale ,scale });

	//�u���[����
	object->SetBloom(true);

	//parts�̃Z�b�g
	float partsScale = scale * 2.0f;
	for (int i = 0; i < partsNum; i++) {
		parts[i].instance = Object3d::Create(bossModel[1].get());
		XMFLOAT3 addPos = pos;
		addPos.x += partsPos[i].x;
		addPos.y += partsPos[i].y;
		addPos.z += partsPos[i].z;
		parts[i].instance->SetPosition(addPos);
		parts[i].instance->SetScale({ partsScale ,partsScale ,partsScale });
	}

	//�U��������
	attack[0].kind = int(BULLET_KIND_CENTER::SNAKE);
	//attack[1].kind = int(BULLET_KIND::LATTICE_BAEM_SET_X);
	for (auto& i : attack)
	{
		i.oldKind = i.kind;
	}

	//�{�X�̈ʒu
	posState = POS_STATE::CENTER;
	//�{�X�̑O��̈ʒu
	oldPosState = posState;
	//�ړ��^�C�}�[
	moveTimer = 0;
}

std::unique_ptr<BossA> BossA::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	BossA* instance = new BossA();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();

	return std::unique_ptr<BossA>(instance);
}

void BossA::Update()
{
	if (!isMovie) {
		//�s���E�U���̍X�V
		if (timer % 300 == 299)
		{
			//�U���ɉ����Ĉړ�
			posState= POS_STATE(Randomint(int(POS_STATE::SIZE)-1));

			//�U�������߂�
			if (posState == POS_STATE::CENTER) {
				for (auto& i : attack)
				{
					i.kind = Randomint(int(BULLET_KIND_CENTER::SIZE) - 2) + 1;
					i.rota = { 0,0,0 };
				}
			} else if (posState == POS_STATE::SPLIT) {
				attack[0].kind = Randomint(int(BULLET_KIND_SPLIT::SIZE) - 2) + 1;
				attack[0].rota = { 0,0,0 };
			}

			timer = 0;
		}

		//�ړ�
		if (posState != oldPosState)
		{
			Move();
		}
		else {
			timer++;

			//�U��
			Attack();
		}
	}
	BaseEnemy::Update();
}

void BossA::Draw()
{
	if (isDamageStaging) {
		damageTimer++;
		if (damageTimer % 5) {
			if (isDraw)
			{
				isDraw = false;
			} else
			{
				isDraw = true;
			}
		}
		if (damageTimer > 50)
		{
			isDamageStaging = false;
			isDraw = true;
			damageTimer = 0;
		}
	}

	if (!isDraw) { return; }
	BaseEnemy::Draw();
	for (auto& i : parts) {
		i.instance->Draw();
	}
}

void BossA::Attack()
{
	if (posState == POS_STATE::CENTER)
	{
		for (auto& attackItr : attack)
		{
			//�~�`�̃_���}�N
			if (attackItr.kind == int(BULLET_KIND_CENTER::CIRCLE) && timer % 20 == 1)
			{
				for (int a = 0; a < 50; a++)
				{
					attackItr.rota.y += 1.0f;
					if (attackItr.rota.y > 360) {
						attackItr.rota.y -= 360.0f;
					}
					float radiunXZ = XMConvertToRadians(attackItr.rota.y);
					float cosXZ = cosf(radiunXZ);
					float sonXZ = sinf(radiunXZ);

					for (int b = 0; b < 5; b++)
					{
						attackItr.rota.x += 10;
						if (attackItr.rota.x > 360) {
							attackItr.rota.x -= 360.0f;
						}
						float radiun = XMConvertToRadians(attackItr.rota.x);

						BulletManager::SetBossBulletCircle(pos,
							{ cos(radiun) * cosXZ,cos(radiun) * sonXZ,sin(radiun) }, 10.0f);
					}
				}
			}
			//�Ռ��g�̂悤�ȉ�̒e���΂�
			else if (attackItr.kind == int(BULLET_KIND_CENTER::SHOCK_WAVE) && timer % 10 == 1)
			{
				for (int a = 0; a < 40; a++)
				{
					attackItr.rota.y += 1.0f;
					if (attackItr.rota.y > 360) {
						attackItr.rota.y -= 360.0f;
					}
					float radiunXZ = XMConvertToRadians(attackItr.rota.y);
					float cosXZ = cosf(radiunXZ);
					float sonXZ = sinf(radiunXZ);

					for (int b = 0; b < 5; b++)
					{
						attackItr.rota.x += 360.0f / 5.0f;
						if (attackItr.rota.x > 360) {
							attackItr.rota.x -= 360.0f;
						}
						float radiun = XMConvertToRadians(attackItr.rota.x);

						BulletManager::SetBossBulletCircle(pos,
							{ cos(radiun) * cosXZ,cos(radiun) * sonXZ,sin(radiun) }, 10.0f);
					}
				}
			}
			//�~�`�ɉ�]���Ȃ����΂�
			else if (attackItr.kind == int(BULLET_KIND_CENTER::CIRCLE_ROTATE))
			{
				if (timer % 3 == 1)
				{
					attackItr.rota.y += 2.0f;
					float radiunXZ = XMConvertToRadians(attackItr.rota.y);
					for (int i = 0; i < bulletNum; i++)
					{
						float ratio = float(i) / float(bulletNum);
						float nowAngle = ratio * 360.0f;
						float radiun = XMConvertToRadians(nowAngle);
						BulletManager::SetBossBulletCircle(pos,
							{ cos(radiun) * cos(radiunXZ),cos(radiun) * sin(radiunXZ),sin(radiun) }, 10.0f);
					}
				}
			}
			//�i�q�r�[��
			else if (attackItr.kind == int(BULLET_KIND_CENTER::LATTICE_BAEM_SET_X))
			{
				const float inConstPos = -mapSizeX / 2.0f;
				XMFLOAT3 inMoveVec = { 1.0f,0.0f,0.0f };
				const float range = mapSizeX / lattice_beam_side_num;
				const int onefor = lattice_beam_side_num / 3;
				attackItr.beamSetNum++;
				//�r�[���Z�b�g
				for (int a = attackItr.lattice_beam_pos[0]; a < attackItr.beamSetNum * onefor; a++) {
					for (int b = 0; b < lattice_beam_side_num; b++) {
						XMFLOAT3 inPos = { inConstPos,range * float(a),range * float(b) };
						BeamManager::SetLatticeBeam(inPos, inMoveVec, 10.0f, { 0.9f,0.2f,0.2f });
					}
					attackItr.lattice_beam_pos[0] = a;
				}

				attackItr.lattice_beam_pos[1] = 0;

				//���ɍs��
				if (attackItr.beamSetNum >= 3) {
					attackItr.beamSetNum = 0;
					attackItr.lattice_beam_pos[0] = 0;
					attackItr.kind = int(BULLET_KIND_CENTER::LATTICE_BAEM_SET_Y);
				}
			}
			//�i�q�r�[��
			else if (attackItr.kind == int(BULLET_KIND_CENTER::LATTICE_BAEM_SET_Y))
			{
				const float inConstPos = -mapSizeX / 2.0f;
				XMFLOAT3 inMoveVec = { 0.0f,1.0f,0.0f };
				const float range = mapSizeX / lattice_beam_side_num;
				const int onefor = lattice_beam_side_num / 3;
				attackItr.beamSetNum++;

				//�r�[���Z�b�g
				for (int a = attackItr.lattice_beam_pos[0]; a < attackItr.beamSetNum * onefor; a++) {
					for (int b = 0; b < lattice_beam_side_num; b++) {
						XMFLOAT3 inPos = { range * float(a),inConstPos,range * float(b) };
						BeamManager::SetLatticeBeam(inPos, inMoveVec, 10.0f, { 0.9f,0.2f,0.2f });
					}
					attackItr.lattice_beam_pos[0] = a;
				}

				attackItr.lattice_beam_pos[1] = 0;

				//���ɍs��
				if (attackItr.beamSetNum >= 3) {
					attackItr.beamSetNum = 0;
					attackItr.lattice_beam_pos[0] = 0;
					attackItr.kind = int(BULLET_KIND_CENTER::LATTICE_BAEM_SET_Z);
				}
			}
			//�i�q�r�[��
			else if (attackItr.kind == int(BULLET_KIND_CENTER::LATTICE_BAEM_SET_Z))
			{
				const float inConstPos = -mapSizeX / 2.0f;
				XMFLOAT3 inMoveVec = { 0.0f,0.0f,1.0f };
				const float range = mapSizeX / lattice_beam_side_num;
				const int onefor = lattice_beam_side_num / 3;
				attackItr.beamSetNum++;
				//�r�[���Z�b�g
				for (int a = attackItr.lattice_beam_pos[0]; a < attackItr.beamSetNum * onefor; a++) {
					for (int b = 0; b < lattice_beam_side_num; b++) {
						XMFLOAT3 inPos = { range * float(a),range * float(b),inConstPos };
						BeamManager::SetLatticeBeam(inPos, inMoveVec, 10.0f, { 0.9f,0.2f,0.2f });
					}
					attackItr.lattice_beam_pos[0] = a;
				}

				attackItr.lattice_beam_pos[1] = 0;

				//���ɍs��
				if (attackItr.beamSetNum >= 3) {
					attackItr.beamSetNum = 0;
					attackItr.lattice_beam_pos[0] = 0;
					attackItr.kind = int(BULLET_KIND_CENTER::FIREWORKE);
				}
			}
			//��̒e�����j���~�`�ɒe���΂�
			else if (attackItr.kind == int(BULLET_KIND_CENTER::FIREWORKE))
			{
				if (timer % 50 == 0) {
					BulletManager::SetBossBulletFireWorke(pos, 10.0f, { 0.1f,0.6f,0.1f });
				}
			}
			//�v���C���[�̕����ɒe���΂�
			else if (attackItr.kind == int(BULLET_KIND_CENTER::HOMING))
			{
				//�F����
				XMFLOAT3 color = SetColor(timer);
				BulletManager::SetBossBulletHoming(pos, 10.0f, color);
			}
			//���j���v���C���[�̕����ɒe���΂�
			else if (attackItr.kind == int(BULLET_KIND_CENTER::BOMB_HOMING))
			{
				if (timer % 3 == 1)
				{
					XMFLOAT2 _angle = { Randomfloat(360),Randomfloat(360) };
					XMFLOAT2 radiun = { XMConvertToRadians(_angle.x),XMConvertToRadians(_angle.y) };

					//�F����
					XMFLOAT3 color = SetColor(timer);
					BulletManager::SetBossBulletBombHoming(pos, { cos(radiun.x) * cos(radiun.y),cos(radiun.x) * sin(radiun.y),sin(radiun.x) }, color);
				}
			}
			//���ʏ�ɃE�l�E�l����e���΂�
			else if (attackItr.kind == int(BULLET_KIND_CENTER::SNAKE))
			{
				//�F����
				XMFLOAT3 color = SetColor(timer);
				BulletManager::SetBossBulletSnake(pos, color);
			}
			
			//�O�t���[���̏W�ނ�ۑ�
			attackItr.oldKind = attackItr.kind;
		}
	}
	//���􎞂̍U��
	else if (posState == POS_STATE::SPLIT)
	{
		//�Ǐ]��������΂�
		if (attack[0].kind == int(BULLET_KIND_SPLIT::HOMING_LINE))
		{
			if (timer == 1) {
				for (auto& itr : parts)
				{
					itr.HOMING_LINEpos = itr.instance->GetPosition();
				}
			}

			//�ǔ��e��
			for (auto& itr : parts)
			{
				//�F����
				XMFLOAT3 color = SetColor(timer);
				BulletManager::SetBossBulletHomingLine(itr.HOMING_LINEpos, 5.0f, color, 1);
			}
		}
	}
}

void BossA::Move()
{
	//�������番�� / �R�A�͒���
	if ((oldPosState == POS_STATE::CENTER) && (posState == POS_STATE::SPLIT)) {
		moveTimer++;
		const float maxTime = 100.0f;
		float ratio = float(moveTimer) / maxTime;

		float endScale = 15.0f;

		for (int i = 0; i < 8; i++) {
			XMFLOAT3 inPos = {};
			inPos.x = Easing::InCirc(mapSizeX / 2.0f + partsPos[i].x, partsSplitPos[i].x, ratio);
			inPos.y = Easing::InCirc(250.0f + partsPos[i].y, partsSplitPos[i].y, ratio);
			inPos.z = Easing::InCirc(mapSizeZ / 2.0f + partsPos[i].z, partsSplitPos[i].z, ratio);

			float inScale = 0.0f;
			inScale = Easing::InCirc(6.0f, endScale, ratio);

			parts[i].instance->SetPosition(inPos);
			parts[i].instance->SetScale({ inScale ,inScale ,inScale });
		}

		if (ratio >= 1.0f) {
			oldPosState = posState;
			moveTimer = 0;
		}
	}
	//�������璆��
	else if ((oldPosState == POS_STATE::SPLIT) && (posState == POS_STATE::CENTER)){
		moveTimer++;
		const float maxTime = 100.0f;
		float ratio = float(moveTimer) / maxTime;

		float endScale = 15.0f;

		for (int i = 0; i < 8; i++) {
			XMFLOAT3 inPos = {};
			inPos.x = Easing::InCirc(partsSplitPos[i].x, mapSizeX / 2.0f + partsPos[i].x, ratio);
			inPos.y = Easing::InCirc(partsSplitPos[i].y, 250.0f + partsPos[i].y, ratio);
			inPos.z = Easing::InCirc(partsSplitPos[i].z, mapSizeZ / 2.0f + partsPos[i].z, ratio);

			float inScale = 0.0f;
			inScale = Easing::InCirc(endScale, 6.0f, ratio);

			parts[i].instance->SetPosition(inPos);
			parts[i].instance->SetScale({ inScale ,inScale ,inScale });
		}

		if (ratio >= 1.0f) {
			oldPosState = posState;
			moveTimer = 0;
		}
	}
}