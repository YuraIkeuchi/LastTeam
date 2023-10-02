#pragma once
#include "Framework.h"
#include"SceneManager.h"

/// ゲームシーン
class IkeGame : public Framework
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private: // 静的メンバ変数
	static const int debugTextTexNumber = 0;

public: // メンバ関数

	//コンストクラタ
	IkeGame();

	//デストラクタ
	~IkeGame();

	//初期化
	void Initialize(DirectXCommon* dxCommon) override;

	//毎フレーム処理
	void Update(DirectXCommon* dxCommon) override;

	//描画
	void Draw(DirectXCommon* dxCommon) override;

	//終了
	void Finalize() override;


private://メンバ変数

};