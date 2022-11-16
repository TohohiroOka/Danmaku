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
	XMFLOAT3{10,10,10},{-10,10,10},
	{10,10,-10},{-10,10,-10},
	{10,-10,10},{-10,-10,10},
	{10,-10,-10},{-10,-10,-10}
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

BossA::BossA(const XMFLOAT3& _pos, const int _destination)
{
	//タイマー
	timer = 0;
	//HP
	hp = maxHp;
	//地面についているか
	onGround = false;
	//移動速度
	speed = 1.0f;
	//オブジェクトの生成
	object = Object3d::Create(bossModel[0].get());
	//座標セット
	pos = _pos;
	pos.y = 200.0f;
	object->SetPosition(pos);

	//大きさセット
	scale = 3.0f;
	object->SetScale({ scale ,scale ,scale });

	//ブルーム等
	object->SetBloom(true);

	//partsのセット
	float partsScale = scale * 2.0f;
	for (int i = 0; i < partsNum; i++) {
		parts[i] = Object3d::Create(bossModel[1].get());
		XMFLOAT3 addPos = pos;
		addPos.x += partsPos[i].x;
		addPos.y += partsPos[i].y;
		addPos.z += partsPos[i].z;
		parts[i]->SetPosition(addPos);
		parts[i]->SetScale({ partsScale ,partsScale ,partsScale });
	}

	//移動フラグ
	isMove = true;
	//移動タイマー
	moveTimer = 0;
	//現在の移動先番号
	destinationNumber = _destination;
	//次の移動先番号
	int rand = int(Randomfloat(int(moveList[destinationNumber].destination.size()) - 1));
	nextDestinationNumber = rand;

	//攻撃初期化
	attack[0].kind = int(BULLET_KIND::LATTICE_BAEM_SET_X);
	//attack[1].kind = int(BULLET_KIND::LATTICE_BAEM_SET_X);
	for (auto& i : attack)
	{
		i.oldKind = i.kind;
	}
}

std::unique_ptr<BossA> BossA::Create(const XMFLOAT3& _pos, const int _destination)
{
	// 3Dオブジェクトのインスタンスを生成
	BossA* instance = new BossA(_pos, _destination);
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	return std::unique_ptr<BossA>(instance);
}

void BossA::Update()
{
	timer++;

	//攻撃
	Attack();

	//移動
	//if (isMove)
	//{
	//	moveTimer++;
	//	const float maxTimer = 150.0f;
	//	const float ratio = float(moveTimer) / maxTimer;
	//	pos.x = Easing::Lerp(moveList[destinationNumber].pos.x, moveList[nextDestinationNumber].pos.x, ratio);
	//	pos.z = Easing::Lerp(moveList[destinationNumber].pos.z, moveList[nextDestinationNumber].pos.z, ratio);

	//	if (ratio >= 1.0f) {
	//		int rand = destinationNumber;

	//		while (rand == destinationNumber) {
	//			rand = int(Randomfloat(int(moveList[nextDestinationNumber].destination.size()) - 1));
	//			rand = moveList[nextDestinationNumber].destination[rand];
	//		}
	//		destinationNumber = nextDestinationNumber;
	//		nextDestinationNumber = rand;
	//		moveTimer = 0;
	//	}
	//}

	//parts更新
	for (int i = 0; i < partsNum; i++) {
		XMFLOAT3 addPos = pos;
		addPos.x += partsPos[i].x;
		addPos.y += partsPos[i].y;
		addPos.z += partsPos[i].z;
		parts[i]->SetPosition(addPos);
	}

	BaseEnemy::Update();

	DebugText* text = DebugText::GetInstance();
	std::string bossHp = std::to_string(hp);
	std::string bossAttack1= std::to_string(attack[0].kind);
	//std::string bossAttack2 = std::to_string(attack[1].kind);
	std::string strDestinationNumber = std::to_string(destinationNumber);
	std::string strNextDestinationNumber = std::to_string(nextDestinationNumber);
	text->Print("Boss hp : " + bossHp, 100, 200);
	text->Print("Boss Attack1 : " + bossAttack1, 100, 225);
	text->Print("destinationNumber : " + strDestinationNumber + "nextDestinationNumber : " + strNextDestinationNumber, 100, 250);

	text = nullptr;
}

void BossA::Draw()
{
	BaseEnemy::Draw();
	for (auto& i : parts) {
		i->Draw();
	}
}

void BossA::Attack()
{
	//if (DirectInput::GetInstance()->TriggerKey(DIK_K))
	//{
	//	attack[0].kind++;
	//	attack[0].kind = attack[0].kind % int(BULLET_KIND::SIZE);
	//}

	//攻撃の更新
	if (timer % 300 == 299)
	{
		for (auto& i : attack)
		{
			i.kind++;
			i.kind = i.kind % int(BULLET_KIND::SIZE);
			i.rota = { 0,0,0 };
		}
		timer = 0;
	}

	//for (int i = 0; i < 1; i++)
	for (int i = 0; i < kindNum; i++)
	{
		if (attack[i].kind == int(BULLET_KIND::CIRCLE) && timer % 20 == 1)
		{
			for (int a = 0; a < 50; a++)
			{
				attack[i].rota.y += 1.0f;
				if (attack[i].rota.y > 360) {
					attack[i].rota.y -= 360.0f;
				}
				float radiunXZ = XMConvertToRadians(attack[i].rota.y);
				float cosXZ = cosf(radiunXZ);
				float sonXZ = sinf(radiunXZ);

				for (int b = 0; b < 5; b++)
				{
					attack[i].rota.x += 10;
					if (attack[i].rota.x > 360) {
						attack[i].rota.x -= 360.0f;
					}
					float radiun = XMConvertToRadians(attack[i].rota.x);

					BulletManager::SetBossBulletCircle(pos,
						{ cos(radiun) * cosXZ,cos(radiun) * sonXZ,sin(radiun) }, 10.0f);
				}
			}
		}
		else if (attack[i].kind == int(BULLET_KIND::SHOCK_WAVE) && timer % 10 == 1)
		{
			for (int a = 0; a < 40; a++)
			{
				attack[i].rota.y += 1.0f;
				if (attack[i].rota.y > 360) {
					attack[i].rota.y -= 360.0f;
				}
				float radiunXZ = XMConvertToRadians(attack[i].rota.y);
				float cosXZ = cosf(radiunXZ);
				float sonXZ = sinf(radiunXZ);

				for (int b = 0; b < 5; b++)
				{
					attack[i].rota.x += 360.0f / 5.0f;
					if (attack[i].rota.x > 360) {
						attack[i].rota.x -= 360.0f;
					}
					float radiun = XMConvertToRadians(attack[i].rota.x);

					BulletManager::SetBossBulletCircle(pos,
						{ cos(radiun) * cosXZ,cos(radiun) * sonXZ,sin(radiun) }, 10.0f);
				}
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::CIRCLE_ROTATE))
		{
			if (timer % 3 == 1)
			{
				attack[i].rota.y += 2.0f;
				float radiunXZ = XMConvertToRadians(attack[i].rota.y);
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
		else if (attack[i].kind == int(BULLET_KIND::LATTICE_BAEM_SET_X))
		{
			const float inConstPos = -mapSize / 2.0f;
			XMFLOAT3 inMoveVec = { 1.0f,0.0f,0.0f };
			const float range = mapSize / lattice_beam_side_num;
			const int onefor = lattice_beam_side_num / 3;
			attack[i].beamSetNum++;
			//ビームセット
			for (int a= attack[i].lattice_beam_pos[0]; a < attack[i].beamSetNum * onefor; a++) {
				for (int b= 0; b < lattice_beam_side_num; b++) {
					XMFLOAT3 inPos = { inConstPos,range * float(a),range * float(b) };
					BeamManager::SetLatticeBeam(inPos, inMoveVec, 10.0f, { 0.9f,0.2f,0.2f });
				}
				attack[i].lattice_beam_pos[0] = a;
			}

			attack[i].lattice_beam_pos[1] = 0;

			//次に行く
			if (attack[i].beamSetNum >= 3) {
				attack[i].beamSetNum = 0;
				attack[i].lattice_beam_pos[0] = 0;
				attack[i].kind = int(BULLET_KIND::LATTICE_BAEM_SET_Y);
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::LATTICE_BAEM_SET_Y))
		{
			const float inConstPos = -mapSize / 2.0f;
			XMFLOAT3 inMoveVec = { 0.0f,1.0f,0.0f };
			const float range = mapSize / lattice_beam_side_num;
			const int onefor = lattice_beam_side_num / 3;
			attack[i].beamSetNum++;

			//ビームセット
			for (int a = attack[i].lattice_beam_pos[0]; a < attack[i].beamSetNum * onefor; a++) {
				for (int b = 0; b < lattice_beam_side_num; b++) {
					XMFLOAT3 inPos = { range * float(a),inConstPos,range * float(b) };
					BeamManager::SetLatticeBeam(inPos, inMoveVec, 10.0f, { 0.9f,0.2f,0.2f });
				}
				attack[i].lattice_beam_pos[0] = a;
			}

			attack[i].lattice_beam_pos[1] = 0;

			//次に行く
			if (attack[i].beamSetNum >= 3) {
				attack[i].beamSetNum = 0;
				attack[i].lattice_beam_pos[0] = 0;
				attack[i].kind = int(BULLET_KIND::LATTICE_BAEM_SET_Z);
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::LATTICE_BAEM_SET_Z))
		{
			const float inConstPos = -mapSize / 2.0f;
			XMFLOAT3 inMoveVec = { 0.0f,0.0f,1.0f };
			const float range = mapSize / lattice_beam_side_num;
			const int onefor = lattice_beam_side_num / 3;
			attack[i].beamSetNum++;
			//ビームセット
			for (int a = attack[i].lattice_beam_pos[0]; a < attack[i].beamSetNum * onefor; a++) {
				for (int b = 0; b < lattice_beam_side_num; b++) {
					XMFLOAT3 inPos = { range * float(a),range * float(b),inConstPos };
					BeamManager::SetLatticeBeam(inPos, inMoveVec, 10.0f, { 0.9f,0.2f,0.2f });
				}
				attack[i].lattice_beam_pos[0] = a;
			}

			attack[i].lattice_beam_pos[1] = 0;

			//次に行く
			if (attack[i].beamSetNum >= 3) {
				attack[i].beamSetNum = 0;
				attack[i].lattice_beam_pos[0] = 0;
				attack[i].kind = int(BULLET_KIND::FIREWORKE);
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::FIREWORKE))
		{
			if (timer % 50==0) {
				BulletManager::SetBossBulletFireWorke(pos, 10.0f, { 0.1f,0.6f,0.1f });
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::HOMING))
		{
			//色決め
			XMFLOAT3 color = SetColor(timer);
			BulletManager::SetBossBulletHoming(pos, 10.0f, color);
		} 
		else if (attack[i].kind == int(BULLET_KIND::BOMB_HOMING))
		{
			if (timer % 3 == 1)
			{
				XMFLOAT2 _angle = { Randomfloat(360),Randomfloat(360) };
				XMFLOAT2 radiun = { XMConvertToRadians(_angle.x),XMConvertToRadians(_angle.y) };

				//色決め
				XMFLOAT3 color = SetColor(timer);
				BulletManager::SetBossBulletBombHoming(pos, { cos(radiun.x) * cos(radiun.y),cos(radiun.x) * sin(radiun.y),sin(radiun.x) }, color);
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::SNAKE))
		{
			//色決め
			XMFLOAT3 color = SetColor(timer);
			BulletManager::SetBossBulletSnake(pos, color);
		} 
		else if (attack[i].kind == int(BULLET_KIND::HOMING_LINE1))
		{
			for (int lineNumber = 0; lineNumber < homing_line_num; lineNumber++)
			{
				//一つ分の角度
				const float angleOne = 360 / homing_line_num;

				//前準備座標のセット
				if (attack[i].oldKind != int(BULLET_KIND::HOMING_LINE1))
				{
					attack[i].HOMING_LINEpos[lineNumber] = { pos.x + Randomfloat(100) - 50.0f,
						pos.y + Randomfloat(100) - 50.0f,pos.z + Randomfloat(100) - 50.0f };

					attack[i].radiun[lineNumber] = { XMConvertToRadians(Randomfloat(360 * 2) - 360.0f),
						XMConvertToRadians(i * angleOne) };
				}

				//弾セット
				XMFLOAT3 color = SetColor(timer);
				BulletManager::SetBossBulletHomingLine(attack[i].HOMING_LINEpos[lineNumber], 5.0f, color, false);

				const float speed = 2.0f;
				attack[i].HOMING_LINEpos[lineNumber].x += speed * cos(attack[i].radiun[lineNumber].x) * cos(attack[i].radiun[lineNumber].y);
				attack[i].HOMING_LINEpos[lineNumber].y += speed * cos(attack[i].radiun[lineNumber].x) * sin(attack[i].radiun[lineNumber].y);
				attack[i].HOMING_LINEpos[lineNumber].z += speed * sin(attack[i].radiun[lineNumber].x);

				//タイマー以上で追従モードに変化
				if (timer / 300 > 30)
				{
					attack[i].kind = int(BULLET_KIND::HOMING_LINE2);
				}
			}
		} 
		else if (attack[i].kind == int(BULLET_KIND::HOMING_LINE2))
		{
			for (auto& linePos : attack[i].HOMING_LINEpos)
			{
				//色決め
				XMFLOAT3 color = SetColor(timer);
				BulletManager::SetBossBulletHomingLine(linePos, 5.0f, color, 1);
			}
		}

		//前フレームの集類を保存
		attack[i].oldKind = attack[i].kind;
	}
}
