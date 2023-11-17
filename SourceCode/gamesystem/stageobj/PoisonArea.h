#pragma once
#include"DirectXCommon.h"
#include <DirectXMath.h>
#include "IKETexture.h"
#include "ObjCommon.h"
#include "Player.h"
#include "PredictArea.h"
using namespace std;         //  ���O��Ԏw��

//�U���G���A�N���X
class PoisonArea : public ObjCommon{
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	PoisonArea();
public:
	//������
	bool Initialize() override;
	//�X�e�[�^�X������
	void InitState(const int width, const int height);
	//�X�V
	void Update() override;
	//�`��
	void Draw(DirectXCommon* dxCommon) override;
	//ImGui
	void ImGuiDraw();
private:
	void Move();
	void Collide();
	XMFLOAT3 SetPanelPos(const int width, const int height);
	void BirthPredict(const int Width, const int Height);//�\���G���A
public:
	//gettersetter
	const bool GetAlive() { return panels.Alive; }
	void SetPlayer(Player* player) { this->player = player; }
private:
	
	Player* player;
	//�p�l��
	struct Panel {
		unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool Alive = false;
		int Width = {};
		int Height = {};
		int Timer = {};
		float scale = {};
		float afterscale = 0.2f;
		float frame = {};
		int DamageTimer = 49;
		bool Damage = false;
	};

	Panel panels;

	enum PoisonState{
		POISON_THROW,
		POISON_DROP,
		POISON_WIDE,
		POISON_END,
	}_PoisonState = POISON_THROW;
	bool m_BulletAlive = false;

	struct PredictState {
		unique_ptr<PredictArea> area;
		int Timer = {};
		int Target = 200;
	};

	PredictState predict;
};