#pragma once
#include "TextManager.h"
#include <vector>
#include <DirectXCommon.h>
#include <string>

/// <summary>
/// �p�b�V�u�̃e�L�X�g���m�F���邾���̃N���X
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

