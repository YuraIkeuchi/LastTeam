#pragma once
#include "TextManager.h"
#include <vector>
#include <DirectXCommon.h>
#include <string>

/// <summary>
/// パッシブのテキストを確認するだけのクラス
/// </summary>
class PassiveManager {
public:
	static PassiveManager* GetInstance();

	void Initialize(DirectXCommon* dxCommon);

	void LoadText(const int Number);

	void TextDraw(const int Number);
private:
	DirectXCommon* dxcommon;

	struct PassiveText {
		std::unique_ptr<TextManager> text_;
		wchar_t* baseSentence[3] = { L"",L"",L"" };
	};

	std::vector<PassiveText> passiveTexts;

};

