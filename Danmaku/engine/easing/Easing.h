#pragma once
#include <DirectXMath.h>

class Easing
{
private: // エイリアス
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;

public:
	//等速直線運動
	static float Lerp(const float _start, const float _end, const float _time);
	static XMFLOAT3 Lerp(const XMFLOAT3& _start, const XMFLOAT3& _end, const float _time);

	//sin
	static float InSine(const float _start, const float _end, const float _time);
	static float OutSine(const float _start, const float _end, const float _time);
	static float InOutSine(const float _start, const float _end, const float _time);

	//2乗
	static float InQuad(const float _start, const float _end, const float _time);
	static float OutQuad(const float _start, const float _end, const float _time);
	static float InOutQuad(const float _start, const float _end, const float _time);

	//3乗
	static float InCubic(const float _start, const float _end, const float _time);
	static float OutCubic(const float _start, const float _end, const float _time);
	static float InOutCubic(const float _start, const float _end, const float _time);

	//4乗
	static float InQuart(const float _start, const float _end, const float _time);
	static float OutQuart(const float _start, const float _end, const float _time);
	static float InOutQuart(const float _start, const float _end, const float _time);

	//5乗
	static float InQuint(const float _start, const float _end, const float _time);
	static float OutQuint(const float _start, const float _end, const float _time);
	static float InOutQuint(const float _start, const float _end, const float _time);

	//Expo
	static float InExpo(const float _start, const float _end, const float _time);
	static float OutExpo(const float _start, const float _end, const float _time);
	static float InOutExpo(const float _start, const float _end, const float _time);

	//Circ
	static float InCirc(const float _start, const float _end, const float _time);
	static float OutCirc(const float _start, const float _end, const float _time);
	static float InOutCirc(const float _start, const float _end, const float _time);

	//Back
	static float InBack(const float _start, const float _end, const float _time);
	static float OutBack(const float _start, const float _end, const float _time);
	static float InOutBack(const float _start, const float _end, const float _time);

	//Elast
	static float InElastic(const float _start, const float _end, const float _time);
	static float OutElastic(const float _start, const float _end, const float _time);
	static float InOutElastic(const float _start, const float _end, const float _time);

	//Bounc
	static float InBounce(const float _start, const float _end, const float _time);
	static float OutBounce(const float _start, const float _end, const float _time);
	static float InOutBounce(const float _start, const float _end, const float _time);

private:
	/// <summary>
	/// Bounce計算用
	/// </summary>
	/// <param name="time">イージング時間(0～1)</param>
	/// <returns>イージング計算結果</returns>
	static float OutBounceCalculate(float& _time);
};