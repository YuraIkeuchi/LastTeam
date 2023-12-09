#pragma once
#include"DirectXCommon.h"
#include "IKETexture.h"
#include <DirectXMath.h>
#include <ObjCommon.h>

using namespace std;         //  名前空間指定

//攻撃エリアクラス
class AttackArea :
	public ObjCommon {
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	AttackArea(string& m_Name);
public:
	//初期化
	bool Initialize() override;
	//ステータス初期化
	void InitState(const int width, const int height);
	//更新
	void Update() override;
	//描画
	void Draw(DirectXCommon* dxCommon) override;
	//ImGui
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
	void Move();
public:
	//gettersetter
	const bool GetHit() { return m_Hit; }
	const bool GetAlive() { return m_Alive; }
	const bool GetIsFixed() { return isFixed; }
	const string GetName() { return m_Name; }

	const int GetNowWidth() { return m_NowWidth; }
	const int GetNowHeight() { return m_NowHeight; }
	const int GetTimer() { return m_Timer; }
	const float GetDamage() { return m_Damage; }
	const std::string GetStateName() { return StateName; }

	void SetHit(const bool Hit) { m_Hit = Hit; }
	void SetTimer(const int Timer) { m_Timer = Timer; }
	void SetName(const string name) { m_Name = name; }
	void SetDamage(const float Damage) { m_Damage = Damage; }
	void SetIsFixed(bool flag) { isFixed = flag; }
	void SetStateName(const std::string name) { StateName = name; }
private:
	//パネル
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};
	Panel panels;
	string m_Name = "none";
	//生存関係
	bool m_Alive = false;
	int m_BirthTimer = false;
	int m_Timer = {};
	//ヒットしたかどうか
	bool m_Hit = false;
	//他の補正をかけない
	bool isFixed = false;
	int m_NowWidth = {};
	int m_NowHeight = {};
	float m_Damage = {};
	std::string StateName = "NONE";
	//上昇度
	float m_AddPower = 0.0f;
	//重力加速度
	float m_Gravity = 0.02f;
};