#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include <string>
using namespace std;         //  名前空間指定

class InterAction :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:

public:
	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	/// ポーズのとき
	virtual void Pause() = 0;
	//描画
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();//ImGuiの描画

	virtual void EffecttexDraw(DirectXCommon* dxCommon) = 0;

	void Collide();
protected:

	virtual void Action() = 0;//ボス特有の処理

	virtual void ImGui_Origin() = 0;//ボスそれぞれのImGui
public:
	//gettersetter
	bool GetAlive() { return m_Alive; }
protected:
	string m_Tag;
	float m_Radius = 2.0f;
	bool m_Alive = true;
};