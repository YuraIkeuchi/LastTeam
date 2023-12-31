#pragma once
#include "IKESprite.h"
#include <vector>
#include <DirectXMath.h>
#include <array>
#include <memory>
using namespace DirectX;
using namespace std;

/// 数字描画クラス
class DrawNumber
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	DrawNumber(const float scale);
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//数字の指定とか座標とか
	void SetExplain(const XMFLOAT3& pos);
	//変換の場合カメラデータをもらう
	void GetCameraData();
public:
	void SetPosition(const XMFLOAT2& Position) { m_Position = Position; }
	void SetNumber(const int Number) { m_Number = Number; }
	void SetColor(XMFLOAT4 color);
	void SetSize(XMFLOAT2 size);
	int GetNumber() { return m_Number; }

private:
	static const int NUMBER_MAX = 10;
private:
	//Hpの表示(とりあえず三桁)
	array<unique_ptr<IKESprite>, NUMBER_MAX> _Number;
	XMFLOAT2 m_Position = { 0.0f,0.0f };

	//カメラデータのため
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};

	//描画する数字
	int m_Number = {};
};