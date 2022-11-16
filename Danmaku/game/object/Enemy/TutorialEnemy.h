#pragma once
#include "BaseEnemy.h"

class TutorialEnemy : public BaseEnemy
{
public:

	TutorialEnemy(const XMFLOAT3& _pos);
	~TutorialEnemy() {};

	/// <summary>
	/// ê∂ê¨
	/// </summary>
	/// <param name="_pos">ç¿ïW</param>
	/// <returns></returns>
	static std::unique_ptr<TutorialEnemy> Create(const XMFLOAT3& _pos);
};