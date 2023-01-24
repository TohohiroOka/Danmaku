#include "TerrainBox.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "DebugText.h"

#include "GameHelper.h"
#include "BulletManager.h"

const float TerrainBox::box_scale = 5.0f;

TerrainBox::~TerrainBox()
{

}

std::unique_ptr<TerrainBox> TerrainBox::Create()
{
	// �I�u�W�F�N�g�̃C���X�^���X�𐶐�
	TerrainBox* instance = new TerrainBox();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	instance->Initialize();

	return std::unique_ptr<TerrainBox>(instance);
}

void TerrainBox::Initialize()
{
	//���f���ǂݍ���
	const std::string jimen = "jimen.png";
	const std::string kabe = "kabe.png";
	terrainModel[0] = TerrainModel::FlatlandModelCreate(jimen, kabe);
	std::string mapName="heightmap";
	//�g�p����n�`�̐���
	for (int i = 1; i < model_num; i++) {
		terrainModel[i] = TerrainModel::Create(mapName + std::to_string(i) + ".bmp", jimen, kabe);
	}

	//�n�`���Ƃ̍��W
	std::array<XMFLOAT3, box_surface_num> surfacePos = {
		XMFLOAT3{0,0,0},{1275,1275,0},{0,0,1275},{0,1275,0},{1275,0,0},{0,1275,0}
	};

	//�n�`���Ƃ̊p�x
	std::array<XMFLOAT3, box_surface_num> surfaceRota = {
		XMFLOAT3{0,0,0},{0,0,180},{-90,0,0},{90,0,0},{0,0,90},{0,0,-90}
	};

	//�n�`���Ƃ̍U������
	std::array<XMFLOAT3, box_surface_num> surfaceDirection = {
		XMFLOAT3{0,-1,0},{0,1,0},{0,0,-1},{0,0,1},{-1,0,0},{1,0,0}
	};

	//�{�b�N�X�̖ʗp�ɕ��ʐ���
	for (int i = 0; i < box_surface_num; i++) {
		surface[i].model = TerrainModel::FlatlandModelCreate(jimen, kabe);
		surface[i].object = HeightMap::Create(surface[i].model.get());
		surface[i].state = SURFACE_STATE::STOP;
		surface[i].attackDirection = surfaceDirection[i];
		surface[i].changeModelNum[0] = MODEL_NUMBER::FLAT;
		surface[i].changeModelNum[1] = MODEL_NUMBER::FLAT;
		if (i % 2 == 0) {
			surface[i].reverseSurface = i + 1;
		} else {
			surface[i].reverseSurface = i - 1;
		}
		//�}�b�v���ύX
		surface[i].object->SetPosition(surfacePos[i]);
		surface[i].object->SetScale({ box_scale,box_scale,box_scale });
		surface[i].object->SetRotation(surfaceRota[i]);

		// �R���C�_�[�̒ǉ�
		MeshCollider* collider = new MeshCollider;
		surface[i].object->SetCollider(collider);
		collider->ConstructTriangles(surface[i].object->GetHitVertices(), surface[i].object->GetHitIndices());
		collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	}

	SetChangeTerrain();

	//�^�C�}�[
	timer = 0;

	//���݂̒n�`�̏��
	phase = PHASE::CHANGE_TERRAIN;
}

void TerrainBox::Update()
{
	timer++;

	//�U��
	if (phase == PHASE::ATTACK) {
		
		Attack();
	}
	//�n�`�ω�
	else if (phase == PHASE::CHANGE_TERRAIN) {
		ChangeTerrain();
	}

	//�s���̍X�V
	if (timer == 0) {
		SetChangeTerrain();
	}

	DebugText* text = DebugText::GetInstance();
	std::string strtimer = std::to_string(timer);
	text->Print("timer : " + strtimer, 100, 100);
	text = nullptr;

}

void TerrainBox::Draw()
{
	for(auto& itr : surface)
	{
		itr.object->AddConstBufferUpdate();
		itr.object->Draw();
	}
}

void TerrainBox::HitDraw()
{
	for (auto& itr : surface)
	{
		itr.object->ColliderDraw();
	}
}

void TerrainBox::SetCollider(TERRAIN_INFO& _surface)
{
	//���̔������
	_surface.object->DeleteCollider();

	// �R���C�_�[�̒ǉ�
	MeshCollider* collider = new MeshCollider;
	_surface.object->SetCollider(collider);
	//collider->ConstructTriangles(object->GetModel());
	collider->ConstructTriangles(_surface.object->GetHitVertices(), _surface.object->GetHitIndices());
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
}

void TerrainBox::SetChangeTerrain()
{
	//�U����
	const int changeTerrain = Randomint(box_surface_num - 1);
	surface[changeTerrain].changeModelNum[1] = Randomint(MODEL_NUMBER::ATTACK - 1) + 1;
	surface[changeTerrain].state = SURFACE_STATE::CHANGE_ATTACK;
	//�h���
	int defense = -1;
	do {
		defense = Randomint(model_num - 1) + 1;
	} while (surface[changeTerrain].reverseSurface == defense);

	int range= MODEL_NUMBER::DEFENSE - MODEL_NUMBER::ATTACK;
	surface[defense].changeModelNum[1] = Randomint(range - 1) + MODEL_NUMBER::ATTACK - 1;
	surface[defense].state = SURFACE_STATE::CHANGE_DEFENSE;
}

void TerrainBox::ChangeTerrain()
{
	//�n�`�ύX�̎���
	const float maxTime = 100;

	//�n�`�ύX
	for (auto& itr : surface)
	{
		//�`�F���W�t���O��true
		if (itr.state != SURFACE_STATE::CHANGE_ATTACK &&
			itr.state != SURFACE_STATE::CHANGE_DEFENSE) {continue;}

		//�n�`�ω�
		itr.object->SetChangeDrawModel(terrainModel[itr.changeModelNum[0]].get(),
			terrainModel[itr.changeModelNum[1]].get(), float(timer) / maxTime);

		//�Փ˔���ω�
		if (timer % 10 == 0) {
			//����p���_�ϊ�
			itr.object->SetChangeHitModel(terrainModel[itr.changeModelNum[0]].get(),
				terrainModel[itr.changeModelNum[1]].get(), float(timer) / maxTime);

			SetCollider(itr);
		}

		//�ω����I������
		if (timer < maxTime) { continue; }

		//����p���_�ϊ�
		itr.object->SetChangeHitModel(terrainModel[itr.changeModelNum[0]].get(),
			terrainModel[itr.changeModelNum[1]].get(), 1.0f);

		SetCollider(itr);

		//��Ԃ�ύX
		//�U���p
		if(itr.state == SURFACE_STATE::CHANGE_ATTACK){
			itr.state = SURFACE_STATE::ATTACK;
		}
		//�h��p
		else if (itr.state == SURFACE_STATE::CHANGE_DEFENSE) {
			itr.state = SURFACE_STATE::DEFENSE;
		}
		itr.changeModelNum[0] = itr.changeModelNum[1];
	}

	//�ω����I������
	if (timer < maxTime) { return; }
	phase = PHASE::ATTACK;
	timer = 0;
}

void TerrainBox::Attack()
{
	//�����`�e
	for (auto& itr : surface)
	{
		//�U������ʂ��ǂ���
		if (itr.state != SURFACE_STATE::ATTACK) { continue; }

		//�������W
		XMFLOAT3 initPos={};

		//���W����
		//x���W
		if (itr.attackDirection.x != 0) {
			if(itr.attackDirection.x == 1){
				initPos.x = 0;
			} else {
				initPos.x = mapSize;
			}
			initPos.y = Randomfloat(mapSize);
			initPos.z = Randomfloat(mapSize);
		}
		//y���W
		else if (itr.attackDirection.y != 0) {
			if (itr.attackDirection.y == 1) {
				initPos.y = 0;
			} else {
				initPos.y = mapSize;
			}
			initPos.x = Randomfloat(mapSize);
			initPos.z = Randomfloat(mapSize);
		}
		//z���W
		else if (itr.attackDirection.z != 0) {
			if (itr.attackDirection.z == 1) {
				initPos.z = 0;
			} else {
				initPos.z = mapSize;
			}
			initPos.x = Randomfloat(mapSize);
			initPos.y = Randomfloat(mapSize);
		}


		const int maxBullet = 50;
		for (int i = 0; i < maxBullet; i++) {

			BulletManager::SetBossBulletNormal(initPos, itr.attackDirection,
				10.0f, { 0.8f,0.2f,0.8f });

			//�cor�����ɒe���L�тČ������
			if (initPos.x != 0) {
				initPos.y++;
			} else if (initPos.y != 0) {
				initPos.z++;
			} else{
				initPos.x++;
			}
		}
	}
}
