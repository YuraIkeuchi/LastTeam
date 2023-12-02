#pragma once
#include "ObjCommon.h"
#include "CollisionPrimitive.h"
#include "SkillAction.h"
#include <any>
#include <array>
#include "IKESprite.h"
#include "IKETexture.h"
#include "Player.h"
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
	};

public:
	void LoadResource();
	//初期化
	bool Initialize(const float PosY = 0.0f);
	//更新
	void Update();
	//描画
	void Draw(DirectXCommon* dxCommon);
	//行動カードの描画
	void ActDraw(DirectXCommon* dxCommon);
	//ImGui
	void ImGuiDraw();
	//パネルを戻す
	void DeletePanel();

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
	XMFLOAT3 EnemySetPanel(const bool LastEnemy);
	//毒の配置(かぶらないように)
	void PoisonSetPanel(int& width, int& height);
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

	const bool GetAllDelete() { return m_AllDelete; }

private:
	static Player* player;
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
};