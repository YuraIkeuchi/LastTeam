#pragma once
#include "Easing.h"
#include "IKESprite.h"
#include <memory>
#include <array>

using namespace std;         //  ���O��Ԏw��
using namespace DirectX;
//�V�[���`�F���W�N���X
class SceneChanger {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	static SceneChanger* GetInstance();

	/// <summary>
	/// ����k����
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// ����k����
	/// </summary>
	void InitializeOver();
	//�`��
	void Draw();
	//ImGui
	void ImGuiDraw();

private:
	static void (SceneChanger::* stateTable[])();

	void FeedChange();
	void WideChange();
	XMFLOAT4 ChangeColor(const int Weight, const int Height);
private:
	static const int DOOR_NUM = 2;
public:
	const bool GetChange() { return m_Change; }
	const bool GetChangeStart() { return m_ChangeStart; }
	void SetChange(const bool Change) { m_Change = Change; }
	void SetChangeStart(const bool ChangeStart) { m_ChangeStart = ChangeStart; }

private:
	static const int HEIGHT_NUM = 9;
	static const int WIDTH_NUM = 16;
private:
	//�V�[���`�F���W�̃^�C�v
	enum ChangeType {
		CHANGE_FEED,
		CHANGE_WIDE,
	}_ChangeType = CHANGE_WIDE;

	//���ʂ̃t�F�[�h�p
	unique_ptr<IKESprite> change;
	float m_Frame = {};

	bool m_ChangeStart = false;
	bool m_Change = false;
	int m_ChangeTimer = {};

	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,0.0f };
	float m_AfterAlpha = {};

	//�L����悤
	const float width = WinApp::window_width;
	const float height = WinApp::window_height;
	const int base_size = 80;

	unique_ptr<IKESprite> change2[WIDTH_NUM][HEIGHT_NUM];
	XMFLOAT4 m_Color2[WIDTH_NUM][HEIGHT_NUM];
	
	XMFLOAT2 m_WideSize = {};
	float m_AfterSize = {};
	//�V�[���`�F���W���
	enum CHANGESTATE {
		CHANGE_START,
		CHANGE_FINISH
	}_ChangeState;

public:
	int GetChangeState() { return _ChangeState; }
};