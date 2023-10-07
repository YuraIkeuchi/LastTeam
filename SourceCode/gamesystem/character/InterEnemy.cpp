#include "InterEnemy.h"
#include "Collision.h"
#include "Helper.h"
#include "Player.h"
#include <StagePanel.h>
XMFLOAT3 InterEnemy::randPanelPos() {
	int width = Helper::GetInstance()->GetRanNum(4, 7);
	int height = Helper::GetInstance()->GetRanNum(4, 7);
	return StagePanel::GetInstance()->SetPositon(width, height);;
}
//初期化
bool InterEnemy::Initialize() {
	return true;
}
//更新
void InterEnemy::Update() {
	//�s��
	Action();
}
//描画
void InterEnemy::Draw(DirectXCommon* dxCommon) {
}

void InterEnemy::ImGuiDraw() {
	ImGui_Origin();
}