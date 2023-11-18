#pragma once
#include "InterAction.h"

class SkillAction :
	public InterAction {
public:
	SkillAction(const int SkillID);
	bool Initialize() override;//初期化
	void Pause() override;//ポーズ

	void Draw(DirectXCommon* dxCommon) override;//描画
public:

private:
	void Action() override;//行動

	void ImGui_Origin() override;

	//エフェクト
	void EffecttexDraw(DirectXCommon* dxCommon) override;

	void GetData();
private:
	XMMATRIX m_MatView = {};
	XMMATRIX m_MatProjection = {};
	XMMATRIX m_MatPort = {};
};