#pragma once
#include"InterEnemy.h"
#include "EnemyBullet.h"
#include "AttackArea.h"
#include "PredictArea.h"
using namespace std;         //  名前空間指定
//普通の敵
class BossEnemy :public InterEnemy {
public:
	BossEnemy();
	bool Initialize() override;//初期化
	void Finalize() override;//開放
	void Action()override;//更新
	void ImGui_Origin()override;

	void Draw(DirectXCommon* dxCommon) override;//描画
private:
	//関数ポインタ
	static void(BossEnemy::* stateTable[])();
	static void(BossEnemy::* attackTable[])();
private:
	void Inter();//待機
	void Attack();//攻撃
	void Teleport();//移動
	void BulletAttack();//弾の攻撃
	void RowAttack();
	void RandomAttack();
	void BirthBullet();//
	void BirthArea(const int Width,const int Height,const string& name);//攻撃エリア
	void BirthPredict(const int Width,const int Height,const string& name);//予測エリア

	//スキルのCSVを読み取る
	void LoadCsvSkill(std::string& FileName, const int id);
	//CSVを作成
	bool CreateSkill(int id);
	void PlayerCollide();
private:
	static const int BULLET_NUM = 5;
private:
	int m_AttackCount = {};
	int _charaState = STATE_INTER;
	vector<unique_ptr<EnemyBullet>> bullets;//ポルターガイスト

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
	// 攻撃エリア
	std::vector<unique_ptr<AttackArea>> attackarea;
	//予測エリア
	std::unique_ptr<PredictArea> predictarea;

	int m_RandWigth = {};
	int m_RandHeight = {};
};

