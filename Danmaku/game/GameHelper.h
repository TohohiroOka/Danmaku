#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <memory>
#include <random>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "TerrainBox.h"

static const float mapSize = 255.0f * TerrainBox::GetScale();

/// <summary>
/// 桁数取得
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
static unsigned GetDigit(unsigned num) {
	return unsigned(std::to_string(num).length());
}

/// <summary>
/// 二点間の角度取得
/// </summary>
/// <param name="_startPoint">座標1</param>
/// <param name="_endPoint">座標2</param>
/// <returns></returns>
static float GetAngle(const DirectX::XMFLOAT2& _startPoint, const DirectX::XMFLOAT2& _endPoint)
{
	float radian = atan2f(_endPoint.y - _startPoint.y, _endPoint.x - _startPoint.x);
	return DirectX::XMConvertToDegrees(radian);
}

/// <summary>
/// 符号を返す
/// </summary>
/// <param name="A">数値</param>
/// <returns></returns>
static int GetSign(float A) {
	if (A > 0) return 1;
	else if (A < 0) return -1;
	else return 0;
}

/// <summary>
/// 小数部分の桁数出力
/// </summary>
/// <param name="value">値</param>
/// <returns>小数部分の桁数</returns>
static int GetDigit(float value)
{
	std::ostringstream stream;
	stream << value;
	std::string str = stream.str();// 文字列に変換
	const int str_line_max= int(str.size());

	int integer = 0;// 整数部の桁数
	
	for (int i = 0; i < str_line_max; i++)
	{
		// 小数のとき
		if (str[i] == '.')
		{
			// 小数点の位置を保存
			integer = i;
			break;
		}
	}

	//文字列の合計-整数部分で少数部分の桁数を出す
	int count=0;
	if (integer != 0)
	{
		count = str_line_max - integer - 1;
	}

	return count;
}

/// <summary>
/// 乱数生成
/// 0から範囲までの乱数を出力
/// </summary>
/// <param name="range">範囲</param>
/// <returns>乱数</returns>
static float Randomfloat(float _range)
{
	const int str_count= GetDigit(_range);

	//整数に戻すための倍率
	int magnification=1;
	for (int i = 0; i < str_count; i++) {
		magnification*=10;
	}

	//整数での値
	const int int_range= int(_range * magnification);

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, int_range);

	return float(rand100(mt))/ float(magnification);
}

/// <summary>
/// 乱数生成
/// 0から範囲までの乱数を出力
/// </summary>
/// <param name="range">範囲</param>
/// <returns>乱数</returns>
static int Randomint(int _range)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, _range);

	return rand100(mt);
}

/// <summary>
/// 乱数生成
/// 1か-1を出力
/// </summary>
/// <returns>1or-1</returns>
static int RandomSign()
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, 1);

	int sign = rand100(mt);

	if (sign == 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

/// <summary>
/// 円状に乱数を生成。均一化有り
/// </summary>
/// <returns></returns>
static DirectX::XMFLOAT3 GetRandInUniformInCircle() {
	float Range = 1.0f;
	float Theta = float((Randomfloat(314 * 2) - 314)) / 100.0f;

	DirectX::XMFLOAT3 result;
	result.x = Range * cosf(Theta);
	result.y = Range * sinf(Theta);
	result.z = 0;

	return result;
}

/// <summary>
/// 円状に乱数を生成。均一化有り
/// </summary>
/// <returns></returns>
static DirectX::XMFLOAT3 GetHalfRandInUniformInCircle() {
	DirectX::XMFLOAT3 result = { 0,0,0 };

	while (result.x > 0)
	{
		float Range = sqrtf(float(Randomfloat(1)));
		float Theta = float((Randomfloat(314 * 2) - 314)) / 100.0f;
		DirectX::XMFLOAT3 result;

		result.x = Range * cosf(Theta);
		result.y = Range * sinf(Theta);
		result.z = 0;
	}

	return result;
}

/// <summary>
/// valueのm桁目からn桁目を取得する
/// </summary>
/// <param name="value">取得先となる浮動小数点数</param>
/// <param name="m">取得を開始する桁</param>
/// <param name="n">取得を終了する桁</param>
/// <returns></returns>
static int GetDigits(int value, int m, int n) {
	double mod_value;
	int result;
	// 絶対値を求める
	value = int(fabs(value));
	// n桁目以下の桁を取得
	mod_value = fmod(value, pow(10, n + 1));
	// m桁目以上の桁を取得
	result = int(mod_value / pow(10, m));

	return result;
}