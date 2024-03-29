#pragma once
#include"InterEnemy.h"
#include "EnemyRock.h"
#include "EnemyTornade.h"
#include "Onomatope.h"
using namespace std;         //  ¼OóÔwè
//ÊÌG
class BossEnemy3 :public InterEnemy {
public:
	BossEnemy3();
	bool Initialize() override;//ú»
	void Finalize() override;//Jú
	void Action()override;//XV
	void ClearAction()override;//NAXV
	void GameOverAction()override;//Q[I[o[XV
	void ImGui_Origin()override;
	void DeathSpecial()override;

	void Draw(DirectXCommon* dxCommon) override;//`æ
private:
	//Ö|C^
	static void(BossEnemy3::* stateTable[])();
	static void(BossEnemy3::* attackTable[])();
private:
	void Inter();//Ò@
	void Attack();//U
	void Teleport();//Ú®
	void NormalAttack();//ÊÌU
	void RockAttack();//âÌU
	void RandomAttack();//_U
	void AroundAttack();//üèÌU
	void AngerMove();//{èÌ®«
	void BirthRock();//
	void BirthArea(const int Width, const int Height, const string& name);//UGA
	void BirthPredict(const int Width, const int Height, const string& name);//\ªGA

	//XLÌCSVðÇÝæé
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVðì¬
	bool CreateSkill(int id);
	void PlayerCollide();
	void WarpEnemy();//GÌ[v
	void AttackMove();//UÌ®«
	void SelectSafeArea(const string& name);
	void ShieldUpdate();
//@w
	void BirthMagic();
private:
	static const int SUPPORT_NUM = 2;
	static const int BULLET_NUM = 5;
	static const int SHIELD_NUM = 4;
	static const int SAFE_X = 4;
	static const int SAFE_Y = 4;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
		// âGA
	std::vector<unique_ptr<EnemyRock>> enerock;
	unique_ptr<Onomatope> onomatope = nullptr;
	enum AttackState {
		ATTACK_NORMAL,
		ATTACK_RANDOM,
		ATTACK_ROCK,
		ATTACK_AROUND,
	}_AttackState = ATTACK_NORMAL;
	std::vector<std::vector<int>> m_Area = {};
	// UGA
	std::vector<unique_ptr<EnemyTornade>> enetornade;
	
	int m_RandWidth = {};
	int m_RandHeight = {};

	//@wn
	enum MagicState {
		MAGIC_BIRTH,
		MAGIC_VANISH,
	};

	//@w
	struct Magic {
		unique_ptr<IKETexture> tex;
		float Frame = {};
		float Scale = {};
		float AfterScale = {};
		XMFLOAT3 Pos = {};
		bool Alive = false;
		int State = {};
		int Timer = {};
	};

	Magic magic;

	bool m_Warp = false;

	enum WarpState {
		WARP_START,
		WARP_END,
	};

	struct EnemyWarp {
		float Frame = {};
		float Scale = {};
		float AfterScale = 0.5f;
		int State = {};
	};

	EnemyWarp enemywarp;
	//C^[oÆ©
	vector<int>m_Limit;
	//UÌC^[oÆ©
	vector<int>m_AttackLimit;
	int m_BulletNum = {};


	int m_ShotDir = {};
	float m_AfterRotY = {};
	bool m_ChangeRot = {};

	bool m_SafeArea[SAFE_X][SAFE_Y];

	int m_RockCount = {};
	bool m_Anger = false;
	bool m_AngerFinish = false;
	int m_AngerTimer = {};
	int m_AngerCount = {};


	struct Shield {
		std::unique_ptr<IKETexture> tex = nullptr;
		XMFLOAT3 pos = {};
		float scale = 0.5f;
		XMFLOAT4 color = {};
		float CircleSpeed = {};
		float CircleScale = {};
	};

	std::array<Shield,SHIELD_NUM> shield;

	int m_ShieldTimer = {};
	float m_ShieldFrame = {};
	enum ShieldState {
		SHIELD_BIRTH,
		SHIELD_DELETE
	}_ShieldState = SHIELD_BIRTH;
};

