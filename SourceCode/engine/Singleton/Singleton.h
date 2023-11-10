#pragma once

#include <mutex>
#include <assert.h>

class SingletonFinalizer
{
public:
	using FinalizerFunc = void(*)();
	static void AddFinalizer(FinalizerFunc func);
	static void Finalize();
};

template <typename T>
class Singleton
{
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static T &GetInstance()
	{
		return *instance_;
	}

	/// <summary>
	/// 生成
	/// </summary>
	static T *CreateSingleton()
	{
		if (instance_ == nullptr)
		{
			instance_ = new T;
			SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);
			if (!instance_->StartUp())
			{
				assert(0);
			}
		}
		else
		{

		}
		return instance_;
	}

protected:
	Singleton() = default;			// 外部からの生成禁止
	virtual ~Singleton() = default;	// 外部からの破棄禁止

	virtual bool StartUp() { return true; };	// コンストラクタの後に実行されるもの

private:
	void operator=(const Singleton &singleton) {};	// 代入演算禁止
	//Singlton(const Singleton &singleton) {};			// コピーコンストラクタ禁止



	/// <summary>
	/// 破棄
	/// </summary>
	static void Destroy()
	{
		delete instance_;
		instance_ = nullptr;
	}

	static T *instance_;				// インスタンス
};

template <typename T> T *Singleton<T>::instance_ = nullptr;