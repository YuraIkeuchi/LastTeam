#pragma once
#include"Font.h"
#include"DirectXCommon.h"
#include"Helper.h"
#include<DirectXMath.h>

using namespace std;

class Text
{
private:
	enum class state {
		kStartTalk=0,
		kStart
	};
private:
	// DirectX::Çè»ó™
		// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	void Initialize(DirectXCommon* dxcommon,const XMVECTOR& color={1.f,1.f,1.f,1.f});
	
	void Update(wchar_t* firstrow,wchar_t* secondrow,wchar_t* thirdrow);

	void Draw(DirectXCommon* dxcommon);

	
private:
	unique_ptr<Font> firstrow_;
	unique_ptr<Font> secondrow_;
	unique_ptr<Font> thirdrow_;

	wchar_t* ward[3];
};

