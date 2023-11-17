#pragma once
#include "IKESprite.h"
#include <vector>
#include <DirectXMath.h>
#include <array>
#include <memory>
using namespace DirectX;
using namespace std;

/// 数字描画クラス
class DrawDamageNumber
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
	DrawDamageNumber();
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//ImGUi
	void ImGuiDraw();
	//数字の指定とか座標とか
	void SetExplain(const XMFLOAT3& pos);
	//変換の場合カメラデータをもらう
	void GetCameraData();

private:
	void NumberMove();
public:
	const bool GetAlive() { return m_Alive; }
	void SetPosition(const XMFLOAT2& Position) { m_Position = Position; }
	void SetNumber(const int Number) { m_Number = Number; }
private:
private:
	static const int NUMBER_MAX = 10;
private:
	
	//Hpの表示(とりあえず三桁)
	array<unique_ptr<IKESprite>, NUMBER_MAX> _Number;
	XMFLOAT2 m_Position = { 0.0f,0.0f };
	XMFLOAT2 m_AfterPos = {};
	XMFLOAT2 m_Size = { 64.0f,64.0f };
	float m_AfterSize = {};
	//カメラデータのため
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};

	//描画する数字
	int m_Number = {};
	float m_Frame = {};
	bool m_Alive = false;
};