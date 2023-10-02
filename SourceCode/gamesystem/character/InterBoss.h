#pragma once
#include "IKETexture.h"
#include "CollisionPrimitive.h"
#include "ObjCommon.h"
#include "InterEnemy.h"

using namespace std;         //  名前空間指定

//ボスの基底クラス
class InterBoss :
	public ObjCommon {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//初期化
	virtual bool Initialize() = 0;
	//更新
	void Update();
	//描画
	virtual void Draw(DirectXCommon* dxCommon)override;

	void ImGuiDraw();//ImGuiの描画

protected:
	virtual void Action() = 0;//ボス特有の処理
};

