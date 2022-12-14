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
	Singleton() {} // 外部でのインスタンス作成は禁止
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) {} // 代入演算子禁止
	Singleton(const Singleton& obj) {} // コピーコンストラクタ禁止

private:

	static T* instance;
};

template <typename T> T* Singleton<T>::instance = nullptr;