#pragma once
#include "IKESprite.h"
#include <memory>
#include <array>
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
	void InitState(const int ActCount, const string& Tag);
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
	void SetActCount(const int ActCount) { m_ActCount = ActCount; }
	void SetUse(const bool Use) { m_Use = Use; }

private:
	static const int TEX_MAX = 3;
private:
	array<unique_ptr<IKESprite>,TEX_MAX> tex;

	XMFLOAT2 m_Position = {};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	int m_ActCount = {};

	int m_ActType = {};
	bool m_Use = false;
	float m_Frame = {};
	bool m_Alive = true;
};