#pragma once
#include "DirectXMath.h"


using namespace std;         //  名前空間指定
//ラスボスのステートを管理するクラス
class LastBossState {
public:
	static LastBossState* GetInstance();
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//settergetter
	const bool GetBossShield() { return m_BossShield; }
	const int GetLastWidth() { return m_LastWidth; }
	const int GetLastHeight() { return m_LastHeight; }
	const XMFLOAT3& GetBossPos() { return m_BossPos; }
	void SetBossSpace(const int width, const int height) { m_LastWidth = width, m_LastHeight = height; }
	void SetBossPos(const XMFLOAT3& pos) { m_BossPos = pos; }
	void SetBossShield(const bool shield) { m_BossShield = shield; }
private:
	//ボスが居るマス
	int m_LastWidth = {};
	int m_LastHeight = {};
	XMFLOAT3 m_BossPos = {};
	bool m_BossShield = false;
};