#include "character/BaseEnemy.h"

#include "Helper.h"
#include "StagePanel.h"
#include "CsvLoader.h"
#include "GameStateManager.h"
#include "ParticleEmitter.h"
#include "ImageManager.h"

BaseEnemy::BaseEnemy():
	GameObject("Enemy")
{
	// ビヘイビアツリー生成
	behavior_tree_ = std::make_unique<behaviorTree::SimpleBehaviorTree>(this);

	for (auto i = 0; i < draw_number_.size(); i++) {
		draw_number_[i] = make_unique<DrawNumber>();
		draw_number_[i]->Initialize();
	}
	hptex_ = IKESprite::Create(ImageManager::ENEMYHPUI, { 0.0f,0.0f });

}

bool BaseEnemy::Initialize()
{
	// エネミーのポインタをゲームステートマネージャに格納
	GameStateManager::GetInstance()->AddEnemy(std::static_pointer_cast<BaseEnemy>(shared_from_this()));
	return false;
}

void BaseEnemy::Update()
{
	// 毒フラグあり
	if (debuff_ & static_cast<int>(Debuff::kPoison))
	{
		int kTimerMax = 800;
		if (m_PoisonLong) { kTimerMax *= 2; }
		m_PoisonTimer++;

		if (m_PoisonTimer % 80 == 0) {	//一定フレームで1ずつ減らす
			if (!m_IsVenom) {
				hitpoint_ -= 1.0f;
			}
			else {
				hitpoint_ -= 2.0f;
			}
		}
		else if (m_PoisonTimer % 50 == 0) {		//毒のエフェクト
			BirthPoisonParticle();
		}

		if (m_PoisonTimer == kTimerMax) {	//一定時間立ったら毒終了
			debuff_ -= static_cast<int>(Debuff::kPoison);
			m_PoisonTimer = {};
		}
	}

	// ツリー更新
	behavior_tree_->Update();
	Obj_SetParam();

	if (hitpoint_ > 0.0f)
	{
		for (auto i{ 0 }; i < draw_number_.size(); ++i)
		{
			draw_number_[i]->Update();
		}
	}
	//数値化したHP
	for (auto i = 0; i < draw_number_.size(); i++) {
		digit_number_[i] = Helper::GetInstance()->getDigits(inter_hitpoint_, i, i);
	}
	//UIをワールド座標に変換する
	WorldDivision();
	hptex_->SetPosition(m_HPPos);
	hptex_->SetSize({ HpPercent() * m_HPSize.x,m_HPSize.y });


	inter_hitpoint_ = static_cast<int>(hitpoint_);
	
}

void BaseEnemy::Draw()
{
	Obj_Draw();
}

void BaseEnemy::UIDraw()
{
	IKESprite::PreDraw();
	//HPバー
	hptex_->Draw();
	//HP(数字)
	if (inter_hitpoint_ != 0)
		draw_number_[kFirstDight]->Draw();
	if (inter_hitpoint_ >= 10)
		draw_number_[kSecondDight]->Draw();
	if (inter_hitpoint_ >= 100)
		draw_number_[kThirdDight]->Draw();
	IKESprite::PostDraw();
}

void BaseEnemy::ImGuiDraw()
{
}

XMFLOAT3 BaseEnemy::SetPannelPos(int width, int height)
{
	panel_position_ =
	{ width,
		height
	};
	return StagePanel::GetInstance()->SetPositon(panel_position_.width, panel_position_.height);
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

void BaseEnemy::BirthPoisonParticle()
{
	const XMFLOAT4 s_color = { 0.5f,0.0f,0.5f,1.0f };
	const XMFLOAT4 e_color = { 0.5f,0.0f,0.5f,1.0f };
	const float s_scale = 1.0f;
	const float e_scale = 0.0f;
	for (int i = 0; i < 3; i++) {
		ParticleEmitter::GetInstance()->PoisonEffect(50, { m_Position.x,m_Position.y + 1.0f,m_Position.z }, s_scale, e_scale, s_color, e_color);
	}
}

void BaseEnemy::WorldDivision()
{
	Camera *camera = Helper::GetInstance()->GetCamera();
	m_MatView = camera->GetViewMatrix();
	m_MatProjection = camera->GetProjectionMatrix();
	m_MatPort = camera->GetViewPort();
	//HPバー
	XMVECTOR tex2DPos = { m_Position.x - 0.2f, m_Position.y, m_Position.z - 0.25f };
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatView, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatProjection, true);
	tex2DPos = Helper::GetInstance()->WDivision(tex2DPos, false);
	tex2DPos = Helper::GetInstance()->PosDivi(tex2DPos, m_MatPort, false);

	m_HPPos = { tex2DPos.m128_f32[0],tex2DPos.m128_f32[1] };

	//描画する数字と座標をここでセットする
	draw_number_[kFirstDight]->SetExplain({ m_Position.x + 0.55f, m_Position.y, m_Position.z - 0.55f });
	draw_number_[kSecondDight]->SetExplain({ m_Position.x + 0.2f, m_Position.y, m_Position.z - 0.55f });
	draw_number_[kThirdDight]->SetExplain({ m_Position.x - 0.15f, m_Position.y, m_Position.z - 0.55f });
	for (auto i = 0; i < draw_number_.size(); i++) {
		draw_number_[i]->GetCameraData();
		draw_number_[i]->SetNumber(digit_number_[i]);
	}
}

float BaseEnemy::HpPercent()
{
	float temp = hitpoint_ / max_hitpoint_;
	Helper::GetInstance()->Clamp(temp, 0.0f, 1.0f);
	return temp;
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
	BaseEnemy::Initialize();
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
	BaseEnemy::UIDraw();
}


void TestEnemy::ImGuiDraw()
{
	ImGui::Begin("TestEnemy");
	ImGui::Text("POSX:%f", m_Position.x);
	ImGui::Text("POSZ:%f", m_Position.z);
	ImGui::Text("Width:%d,Height:%d", panel_position_.width, panel_position_.height);
	ImGui::Text("Poison:%d",m_PoisonTimer);
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
