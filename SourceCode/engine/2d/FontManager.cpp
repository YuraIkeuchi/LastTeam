#include "FontManager.h"



void FontManager::Initialize() {
	assert(dxcommon);
}

void FontManager::Create(wchar_t* word_, XMFLOAT2 pos_, XMVECTOR color_) {
	Sentense sentense;
	sentense.font = std::make_unique<Font>();
	sentense.font->LoadFont(dxcommon);

}
