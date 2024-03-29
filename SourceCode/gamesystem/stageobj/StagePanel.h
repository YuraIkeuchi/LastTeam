#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include "SkillAction.h"
#include <any>
#include <array>
#include "IKESprite.h"
#include "IKETexture.h"
#include "Player.h"
#include "Onomatope.h"
using namespace DirectX;
using namespace std;

static const int PANEL_HEIGHT = 4;
static const int PANEL_WIDTH = 8;
static const float PANEL_SIZE = 1.5f;
class StagePanel {
public:
	static StagePanel* GetInstance();
private:
	
	static const int DIR_MAX = 4;

	//パネルの種類
	enum PanelType {
		NO_PANEL = 0,
		ATTACK_PANEL,
		GUARD_PANEL,
		SKILL_PANEL,
		DAMAGE_PANNEL,
	};
	//パネル
	struct Panel {
		unique_ptr<IKEObject3d> object = nullptr;
		unique_ptr<IKETexture> line = nullptr;
		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		int type = NO_PANEL;
		bool isHit = false;
		bool isEnemyHit = false;
		bool predict = false;
		bool isPoison = false;
		int PoisonTimer = {};
		int TargetTimer = {};
		float Frame = {};
		float Disolve = {};
		bool isClose = false;
		bool isHeal = false;
		bool isRock = false;
	};

public:
	void LoadResource();
	//初期化
	bool Initialize(const float PosY = 0.0f);
	//更新
	void Update();
	void CreateStage();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//行動カードの描画
	void ActDraw(DirectXCommon* dxCommon);
	//オノマトペの描画
	void OnomatoDraw();
	//ImGui
	void ImGuiDraw();
	//パネルを戻す
	void DeletePanel();
	bool AllCleanCheack();
	void RandomPanel(int num);
	void ResetPanel();
	void ResetAction();
	void EnemyHitReset();
	void PoisonUpdate();

	//色変更(敵)
	void SetEnemyHit(IKEObject3d* obj,int& width, int& height,bool m_Alive);
	//今いる位置のパネル番号を検索
	void SetPanelSearch(IKEObject3d* obj, int& width, int& height);
	//敵の配置(かぶらないように)
	XMFLOAT3 EnemySetPanel(const bool LastEnemy, const string& name = "NONE");
	//敵の配置(プレイヤーの前に来るやつ)
	XMFLOAT3 FrontPlayerSetPanel();
	void ClosePanel(IKEObject3d* obj, bool m_Alive);
	//毒の配置(かぶらないように)
	void PoisonSetPanel(int& width, int& height);
	//岩の配置(かぶらないように)
	void RockSetPanel(int& width, int& height);

	//アクションの全削除
	void DeleteAction();
private:
	//バトル中の更新
	void BattleUpdate();
	//スキルセットの更新
	void SetUpdate();
	XMFLOAT4 ChangeColor(const int Widht, const int Height);

public:
	static void SetPlayer(Player* player) { StagePanel::player = player; }
	//gettersetter
	XMFLOAT3 SetPositon(int width, int height) {
		return { panels[width][height].position.x,0.0f,panels[width][height].position.z };
	}

	//gettersetter
	bool SetPredict(int width, int height, bool Flag) {
		return panels[width][height].predict = Flag;
	}

	bool SetHeal(int width, int height, bool Flag) {
		return panels[width][height].isHeal = Flag;
	}

	bool SetClose(int width, int height, bool Flag) {
		return panels[width][height].isClose = Flag;
	}

	bool SetRock(int width, int height, bool Flag) {
		return panels[width][height].isRock = Flag;
	}

	const bool GetAllDelete() { return m_AllDelete; }
	const bool GetCreateFinish() { return m_CreateFinish; }
	const int GetPanelType(const int width, const int height) { return panels[width][height].type; }
	const XMFLOAT3& GetPosition(const int width, const int height) { return panels[width][height].position; }
	const int GetisEnemyHit(const int width, const int height) { return panels[width][height].isEnemyHit; }
	const int GetClose(const int width, const int height) { return panels[width][height].isClose; }
	const int GetHeal(const int width, const int height) { return panels[width][height].isHeal; }
	void SetCreateFinish(const bool CreateFinish) { m_CreateFinish = CreateFinish; }
private:
	static Player* player;
	unique_ptr<Onomatope> onomatope = nullptr;
	//パネル
	Panel panels[PANEL_WIDTH][PANEL_HEIGHT];
	//スキル
	vector<unique_ptr<InterAction>> actions;


	//マスの位置
	int m_SelectHeight = 0;
	int m_SelectWidth = 0;

	//方向
	enum SelectDir {
		DIR_UP,
		DIR_DOWN,
		DIR_RIGHT,
		DIR_LEFT
	};

	//各方向入力フレーム
	array<int, DIR_MAX> m_Timer;

	//XMFLOAT3 m_SelectPos = {};

	OBB m_OBB1 = {}, m_OBB2 = {};

	//スキルをすべて手に入れているか
	bool m_AllDelete = false;
	int m_ActionCount = {};

	int m_CreateTimer = {};
	bool m_CreateFinish = false;
};