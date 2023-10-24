#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include <string>
#include <IKETexture.h>
using namespace std;         //  名前空間指定
enum State {
	STATE_SPAWN = 0,
	STATE_ALIVE,
	STATE_VANISH,
};

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
	const bool GetAlive() { return m_Alive; }
	const int GetSkillID() { return m_SkillID; }
	void SetState(const int state) { _state= state; }
	void SetSkillID(const int SkillID) { m_SkillID = SkillID; }

protected:
	string m_Tag;
	unique_ptr<IKETexture> m_Pannel = nullptr;
	XMFLOAT3 m_PannelScale = {0.15f,0.15f,0.15f};
	XMFLOAT3 m_PannelRot = { 90.0f,0.0f,0.0f };
	float m_Radius = 0.35f;
	bool m_Alive = true;

	float m_VanishFrame = 0.f;
	float kVanishMax = 45.f;
	//関数ポインタ
	static void(InterAction::* stateTable[])();
	int _state = STATE_SPAWN;
	int m_SkillID = {};
private:
	void Spawn();//待機
	void Alive();
	void Vanish();

};