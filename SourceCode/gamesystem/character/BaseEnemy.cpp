#include "character/BaseEnemy.h"

#include "Helper.h"
#include "StagePanel.h"
#include "CsvLoader.h"



BaseEnemy::BaseEnemy():
	GameObject("Enemy")
{
	// ビヘイビアツリー生成
	behavior_tree_ = std::make_unique<behaviorTree::SimpleBehaviorTree>(this);	
}

bool BaseEnemy::Initialize()
{
	return false;
}

void BaseEnemy::Update()
{
	// ツリー更新
	behavior_tree_->Update();
	Obj_SetParam();
	
}

void BaseEnemy::Draw()
{
	Obj_Draw();
}

void BaseEnemy::UIDraw()
{
}

void BaseEnemy::ImGuiDraw()
{
}

XMFLOAT3 BaseEnemy::RandPanelPos()
{
	panel_position_ =
	{
		Helper::GetInstance()->GetRanNum(4, 7),
		Helper::GetInstance()->GetRanNum(0, 3)
	};
	return StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
}

TestEnemy::TestEnemy()
{
	// モデルセット
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));
	
	// ツリー実行開始
	CreateTree();
	behavior_tree_->Start();


}

bool TestEnemy::Initialize()
{
	// 座標セット
	m_Position = RandPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };

	// HPセット
	hitpoint_ = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	max_hitpoint_ = hitpoint_;

	return false;
}

void TestEnemy::Draw()
{
	Obj_Draw();
}

void TestEnemy::UIDraw()
{
}


void TestEnemy::ImGuiDraw()
{
	ImGui::Begin("TestEnemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", panel_position_.width);
	ImGui::Text("Height:%d", panel_position_.height);
	ImGui::End();
}

void TestEnemy::CreateTree()
{
	// ビヘイビアツリー設定デモ
	// ポーン(スプレッドシート4p)想定

	// コンディションノード
	// シーケンサやセレクタで条件判定を行うためのノード
	// ラムダ式を使用できるため、複雑な判定も可能
	ConditionalNode *front_check = new ConditionalNode // 最前列にいるか判定するノード
	(
		[&](GameObject *owner)
		{
			return panel_position_.width == 4 ?	// 敵陣地の最前列にいれば成功
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	front_check->Name = "最前列にいるか";


	// アクションノード
	// 実際に何かの処理を行うノード
	// Ranningを返す場合実行途中を意味し、処理を分岐できる
	ActionNode *pawn_attack = new ActionNode	// 斜めに攻撃
	(
		[&](GameObject *owner)
		{
			// アニメーションの代わりの疑似カウンタ
			static int temp_counter = 0;

			static bool init_flag = false;
			// 一度だけアニメーションする
			if (!init_flag)
			{
				// 攻撃モーションの実行をここに

				// 攻撃判定生成処理をここに

				init_flag = true;
				temp_counter = 0;
			}
			// アニメーションが終了したらランダムな最後列に移動して終了
			if (temp_counter >= 60)
			{
				// 最後尾に移動
				panel_position_ =
				{
					7,
					Helper::GetInstance()->GetRanNum(0, 3)
				};
				m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
				init_flag = false;
				return BehaviorStatus::Sucess;
			}

			temp_counter++;

			return BehaviorStatus::Runnning;
		}
	);
	pawn_attack->Name = "前方1マス斜めに攻撃";

	// アクションノード
	// 実際に何かの処理を行うノード
	// Ranningを返す場合実行途中を意味し、処理を分岐できる
	ActionNode *move = new ActionNode	// 移動
	(
		[&](GameObject *owner)
		{
			// 疑似カウンタ
			static int temp_diray_counter = 0;	// 移動直後の待機用カウンタ 
			static int temp_move_counter = 0;	// 移動間隔用カウンタ
			
			static bool init_flag = false;			// 疑似初期化
			// 一度だけアニメーションする
			if (!init_flag)
			{
				// 攻撃モーションの実行をここに

				// 攻撃判定生成処理をここに

				temp_diray_counter = 0;
				temp_move_counter = 0;
				init_flag = true;
			}
			// ディレイが終了したら移動開始
			// （実際にはディレイ用にノードを分けた方がいいが、いったんこのまま実装）
			if (temp_diray_counter >= 90)
			{
				if (temp_move_counter >= 30)
				{
					temp_move_counter = 0;

					// 最後尾に移動
					panel_position_ =
					{
						panel_position_.width - 1,
						panel_position_.height,
					};
					m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);

					if (panel_position_.width == 4)
					{
						init_flag = false;
						return BehaviorStatus::Sucess;
					}

				}
				else
				{
					temp_move_counter++;
				}
			}
			else
			{
				temp_diray_counter++;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move->Name = "移動実行";


	// シーケンサ
	// 失敗するまで実行するノード
	// どれか一つでも失敗するまで子のノードを実行
	Sequencer *is_attack = new Sequencer;	// 攻撃状態か識別する為のノード
	is_attack->Name = "攻撃";
	is_attack->NeedsConditionalAbort = true;	// 再調査
	is_attack->AddNode(front_check);	// 最前列チェック
	is_attack->AddNode(pawn_attack);	// 斜めに攻撃


	// コンディションノード
	// シーケンサやセレクタで条件判定を行うためのノード
	// ラムダ式を使用できるため、複雑な判定も可能
	ConditionalNode *alive_check = new ConditionalNode // HPチェッカー
	(
		[&](GameObject *owner)
		{
			return hitpoint_ > 0 ?	// HPが0より大きければ成功
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	alive_check->Name = "生存フラグ確認";


	// セレクター
	// 成功するまで実行するノード
	// どれか一つでも成功するまで子のノードを実行
	Selector *state_check = new Selector;	// 状態確認用ノード
	state_check->Name = "状態確認";
	state_check->AddNode(is_attack);
	state_check->AddNode(move);

	// シーケンサ
	// 失敗するまで実行するノード
	// どれか一つでも失敗するまで子のノードを実行
	Sequencer *is_alive = new Sequencer;
	is_alive->Name = "生存確認";
	is_alive->NeedsConditionalAbort = true;	// 再調査
	is_alive->AddNode(alive_check);	// 生存フラグ確認
	is_alive->AddNode(state_check);	// 状態確認

	// セレクター
	// 成功するまで実行するノード
	// どれか一つでも成功するまで子のノードを実行
	Selector *alive_state_checker = new Selector;	// 生存確認用ノード
	alive_state_checker->Name = "生死確認";
	alive_state_checker->AddNode(is_alive);



	// リピーター
	// ツリーをループに対応させるためのもの
	// 各種処理が実行中かどうか識別する
	Repeater *repeater = new Repeater();
	repeater->Name = "Repeater";
	repeater->AddNode(alive_state_checker);

	behavior_tree_->SetRootNode(repeater);	// リピーターをルートノードとして設定

}


RookEnemy::RookEnemy()
{
	// モデルセット
	m_Object.reset(new IKEObject3d());
	m_Object->Initialize();
	m_Object->SetModel(ModelManager::GetInstance()->GetModel(ModelManager::PLAYERMODEL));

	// ツリー実行開始
	CreateTree();
	behavior_tree_->Start();


}

bool RookEnemy::Initialize()
{
	// 座標セット
	m_Position = RandPanelPos();
	m_Rotation = { 0.0f,0.0f,0.0f };
	m_Color = { 1.0f,0.0f,0.5f,1.0f };
	m_Scale = { 0.5f,0.5f,0.5 };

	// HPセット
	hitpoint_ = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/chara/enemy/enemy.csv", "hp")));
	max_hitpoint_ = hitpoint_;

	return false;
}

void RookEnemy::Draw()
{
	Obj_Draw();
}

void RookEnemy::UIDraw()
{
}


void RookEnemy::ImGuiDraw()
{
	ImGui::Begin("RookEnemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d", panel_position_.width);
	ImGui::Text("Height:%d", panel_position_.height);
	ImGui::End();
}

void RookEnemy::CreateTree()
{
	// ビヘイビアツリー設定デモ
	// ポーン(スプレッドシート4p)想定

	// コンディションノード
	// シーケンサやセレクタで条件判定を行うためのノード
	// ラムダ式を使用できるため、複雑な判定も可能

#pragma region 突進回数チェック(コンディション)
	//突進回数が3回超えたかのチェック
	ConditionalNode* tacklecount_check = new ConditionalNode
	(
		[&](GameObject* owner)
		{
			return tacklecount == 3 ?	// タックル回数が3超えていればOK
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	tacklecount_check->Name = "3回突進した後か";
#pragma endregion

#pragma region 追尾突進実行(アクションノード)
	// アクションノード
	// 実際に何かの処理を行うノード
	// Ranningを返す場合実行途中を意味し、処理を分岐できる
	ActionNode* tracking_tackle = new ActionNode	// まっすぐ突進
	(
		[&](GameObject* owner)
		{
			// アニメーションの代わりの疑似カウンタ
			static int temp_counter = 0;

			static bool init_flag = false;
			// 一度だけアニメーションする
			if (!init_flag)
			{
				// 攻撃モーションの実行をここに

				// 攻撃判定生成処理をここに

				init_flag = true;
				temp_counter = 0;
			}
			// アニメーションが終了したらランダムな最後列に移動して終了
			if (temp_counter >= 60)
			{
				// 最後尾に移動
				panel_position_ =
				{
					7,
					Helper::GetInstance()->GetRanNum(0, 3)
				};
				m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
				init_flag = false;
				return BehaviorStatus::Sucess;
			}

			temp_counter++;

			return BehaviorStatus::Runnning;
		}
	);
	tracking_tackle->Name = "追尾突進";

#pragma endregion

#pragma region 追尾突進(シーケンサ)
	// シーケンサ
	// 失敗するまで実行するノード
	// どれか一つでも失敗するまで子のノードを実行
	Sequencer* is_tracking_tackle = new Sequencer;	// 攻撃状態か識別する為のノード
	is_tracking_tackle->Name = "追尾突進";
	is_tracking_tackle->NeedsConditionalAbort = true;	// 再調査
	is_tracking_tackle->AddNode(tacklecount_check);	//3回突進したかどうか
	is_tracking_tackle->AddNode(tracking_tackle);	// 追尾突進
#pragma endregion

#pragma region 通常突進実行(アクションノード)
	// アクションノード
	// 実際に何かの処理を行うノード
	// Ranningを返す場合実行途中を意味し、処理を分岐できる
	ActionNode* normal_tackle = new ActionNode	// まっすぐ突進
	(
		[&](GameObject* owner)
		{
			// アニメーションの代わりの疑似カウンタ
			static int temp_counter = 0;

			static bool init_flag = false;
			// 一度だけアニメーションする
			if (!init_flag)
			{
				// 攻撃モーションの実行をここに

				// 攻撃判定生成処理をここに

				init_flag = true;
				temp_counter = 0;
			}
			// アニメーションが終了したらランダムな最後列に移動して終了
			if (temp_counter >= 60)
			{
				// 最後尾に移動
				panel_position_ =
				{
					7,
					Helper::GetInstance()->GetRanNum(0, 3)
				};
				m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
				init_flag = false;
				return BehaviorStatus::Sucess;
			}

			temp_counter++;

			return BehaviorStatus::Runnning;
		}
	);
	normal_tackle->Name = "通常突進";

#pragma endregion

#pragma region 通常突進(シーケンサ)
	// シーケンサ
	// 失敗するまで実行するノード
	// どれか一つでも失敗するまで子のノードを実行
	Sequencer* is_normal_tackle = new Sequencer;	// 攻撃状態か識別する為のノード
	is_normal_tackle->Name = "通常突進";
	is_normal_tackle->NeedsConditionalAbort = true;	// 再調査
	//is_normal_tackle->AddNode(waitingcount_check);	//攻撃可能かどうか
	is_normal_tackle->AddNode(normal_tackle);	// 通常突進
#pragma endregion

#pragma region 攻撃状態確認(コンディション)
	//攻撃可能かどうか
	ConditionalNode* waitingcount_check = new ConditionalNode
	(
		[&](GameObject* owner)
		{
			return waitingcount == 60 ?	//待ち時間が1秒超えていればok
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	waitingcount_check->Name = "攻撃可能かどうか";
#pragma endregion

#pragma region 攻撃種類確認(セレクター)
	// セレクター
	// 成功するまで実行するノード
	// どれか一つでも成功するまで子のノードを実行
	Selector* attacktype_check = new Selector;	// 攻撃種類確認用ノード
	attacktype_check->Name = "攻撃種類確認";
	attacktype_check->AddNode(is_tracking_tackle);
	attacktype_check->AddNode(is_normal_tackle);
#pragma endregion

#pragma region 攻撃(シーケンサ)
	// シーケンサ
	// 失敗するまで実行するノード
	// どれか一つでも失敗するまで子のノードを実行
	Sequencer* is_attack = new Sequencer;	// 攻撃状態か識別する為のノード
	is_attack->Name = "攻撃";
	is_attack->NeedsConditionalAbort = true;	// 再調査
	is_attack->AddNode(waitingcount_check);	//攻撃可能かどうか
	is_attack->AddNode(attacktype_check);	//攻撃種類確認
#pragma endregion

#pragma region 移動実行(アクションノード)
	// アクションノード
	// 実際に何かの処理を行うノード
	// Ranningを返す場合実行途中を意味し、処理を分岐できる
	ActionNode* move = new ActionNode	// 移動
	(
		[&](GameObject* owner)
		{
			// 疑似カウンタ
			static int temp_diray_counter = 0;	// 移動直後の待機用カウンタ 
			static int temp_move_counter = 0;	// 移動間隔用カウンタ

			static bool init_flag = false;			// 疑似初期化
			// 一度だけアニメーションする
			if (!init_flag)
			{
				// 攻撃モーションの実行をここに

				// 攻撃判定生成処理をここに

				temp_diray_counter = 0;
				temp_move_counter = 0;
				init_flag = true;
			}
			// ディレイが終了したら移動開始
			// （実際にはディレイ用にノードを分けた方がいいが、いったんこのまま実装）
			if (temp_diray_counter >= 90)
			{
				if (temp_move_counter >= 30)
				{
					temp_move_counter = 0;

					// 最後尾に移動
					panel_position_ =
					{
						panel_position_.width - 1,
						panel_position_.height,
					};
					m_Position = StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);

					if (panel_position_.width == 4)
					{
						init_flag = false;
						return BehaviorStatus::Sucess;
					}

				}
				else
				{
					temp_move_counter++;
				}
			}
			else
			{
				temp_diray_counter++;
			}

			return BehaviorStatus::Runnning;
		}
	);
	move->Name = "移動実行";

#pragma endregion

#pragma region 生存フラグ確認(コンディション)
	// コンディションノード
	// シーケンサやセレクタで条件判定を行うためのノード
	// ラムダ式を使用できるため、複雑な判定も可能
	ConditionalNode* alive_check = new ConditionalNode // HPチェッカー
	(
		[&](GameObject* owner)
		{
			return hitpoint_ > 0 ?	// HPが0より大きければ成功
				BehaviorStatus::Sucess : BehaviorStatus::Failure;
		}
	);
	alive_check->Name = "生存フラグ確認";
#pragma endregion

#pragma region 状態確認(セレクター)
	// セレクター
	// 成功するまで実行するノード
	// どれか一つでも成功するまで子のノードを実行
	Selector* state_check = new Selector;	// 状態確認用ノード
	state_check->Name = "状態確認";
	state_check->AddNode(is_attack);
	state_check->AddNode(move);
#pragma endregion

#pragma region 生存(シーケンサ)
	// シーケンサ
	// 失敗するまで実行するノード
	// どれか一つでも失敗するまで子のノードを実行
	Sequencer* is_alive = new Sequencer;
	is_alive->Name = "生存";
	is_alive->NeedsConditionalAbort = true;	// 再調査
	is_alive->AddNode(alive_check);	// 生存フラグ確認
	is_alive->AddNode(state_check);	// 状態確認
#pragma endregion

#pragma region 生存確認(セレクタ)
	// セレクター
	// 成功するまで実行するノード
	// どれか一つでも成功するまで子のノードを実行
	Selector* alive_state_checker = new Selector;	// 生存確認用ノード
	alive_state_checker->Name = "生存確認";
	alive_state_checker->AddNode(is_alive);
#pragma endregion


	// リピーター
	// ツリーをループに対応させるためのもの
	// 各種処理が実行中かどうか識別する
	Repeater* repeater = new Repeater();
	repeater->Name = "Repeater";
	repeater->AddNode(alive_state_checker);

	behavior_tree_->SetRootNode(repeater);	// リピーターをルートノードとして設定

}