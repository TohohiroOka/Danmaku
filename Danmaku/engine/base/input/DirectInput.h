#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <DirectXMath.h>
#include "Singleton.h"

class DirectInput : public Singleton <DirectInput>
{
public:
	friend class Singleton<DirectInput>; // Singleton �ł̃C���X�^���X�쐬�͋���

public: //�G�C���A�X
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

protected:

	DirectInput() {};
	virtual ~DirectInput();

public: //��
	//�}�E�X�{�^��
	enum MOUSE_BUTTON
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_WHEEL
	};

public: //�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name = "_keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(const BYTE& _keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name = "_keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(const BYTE& _keyNumber);

	/// <summary>
	/// �L�[�𗣂������`�F�b�N
	/// </summary>
	/// <param name = "_keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>��������</returns>
	bool ReleaseKey(const BYTE& _keyNumber);

	/// <summary>
	/// �}�E�X�{�^���̉������`�F�b�N
	/// </summary>
	/// <param name = "_mouseButton">�}�E�X�{�^��</param>
	/// <returns>������Ă��邩</returns>
	bool PushMouseButton(int _mouseButton);

	/// <summary>
	/// �}�E�X�{�^���̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name = "_mouseButton">�}�E�X�{�^��</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseButton(int _mouseButton);

	/// <summary>
	/// �}�E�X�{�^���𗣂������`�F�b�N
	/// </summary>
	/// <param name = "_mouseButton">�}�E�X�{�^��</param>
	/// <returns>��������</returns>
	bool ReleaseMouseButton(int _mouseButton);

	/// <summary>
	/// �}�E�X�|�C���^�[�̍��W���擾
	/// </summary>
	/// <returns>�}�E�X�|�C���^�[�̍��W</returns>
	DirectX::XMFLOAT2 GetMousePoint();

	/// <summary>
	/// 1�t���[���̃}�E�X�̈ړ��ʂ̎擾
	/// </summary>
	/// <returns>1�t���[���̃}�E�X�̈ړ���</returns>
	DirectX::XMFLOAT2 GetMouseVelocity();

	/// <summary>
	/// 1�t���[���̃}�E�X�z�C�[���̈ړ��ʂ̎擾
	/// </summary>
	/// <returns>1�t���[���̃}�E�X�z�C�[���̈ړ���</returns>
	float GetMouseWheelVelocity();

private: //�����o�ϐ�
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};
	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devMouse;
	//�}�E�X�̏��
	DIMOUSESTATE mouse;
	//�O��̃}�E�X�̏��
	DIMOUSESTATE mousePre;
	//�}�E�X�|�C���^�[
	POINT mousePoint;
};