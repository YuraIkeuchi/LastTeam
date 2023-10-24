#include "Font.h"
#include<wchar.h>
#include <Helper.h>

DirectX::GraphicsMemory* Font::_gmemory = nullptr;

Font::~Font() {
	/*delete _spritefont;
	delete _spritebatch;*/
}

void Font::Initialize(DirectXCommon* dxcommon) {
	_gmemory = new DirectX::GraphicsMemory(dxcommon->GetDev());
}

void Font::LoadFont(DirectXCommon* dxcommon) {
	DirectX::ResourceUploadBatch resUploadBatch(dxcommon->GetDev());

	resUploadBatch.Begin();
	DirectX::RenderTargetState rtState(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT);
	DirectX::SpriteBatchPipelineStateDescription pd(rtState);

	_spritebatch = new DirectX::SpriteBatch(dxcommon->GetDev(), resUploadBatch, pd);

	_heapForSpriteFont = dxcommon->CreateDescriptorHeapForSproteFont();
	_spritefont = new DirectX::SpriteFont(dxcommon->GetDev(),
		resUploadBatch,
		L"Resources/font/keifont.spritefont",
		_heapForSpriteFont->GetCPUDescriptorHandleForHeapStart(),
		_heapForSpriteFont->GetGPUDescriptorHandleForHeapStart());

	auto future = resUploadBatch.End(dxcommon->GetQue());
	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	future.wait();
	_spritebatch->SetViewport(dxcommon->GetViewPort());
}


void Font::StringReset() {
	m_Word = L" ";
}

void Font::Draw(DirectXCommon* dxcommon) {
	dxcommon->GetCmdList()->SetDescriptorHeaps(1, _heapForSpriteFont.GetAddressOf());
	_spritebatch->Begin(dxcommon->GetCmdList());
	_spritefont->DrawString(_spritebatch, m_Word,
		disply_place_, DirectX::Colors::Black, {}, shadow_position_);

	_spritefont->DrawString(_spritebatch, m_Word,
		disply_place_, color_, {}, position_);
	_spritebatch->End();
}

void Font::PostDraw(DirectXCommon* dxcommon) {
	_gmemory->Commit(dxcommon->GetQue());
}

void Font::SetString(wchar_t* ward) {
	m_StartFlag = true;
	m_Word = ward;
}

bool Font::FlowText() {
	if (m_StartFlag == true) {
		m_NowWord_ = m_Word;
		len_ = wcslen(m_NowWord_);
		m_StartFlag = false;
	}
	if (len_ == 0) {
		//テキスト終了
		return  true;
	} else {
		if (Helper::GetInstance()->FrameCheck(time_, 1.0f)) {
			len_ -= 1;
			Helper::GetInstance()->Clamp(len_,(size_t)0,(size_t)20);
			time_ = 0.f;
		}
		size_t newSize = wcslen(m_NowWord_);
		size_t origsize = wcslen(m_NowWord_) + 1;
		wchar_t* wcstr = new wchar_t[newSize];
		wcsncpy_s(wcstr, origsize, m_NowWord_, newSize - len_);
		m_Word = wcstr;
		return false;
	}
}
