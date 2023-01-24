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
/// �����擾
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
static unsigned GetDigit(unsigned num) {
	return unsigned(std::to_string(num).length());
}

/// <summary>
/// ��_�Ԃ̊p�x�擾
/// </summary>
/// <param name="_startPoint">���W1</param>
/// <param name="_endPoint">���W2</param>
/// <returns></returns>
static float GetAngle(const DirectX::XMFLOAT2& _startPoint, const DirectX::XMFLOAT2& _endPoint)
{
	float radian = atan2f(_endPoint.y - _startPoint.y, _endPoint.x - _startPoint.x);
	return DirectX::XMConvertToDegrees(radian);
}

/// <summary>
/// ������Ԃ�
/// </summary>
/// <param name="A">���l</param>
/// <returns></returns>
static int GetSign(float A) {
	if (A > 0) return 1;
	else if (A < 0) return -1;
	else return 0;
}

/// <summary>
/// ���������̌����o��
/// </summary>
/// <param name="value">�l</param>
/// <returns>���������̌���</returns>
static int GetDigit(float value)
{
	std::ostringstream stream;
	stream << value;
	std::string str = stream.str();// ������ɕϊ�
	const int str_line_max= int(str.size());

	int integer = 0;// �������̌���
	
	for (int i = 0; i < str_line_max; i++)
	{
		// �����̂Ƃ�
		if (str[i] == '.')
		{
			// �����_�̈ʒu��ۑ�
			integer = i;
			break;
		}
	}

	//������̍��v-���������ŏ��������̌������o��
	int count=0;
	if (integer != 0)
	{
		count = str_line_max - integer - 1;
	}

	return count;
}

/// <summary>
/// ��������
/// 0����͈͂܂ł̗������o��
/// </summary>
/// <param name="range">�͈�</param>
/// <returns>����</returns>
static float Randomfloat(float _range)
{
	const int str_count= GetDigit(_range);

	//�����ɖ߂����߂̔{��
	int magnification=1;
	for (int i = 0; i < str_count; i++) {
		magnification*=10;
	}

	//�����ł̒l
	const int int_range= int(_range * magnification);

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, int_range);

	return float(rand100(mt))/ float(magnification);
}

/// <summary>
/// ��������
/// 0����͈͂܂ł̗������o��
/// </summary>
/// <param name="range">�͈�</param>
/// <returns>����</returns>
static int Randomint(int _range)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, _range);

	return rand100(mt);
}

/// <summary>
/// ��������
/// 1��-1���o��
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
/// �~��ɗ����𐶐��B�ψꉻ�L��
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
/// �~��ɗ����𐶐��B�ψꉻ�L��
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
/// value��m���ڂ���n���ڂ��擾����
/// </summary>
/// <param name="value">�擾��ƂȂ镂�������_��</param>
/// <param name="m">�擾���J�n���錅</param>
/// <param name="n">�擾���I�����錅</param>
/// <returns></returns>
static int GetDigits(int value, int m, int n) {
	double mod_value;
	int result;
	// ��Βl�����߂�
	value = int(fabs(value));
	// n���ڈȉ��̌����擾
	mod_value = fmod(value, pow(10, n + 1));
	// m���ڈȏ�̌����擾
	result = int(mod_value / pow(10, m));

	return result;
}