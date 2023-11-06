#include "Font.h"
#include<wchar.h>
#include <Helper.h>

using namespace Microsoft::WRL;

DirectX::GraphicsMemory* Font::m_GraphicsMemory = nullptr;
DirectXCommon* Font::m_DirectXCommon = nullptr;

ID3D12Device* Font::device = nullptr;
ID3D12GraphicsCommandList* Font::cmdList = nullptr;
ComPtr<ID3D12DescriptorHeap> Font::descHeap=nullptr;
std::unique_ptr<DirectX::SpriteFont> Font::m_SpriteFont = nullptr;
std::unique_ptr<DirectX::SpriteBatch> Font::m_SpriteBatch = nullptr;

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

}

void Font::LoadFont() {
	DirectX::ResourceUploadBatch resUploadBatch(device);

	resUploadBatch.Begin();
	DirectX::RenderTargetState rtState(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT);
	DirectX::SpriteBatchPipelineStateDescription pd(rtState);

	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(device, resUploadBatch, pd);

	descHeap = m_DirectXCommon->CreateDescriptorHeapForSproteFont();
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(device,
		resUploadBatch,
		L"Resources/font/newUD.spritefont",
		descHeap->GetCPUDescriptorHandleForHeapStart(),
		descHeap->GetGPUDescriptorHandleForHeapStart());

	auto future = resUploadBatch.End(m_DirectXCommon->GetQue());
	cmdList->SetDescriptorHeaps(1, descHeap.GetAddressOf());
	future.wait();
	m_SpriteBatch->SetViewport(m_DirectXCommon->GetViewPort());
}

bool Font::StaticInitialize(DirectXCommon* dxcommon, ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) {
	// nullptrチェック
	assert(dxcommon);
	assert(device);
	assert(cmdList);

	Font::m_DirectXCommon = dxcommon;
	Font::device = device;
	Font::cmdList = cmdList;

	m_GraphicsMemory = new DirectX::GraphicsMemory(device);

	LoadFont();

	return true;
}

void Font::Draw() {
	FlowText();
	cmdList->SetDescriptorHeaps(1, descHeap.GetAddressOf());
	m_SpriteBatch->Begin(cmdList);
	XMFLOAT2 shadow_pos = {
		m_Position.x - 2.f,
		m_Position.y - 2.f
	};
	m_SpriteFont->DrawString(m_SpriteBatch.get(), m_Word,
		shadow_pos, DirectX::Colors::Black);

	m_SpriteFont->DrawString(m_SpriteBatch.get(), m_Word,
		m_Position, m_Color);
	m_SpriteBatch->End();
}


void Font::PostDraw() {
	m_GraphicsMemory->Commit(m_DirectXCommon->GetQue());
}

void Font::SetString(wchar_t* word) {
	m_StartFlag = true;
	m_Word = word;
}

bool Font::FlowText() {
	static float time = 0.f;
	if (m_StartFlag == true) {
		m_NowWord_ = m_Word;
		m_Length = wcslen(m_NowWord_);
		m_StartFlag = false;
	}
	if (m_Length <= 0) {
		//テキスト終了
		m_FinishFlag = true;
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
