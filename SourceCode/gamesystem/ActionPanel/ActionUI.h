#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>
#include "DrawNumber.h"
using namespace std;         //  ���O��Ԏw��

class ActionUI {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	ActionUI();

	//������
	void Initialize();
	//�X�e�[�^�X�Z�b�g
	void InitState(const int ActCount);
	//�X�V
	void Update();
	//UI�̓���
	void UiMove();
	//�`��
	void Draw();
	//ImGui
	void ImGuiDraw();
public:
	//gettersetter 
	const bool GetAlive() { return m_Alive; }
	const bool GetUse() { return m_Use; }
	void SetActCount(const int ActCount) { m_ActCount = ActCount; }
	void SetUse(const bool Use) { m_Use = Use; }
	void SetID(const int ID) { m_ID = ID; }
	void SetPosition(XMFLOAT2 pos) {
		m_Position = pos;
		tex->SetPosition(m_Position);
		_drawnumber->SetPosition(m_Position);
	}
private:
	static const int TEX_MAX = 3;
private:
	unique_ptr<IKESprite> tex;

	XMFLOAT2 m_Position = {};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	int m_ActCount = {};

	int m_ActType = {};
	bool m_Use = false;
	float m_Frame = {};
	bool m_Alive = true;

	int m_ID = {};
	unique_ptr<DrawNumber> _drawnumber;
};