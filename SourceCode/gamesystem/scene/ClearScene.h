#pragma once
#include "IKESprite.h"
#include "BaseScene.h"
#include "EnemyManager.h"
/// �^�C�g���V�[��
class ClearScene : public BaseScene {
public:
	~ClearScene() override {};
public:
	/// ������
	void Initialize(DirectXCommon* dxCommon) override;
	/// �I��
	void Finalize() override;
	/// ���t���[���X�V
	void Update(DirectXCommon* dxCommon) override;
	/// �`��
	void Draw(DirectXCommon* dxCommon) override;

private:
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);
private:
	void MoveSpotLight();
	void SpotSet(XMFLOAT3& Pos, const XMFLOAT3& AfterPos, const float AddFrame);


private:
	static const int SPOT_NUM = 2;
private://�����o�ϐ�
	unique_ptr<IKESprite> sprite;
	struct ShineEffect {
		unique_ptr<IKESprite> tex;
		float frame = 0.f;
		float kFrame = 30.f;
		XMFLOAT2 position = { 0.f,0.f };
		XMFLOAT2 size = { 0.f,0.f };
		bool isVanish = false;
	};

	void RandShineInit();
	void ShineEffectUpdate();
	std::list<ShineEffect> shines;
	unique_ptr<EnemyManager> enemyManager = nullptr;

	//���̓I�Ɏg��
	float m_Angle[SPOT_NUM] = {};
	float m_Angle2[SPOT_NUM] = {};
	float m_AddAngleX[SPOT_NUM] = {};
	float m_AddAngleZ[SPOT_NUM] = {};

	float m_AddPos = {};

	int m_AppTimer = 0;


	enum AppState {
		APP_START,
		APP_NOTICE,
		APP_VANISH,
		APP_END,
	}_AppState = APP_START;

	XMFLOAT2 m_ClearSpritePos = { 0.0f,-1280.0f };
};

