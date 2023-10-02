#pragma once
#include <DirectXMath.h>

using namespace DirectX;
struct PlayerHitState {
	XMFLOAT3& pos;
	XMFLOAT2 radius = { 0.0f,0.0f };
	XMFLOAT3& old_pos;
	bool is_jump = false;
	float addPower = 0.0f;
	int JumpCount = 0;
};