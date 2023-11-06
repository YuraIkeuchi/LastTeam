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
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:

	Font(wchar_t* word, const XMFLOAT2& position, const XMVECTOR& color = { 1.f,1.f,1.f,1.f });

	~Font();

	void Initialize();

	void Draw();

	static void LoadFont();

	static bool StaticInitialize(DirectXCommon* dxcommon,ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);



	
	static void PostDraw();

	/// <summary>
	/// �����e�L�X�g
	/// </summary>
	/// <returns>�e�L�X�g�I��</returns>
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

	bool GetFinish() {
		return m_FinishFlag;
	}

private:
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//�O���t�B�b�N
	static DirectX::GraphicsMemory* m_GraphicsMemory;
	//�_�C���N�g�G�b�N�X�R����
	static DirectXCommon* m_DirectXCommon;
	//
	static std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;
	static std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;

private:
	//ComPtr<ID3D12DescriptorHeap> m_DescriptHeap = nullptr;
	//����
	wchar_t* m_Word = L"Hello World!";
	wchar_t* m_NowWord_ = L"NULL";

	//�����̒���
	size_t m_Length = wcslen(m_Word);
	//�����̊J�n
	bool m_StartFlag = true;
	//�������I�����
	bool m_FinishFlag = false;
	//�|�W�V����
	XMFLOAT2 m_Position = { 0.f,0.f };
	//�J���[
	XMVECTOR m_Color = { 1.0f,1.0f,1.0f,1.0f };
};

