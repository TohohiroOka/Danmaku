#pragma once

template<class T>
class Singleton
{
public:

	static inline void CreateInstance() {
		if (!instance) {
			instance = new T;
		}
	}

	static inline T* GetInstance(){
		return instance;
	}

	static inline void Destroy() {
		delete instance;
		instance = nullptr;
	}

protected:
	Singleton() {} // �O���ł̃C���X�^���X�쐬�͋֎~
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) {} // ������Z�q�֎~
	Singleton(const Singleton& obj) {} // �R�s�[�R���X�g���N�^�֎~

private:

	static T* instance;
};

template <typename T> T* Singleton<T>::instance = nullptr;