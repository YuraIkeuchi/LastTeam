#pragma once
#include"DirectXCommon.h"
#include"VariableCommon.h"
#include"Font.h"
#include<memory>
#include <map>
class FontManager {
public:
	void Initialize();
	void Create(wchar_t* word_ = L"Hello", XMFLOAT2 pos_ = { 0.f,0.f }, XMVECTOR color_ = { 1.f,1.f,1.f,1.f });


	void SetDxCommon(DirectXCommon* dxcommon) { this->dxcommon = dxcommon; }
private:
	struct Sentense {
		std::unique_ptr<Font> font;
		wchar_t* word = L" ";
		XMFLOAT2 position = { 0.f,0.f };
		XMVECTOR color = { 1.f,1.f,1.f,1.f };

	};
private:
	DirectXCommon* dxcommon = nullptr;
	std::list<Sentense> fonts;
};

