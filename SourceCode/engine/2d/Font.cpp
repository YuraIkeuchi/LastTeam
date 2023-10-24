#include "Font.h"
#include<wchar.h>
#include <Helper.h>

DirectX::GraphicsMemory* Font::_gmemory = nullptr;
DirectXCommon* Font::dxCommon = nullptr;
Font::Font(wchar_t* word, const XMFLOAT2& position, const XMVECTOR& color) {
	m_StartFlag = true;
	m_Word = word;
	m_Length = wcslen(m_Word);
	m_Position = position;
	m_Color = color;
	Initialize();
}

Font::~Font() {

}

void Font::Initialize() {
	LoadFont();

}

void Font::LoadFont() {
	DirectX::ResourceUploadBatch resUploadBatch(dxCommon->GetDev());

	resUploadBatch.Begin();
	DirectX::RenderTargetState rtState(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT);
	DirectX::SpriteBatchPipelineStateDescription pd(rtState);

	_spritebatch = std::make_unique<DirectX::SpriteBatch>(dxCommon->GetDev(), resUploadBatch, pd);

	_heapForSpriteFont = dxCommon->CreateDescriptorHeapForSproteFont();
	_spritefont = std::make_unique<DirectX::SpriteFont>(dxCommon->GetDev(),
		resUploadBatch,
		L"Resources/font/keifont.spritefont",
		_heapForSpriteFont->GetCPUDescriptorHandleForHeapStart(),
		_heapForSpriteFont->GetGPUDescriptorHandleForHeapStart());

	auto future = resUploadBatch.End(dxCommon->GetQue());
	dxCommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	future.wait();
	_spritebatch->SetViewport(dxCommon->GetViewPort());
}

void Font::Draw() {
	dxCommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	_spritebatch->Begin(dxCommon->GetCmdList());
	XMFLOAT2 shadow_pos = {
		m_Position.x - 2.f,
		m_Position.y - 2.f
	};
	_spritefont->DrawString(_spritebatch.get(), m_Word,
		shadow_pos, DirectX::Colors::Black);

	_spritefont->DrawString(_spritebatch.get(), m_Word,
		m_Position, m_Color);
	_spritebatch->End();
}

void Font::SetGraphicMemory(DirectXCommon* dxcommon) {
	dxCommon = dxcommon;
	_gmemory = new DirectX::GraphicsMemory(dxCommon->GetDev());
}

void Font::PostDraw() {
	_gmemory->Commit(dxCommon->GetQue());
}

void Font::SetString(wchar_t* word) {
	m_StartFlag = true;
	m_Word = word;
}

bool Font::FlowText() {
	static float time = 0.f;
	static wchar_t* m_NowWord_ = L"NULL";
	if (m_StartFlag == true) {
		m_NowWord_ = m_Word;
		m_Length = wcslen(m_NowWord_);
		m_StartFlag = false;
	}
	if (m_Length <= 0) {
		//テキスト終了
		return  true;
	} else {
		if (Helper::GetInstance()->FrameCheck(time, 1.0f)) {
			m_Length -= 1;
			Helper::GetInstance()->Clamp(m_Length, (size_t)0, (size_t)20);
			time = 0.f;
		}
		size_t newSize = wcslen(m_NowWord_);
		size_t origsize = wcslen(m_NowWord_) + 1;
		wchar_t* wcstr = new wchar_t[newSize];
		wcsncpy_s(wcstr, origsize, m_NowWord_, newSize - m_Length);
		m_Word = wcstr;
		return false;
	}
}
