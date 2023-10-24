#pragma once
#include<SpriteFont.h>
#include<ResourceUploadBatch.h>
#include<Windows.h>
#include"DirectXCommon.h"
#include"WinApp.h"
#include<memory>
#pragma comment(lib,"DirectXTK12.lib")

class Font {
protected:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

	Font(wchar_t* word, const XMFLOAT2& position, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	~Font();

	void Initialize();

	void Draw();

	void LoadFont();

	static void SetGraphicMemory(DirectXCommon* dxcommon);
	
	static void PostDraw();

	/// <summary>
	/// 流れるテキスト
	/// </summary>
	/// <returns>テキスト終了</returns>
	bool FlowText();
public:	//Getter,Setter
	void SetString(wchar_t* word);
	wchar_t* GetString() { return m_Word; }
	void SetPos(const XMFLOAT2& position) {
		this->m_Position = position;
	}
	void SetColor(const XMVECTOR& color) {
		this->m_Color = color;
	}
private:
	static DirectX::GraphicsMemory* _gmemory;
	static DirectXCommon* dxCommon;
private:
	std::unique_ptr<DirectX::SpriteFont> _spritefont = nullptr;
	std::unique_ptr<DirectX::SpriteBatch> _spritebatch = nullptr;
	ComPtr<ID3D12DescriptorHeap> _heapForSpriteFont = nullptr;
	//文字
	wchar_t* m_Word = L"Hello World!";
	//文字の長さ
	size_t m_Length = wcslen(m_Word);
	//動きの開始
	bool m_StartFlag = true;
	//ポジション
	XMFLOAT2 m_Position = { 0.f,0.f };
	//カラー
	XMVECTOR m_Color = { 1.0f,1.0f,1.0f,1.0f };
};

