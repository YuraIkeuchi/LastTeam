#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"
#include "EnemyThorn.h"

using namespace std;         //  ¼OóÔwè
//ÊÌG
class BossEnemy :public InterEnemy {
public:
	BossEnemy();
	bool Initialize() override;//ú»
	void Finalize() override;//Jú
	void Action()override;//XV
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//`æ
private:
	//Ö|C^
	static void(BossEnemy::* stateTable[])();
	static void(BossEnemy::* attackTable[])();
private:
	void Inter();//Ò@
	void Attack();//U
	void Teleport();//Ú®
	void BulletAttack();//eÌU
	void RowAttack();
	void RandomAttack();
	void BirthBullet();//
	void BirthArea(const int Width,const int Height,const string& name);//UGA
	void BirthPredict(const int Width,const int Height,const string& name);//\ªGA

	//XLÌCSVðÇÝæé
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVðì¬
	bool CreateSkill(int id);
	void PlayerCollide();
	void WarpEnemy();//GÌ[v
//@w
	void BirthMagic();
private:
	static const int BULLET_NUM = 5;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//|^[KCXg

	enum BossType {
		Boss_SET,
		Boss_THROW,
		Boss_END,
	}_BossType = Boss_SET;

	enum AttackState {
		ATTACK_BULLET,
		ATTACK_ROW,
		ATTACK_RANDOM,
	}_AttackState = ATTACK_RANDOM;
	std::vector<std::vector<int>> m_Area = {};
	// UGA
	std::vector<unique_ptr<EnemyThorn>> enethorn;
	

	int m_RandWigth = {};
	int m_RandHeight = {};

	//@wn
	enum MagicState {
		MAGIC_BIRTH,
		MAGIC_VANISH,
	};

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
};

