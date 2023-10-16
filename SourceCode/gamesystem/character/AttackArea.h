#pragma once
#include"DirectXCommon.h"
#include "ObjCommon.h"
#include <DirectXMath.h>

using namespace std;         //  名前空間指定

//攻撃エリアクラス
class AttackArea :
public ObjCommon{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	AttackArea();
public:
	//初期化
	bool Initialize()override;
	//ステータス初期化
	void InitState(const int width, const int height);
	//更新
	void Update()override;
	//描画
	void Draw(DirectXCommon* dxCommon)override;
	//ImGui
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);

public:
	//gettersetter
	const bool GetHit() { return m_Hit; }
	const bool GetAlive() { return m_Alive; }
	void SetHit(const bool Hit) { m_Hit = Hit; }
private:
	//生存関係
	bool m_Alive = false;
	int m_AliveTimer = {};

	//ヒットしたかどうか
	bool m_Hit = false;
};