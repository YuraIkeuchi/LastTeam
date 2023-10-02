#pragma once
#include"IKESprite.h"
#include <memory>
#include <DirectXMath.h>
using namespace std;         //  名前空間指定
class MessageWindow
{
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void Initialize();

	void Update(const XMFLOAT4& girlcolor , const XMFLOAT4& sutoponcolor );

	void Draw();
};

