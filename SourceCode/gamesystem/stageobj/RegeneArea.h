#pragma once
#include"DirectXCommon.h"
#include "IKETexture.h"
#include <DirectXMath.h>

using namespace std;         //  ���O��Ԏw��

//�U���G���A�N���X
class RegeneArea{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	RegeneArea();
public:
	//������
	bool Initialize();
	//�X�e�[�^�X������
	void InitState(const int width, const int height);
	//�X�V
	void Update();
	//�`��
	void Draw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
private:
	XMFLOAT3 SetPanelPos(const int width, const int height);
public:
	const bool GetAlive() { return m_Alive; }

	const int GetNowWidth() { return m_NowWidth; }
	const int GetNowHeight() { return m_NowHeight; }
	const int GetTimer() { return m_Timer; }

	void SetTimer(const int Timer) { m_Timer = Timer; }
private:
	//�p�l��
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool predict = false;
	};
	Panel panels;
	//�����֌W
	bool m_Alive = false;
	int m_BirthTimer = 0;
	int m_Timer = {};
	int m_NowWidth = {};
	int m_NowHeight = {};
};