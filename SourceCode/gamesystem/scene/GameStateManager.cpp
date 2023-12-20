#include <GameStateManager.h>
#include <Helper.h>
#include <StagePanel.h>
#include <Input.h>
#include <Easing.h>
#include <ImageManager.h>
#include <SkillManager.h>
#include <TutorialTask.h>

Player* GameStateManager::player = nullptr;
GameStateManager* GameStateManager::GetInstance() {
	static GameStateManager instance;
	return &instance;
}
//初期化
void GameStateManager::Initialize() {
	//カウンター
	m_Counter = false;
	m_CounterTimer = {};
	m_CounterCount = 0;
	//位置のスコア
	m_PosScore = 0;

	//全体スコア
	m_AllScore = {};
	m_MaxDamage = 0;
	m_MaxTakenDamage = 0;
	m_MaxTakenDamage = 0;
	m_HandedCount = 0;
	//終了関連
	isFinish = false;
	isChangeScene = false;
	m_Choice = false;

	//要素の全削除は一旦ここで
	m_AllActCount = {};
	actui.clear();
	m_Act.clear();
	attackarea.clear();
	regenearea.clear();
	//一旦クリア方式で
	GotPassives.clear();
	PassiveCheck();
	skillUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,550.f }, { 0.9f,0.9f,0.9f,1.f }, { 0.5f,1.f });
	skillUI->SetSize(basesize);
	gaugeUI = IKESprite::Create(ImageManager::GAUGE, { 45.f,550.f }, { 0.6f,0.6f,1.f,1.f }, { 0.5f,1.f });
	gaugeUI->SetSize({ basesize.x,0.f });
	gaugeCover = IKESprite::Create(ImageManager::GAUGECOVER, { 45.f,550.f + 32.0f }, { 1.f,1.f,1.f,1.f }, { 0.5f,1.f });
	handsFrame = IKESprite::Create(ImageManager::HANDSCOVER, { 80.f,640.0f }, { 1.f,1.f,1.f,1.f }, { 0.5f,0.5f });
	passiveActive = IKESprite::Create(ImageManager::PASSIVE_ACTIVE, { 640.f,50.0f }, { 1.f,1.f,1.f,1.f }, { 0.5f,0.5f });

	resultReport = make_unique<ResultReport>();
	resultSkill = make_unique<ResultSkill>();
	resultSkill->Initialize(m_dxCommon);
	resultSkill->SetPlayer(player);
	haveSkill = make_unique<HaveResultSkill>();
	haveSkill->Initialize(m_dxCommon);

	onomatope = make_unique<Onomatope>();

	m_PredictTimer = {};
	//
	SkillManager::GetInstance()->Initialize();

	//デッキの初期化
	DeckInitialize();

	//デッキにないカードを検索する
	const int CARD_MAX = SkillManager::GetInstance()->GetSkillMax();
	m_NotDeckNumber.clear();
	for (int i = 0; i < m_DeckNumber.size(); i++) {
		for (int j = 0; j < CARD_MAX; j++) {
			bool found = std::find(m_DeckNumber.begin(), m_DeckNumber.end(), j) != m_DeckNumber.end();		//最初から探す
			if (!found) {
				m_NotDeckNumber.push_back(j);		//なかったら追加する
			}
		}
		break;
	}
	//予測
	predictarea.reset(new PredictArea("PLAYER"));
	predictarea->Initialize();
	m_DiscardNumber.clear();
	//右のゲージ
	m_GaugeCount = {};
	m_NotCount = (int)(m_NotDeckNumber.size()) - 1;//無いカードの枚数を検索してる(ImGui用)

	//m_Object->SetScale({ 2.f,0.1f,2.f });	
	_charge.reset(new IKETexture(ImageManager::CHARGE, {}, { 1.f,1.f,1.f }, { 1.f,1.f,1.f,1.f }));
	_charge->TextureCreate();
	_charge->Initialize();
	_charge->SetRotation({ 90.0f,0.0f,0.0f });
	m_ChargeScale = 1.0f;
	m_Delay = false;
	m_Buff = false;
	predictarea->ResetPredict();

	m_GameStart = false;
}

//更新
void GameStateManager::Update() {
	if (ResultUpdate()) { return; }
	const int l_AddCounterScore = 10;
	m_AllScore = (m_CounterCount * l_AddCounterScore) + m_PosScore;
	//今いる位置でスコアチェンジ
	switch (m_NowWidth) {
	case 0:
		m_PosScore -= 2;
		break;
	case 1:
		m_PosScore--;
		break;
	case 2:
		m_PosScore++;
		break;
	case 3:
		m_PosScore += 2;
		break;
	default:
		break;
	}
	Helper::Clamp(m_PosScore,-1000,1000);

	//カウンターの処理
	if (m_Counter) {
		if (m_CounterTimer == 0) {		//カウンターのスコアに加算
			m_CounterCount++;
		}
		m_CounterTimer++;

		if (Helper::CheckMin(m_CounterTimer, 20, 1)) {		//一定フレームでカウンター終了
			m_Counter = false;
			onomatope->AddOnomato(Counter, { 640.f,660.f });
			m_CounterTimer = {};
		}
	}

	//手に入れたスキルのUIの更新
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->SetActCount(i);
		actui[i]->Update();

		if (!actui[i]->GetAlive()) {
			actui.erase(cbegin(actui) + i);
		}
	}

	//攻撃エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < attackarea.size(); i++) {
		if (attackarea[i] == nullptr)continue;
		attackarea[i]->Update();

		if (!attackarea[i]->GetAlive()) {
			attackarea.erase(cbegin(attackarea) + i);
		}
	}
	//回復エリアの更新(実際はスキルになると思う)
	for (auto i = 0; i < regenearea.size(); i++) {
		if (regenearea[i] == nullptr)continue;
		regenearea[i]->Update();

		if (!regenearea[i]->GetAlive()) {
			regenearea.erase(cbegin(regenearea) + i);
		}
	}

	GaugeUpdate();
	//攻撃した瞬間
	AttackTrigger();
	UseSkill();
	if (m_ResetPredict) {
		m_PredictTimer++;
		if (m_PredictTimer > 1) {
			PredictManager();
			m_ResetPredict = false;
			m_PredictTimer = {};
		}
	}
	SkillManager::GetInstance()->Update();
	player->SetDelay(m_Delay);

	_charge->SetPosition({ player->GetPosition().x,0.5f,player->GetPosition().z });
	_charge->SetScale({ m_ChargeScale,m_ChargeScale,m_ChargeScale });
	_charge->Update();
	onomatope->Update();

	PassiveActive();
	PowerUpEffectUpdate();
	DamageEffectUpdate();
}
//攻撃した瞬間
void GameStateManager::AttackTrigger() {
	Input* input = Input::GetInstance();
	if (m_AllActCount == 0) { return; }
	if (actui[0]->GetUse()) { return; }
	if (player->GetCharaState() == 1) { return; }
	if (isFinish) { return; }
	if (m_Delay) { return; }
	//スキルが一個以上あったらスキル使える
	if (input->TriggerButton(input->B) ||
		input->TriggerKey(DIK_SPACE)) {
		m_HandedCount++;
		AttackSubAction();
		m_Delay = true;
	}
}
void GameStateManager::Draw(DirectXCommon* dxCommon) {
	if (!m_GameStart) { return; }
	if (!isFinish && !isChangeScene) {
		IKETexture::PreDraw2(dxCommon, AlphaBlendType);
		if (m_Delay && m_Act[0].ActDelay >= 30) {
			_charge->Draw();
		}
		IKETexture::PostDraw();
		IKESprite::PreDraw();
		for (DamageEffect& damage : damages) {
			damage.tex->Draw();
		}
		handsFrame->Draw();
		skillUI->Draw();
		gaugeUI->Draw();
		//gaugeCover->Draw();
		if (isPassive) {
			passiveAct->Draw();
			passiveActive->Draw();
		}
		onomatope->Draw();
		IKESprite::PostDraw();
		SkillManager::GetInstance()->UIDraw();
		for (auto i = 0; i < attackarea.size(); i++) {
			if (attackarea[i] == nullptr)continue;
			attackarea[i]->Draw(dxCommon);
		}
		for (auto i = 0; i < regenearea.size(); i++) {
			if (regenearea[i] == nullptr)continue;
			regenearea[i]->Draw(dxCommon);
		}

		if (m_AllActCount != 0) {
			predictarea->Draw(dxCommon);
		}
	}
	if (isFinish) {
		if (!resultReport->GetIsFinish()) {
			resultReport->Draw(dxCommon);
		} else {
			if (_ResultType == GET_SKILL) {
				resultSkill->Draw(dxCommon);
			} else {
				haveSkill->Draw(dxCommon);
			}
		}
	}
}
//描画
void GameStateManager::ImGuiDraw() {
	//SkillManager::GetInstance()->ImGuiDraw();
	if (savedata.m_DeckNum != 0) {
		ImGui::Begin("Deck");
		for (int i = 0; i < savedata.m_DeckNum; i++) {
			ImGui::Text("DeckNum[%d]:%d", i, m_DeckNumber[i]);
			ImGui::Text("OpenDeckNum[%d]:%d", i, savedata.m_OpenDeckNumber[i]);
		}
		ImGui::End();
	}
}
//手に入れたUIの描画
void GameStateManager::ActUIDraw() {
	for (auto i = 0; i < actui.size(); i++) {
		if (actui[i] == nullptr)continue;
		actui[i]->Draw();
	}
	for (unique_ptr<Passive>& passive : GotPassives) {
		passive->Draw();
	}
	IKESprite::PreDraw();
	for (PowerUpEffect& power : powerup) {
		power.tex->Draw();
	}
	IKESprite::PostDraw();
}
//スキルを入手(InterActionCPPで使ってます)
void GameStateManager::AddSkill(const int SkillType, const int ID, const float damage, const int Delay,
	vector<std::vector<int>> area, vector<std::vector<int>> timer, int DisX, int DisY, string name,int Token) {
	ActState act;
	act.SkillType = SkillType;
	if (act.SkillType == 0 || act.SkillType == 1 || act.SkillType == 3) {
		act.ActID = ID;
		act.ActDamage = damage;
		act.AttackArea.resize(7);
		act.AttackTimer.resize(7);
		act.DistanceX = DisX;
		act.DistanceY = DisY;
		act.PoisonToken = Token;

		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 7; j++) {
				act.AttackArea[i].push_back(j);
				act.AttackTimer[i].push_back(j);
			}
		}
		act.AttackArea = area;
		act.AttackTimer = timer;
	}
	act.ActDelay = Delay;
	act.StateName = name;
	m_Act.push_back(act);
	//手に入れたスキルの総数を加算する
	m_AllActCount++;
	BirthActUI(ID, act.SkillType);//UIも増えるよ
	PredictManager();
}
//スキルUIの生成
void GameStateManager::BirthActUI(const int ID, const int Type) {
	//アクションUIのセット
	ActionUI* newactUi = nullptr;
	newactUi = new ActionUI(ID);
	newactUi->Initialize();
	newactUi->InitState(m_AllActCount, Type);
	actui.emplace_back(newactUi);

	Audio::GetInstance()->PlayWave("Resources/Sound/SE/cardget.wav", 0.15f);
}

//攻撃エリアの生成(無理やり処理)
void GameStateManager::BirthArea() {
	int l_BirthBaseX = {};
	int l_BirthBaseY = {};
	int Timer = {};
	l_BirthBaseX = m_NowWidth + m_Act[0].DistanceX;		//生成の初めの位置を見てる
	l_BirthBaseY = m_NowHeight + m_Act[0].DistanceY;
	int AreaX = {};
	int AreaY = {};
	float damage = m_Act[0].ActDamage;
	if (m_Act[0].ActID == 10) {
		//リフレイン攻撃
		damage = (float)m_MaxDamage;
	}
	for (auto i = 0; i < m_Act[0].AttackArea.size(); i++) {
		for (auto j = 0; j < m_Act[0].AttackArea.size(); j++) {
			AreaX = l_BirthBaseX + i;
			AreaY = l_BirthBaseY - j;
			if (m_Act[0].AttackArea[i][j] == 1 && ((AreaY < 4) && (AreaY >= 0)) && (AreaX < 8)) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				//回復エリアの生成かどうか決める
				if (m_Act[0].StateName == "REGENE") {
					std::unique_ptr<RegeneArea> newarea = std::make_unique<RegeneArea>();
					newarea->InitState(AreaX, AreaY);
					regenearea.emplace_back(std::move(newarea));
				}
				else {
					std::unique_ptr<AttackArea> newarea = std::make_unique<AttackArea>((string)"Player");
					newarea->InitState(AreaX, AreaY);
					newarea->SetDamage(damage);
					newarea->SetTimer(m_Act[0].AttackTimer[i][j]);
					newarea->SetPoisonToken(m_Act[0].PoisonToken);
					if (m_Act[0].ActID == 10) {
						//固定ダメージ
						newarea->SetIsFixed(true);
					}
					newarea->SetStateName(m_Act[0].StateName);
					attackarea.emplace_back(std::move(newarea));
				}
			}
		}
	}
}
//予測エリア関係
void GameStateManager::PredictManager() {
	if (m_AllActCount == 0) { return; }
	if (m_Act.empty()) { return; }
	predictarea->ResetPredict();
	int l_BirthBaseX = {};
	int l_BirthBaseY = {};

	l_BirthBaseX = m_NowWidth + m_Act[0].DistanceX;;		//生成の初めの位置を見てる
	l_BirthBaseY = m_NowHeight + m_Act[0].DistanceY;

	if (m_Act[0].SkillType == 0) {
		for (auto i = 0; i < m_Act[0].AttackArea.size(); i++) {
			for (auto j = 0; j < m_Act[0].AttackArea.size(); j++) {

				int AreaX = {};
				int AreaY = {};
				AreaX = l_BirthBaseX + i;
				AreaY = l_BirthBaseY - j;
				if (m_Act[0].AttackArea[i][j] == 1 && (AreaY < 4) && (AreaY >= 0)) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
					predictarea->SetPredict(AreaX, AreaY, true);
				}
			}
		}
	}
	else {
		predictarea->SetPredict(m_NowWidth, m_NowHeight,true);
	}
	if (m_Act[0].SkillType == 0) {
		if (m_Act[0].StateName == "REGENE") {
			predictarea->SetDrawDype(PREDICT_HEAL);
		}
		else {
			predictarea->SetDrawDype(PREDICT_ATTACK);
		}
	}
	else if (m_Act[0].SkillType == 1) {
		if (m_Act[0].StateName == "NEXT") {
			predictarea->SetDrawDype(PREDICT_BUFF);
		}
		else if (m_Act[0].StateName == "RANDOM") {
			predictarea->SetDrawDype(PREDICT_HATENA);
		}
	}
	predictarea->Update();
}
//プレイヤーの現在パネル
void GameStateManager::PlayerNowPanel(const int NowWidth, const int NowHeight) {
	m_NowWidth = NowWidth, m_NowHeight = NowHeight;
}
//スキルの使用
void GameStateManager::UseSkill() {
	if (m_AllActCount == 0) { return; }
	if (!m_Delay) { return; }
	int delay = m_Act[0].ActDelay;
	if (m_ExtendQueen) {
		delay =(int)((float)delay * 0.7f);
	}
	m_ChargeScale = Helper::Lerp(1.0f, 0.0f, m_DelayTimer, delay);		//線形補間でチャージを表してる
	if (Helper::CheckMin(m_DelayTimer, delay, 1)) {
		if (m_Act[0].SkillType == 0) {
			BirthArea();
			if (m_Act[0].StateName != "Refrain") {
				if (m_Act[0].ActDamage <= 3) {
					onomatope->AddOnomato(Attack02, { 440.f,0.f });
				} else if (m_Act[0].ActDamage <= 10) {
					onomatope->AddOnomato(Attack01, { 640.f,360.f });
				} else {
					onomatope->AddOnomato(Attack03, { 840.f,0.f });
				}
			} else {
				onomatope->AddOnomato(Refrain, { 640.f,800.f });

			}
		} else if (m_Act[0].SkillType == 1) {
			if (m_Act[0].StateName == "NEXT") {
				for (int i = 0; i < 2; i++) {
					RandPowerUpInit();
				}
				BirthBuff();
				onomatope->AddOnomato(AttackCharge, { 340.f,360.f });
			}
			else if (m_Act[0].StateName == "RANDOM") {
				int l_rand = {};
				l_rand = Helper::GetRanNum(0, 1);
				if (l_rand == 0) {
					player->HealPlayer(10.0f);
				}
				else {
					player->RecvDamage(10.0f);
				}
			}
		}

		FinishAct();
		if (m_AllActCount == 0) {
			player->AttackCheck(true);
		} else {
			player->AttackCheck(false);
		}
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/SkillUse.wav", 0.1f);
		m_ResetPredict = true;
		m_Delay = false;
		m_DelayTimer = {};
		m_ChargeScale = 5.0f;
	}
}
//行動の終了
void GameStateManager::FinishAct() {
	m_DiscardNumber.push_back(m_Act[0].ActID);
	m_Act.erase(m_Act.begin());
	m_AllActCount--;
	actui[0]->SetUse(true);
	//デッキがない且つ手札を使い切ってたらまた再配布
	if (m_AllActCount == 0 && StagePanel::GetInstance()->GetAllDelete()) {
		//デッキの初期化
		//DeckInitialize();
	}
}

void GameStateManager::GaugeUpdate() {
	if (!m_GameStart) { return; }
	if (m_Act.size() == m_DeckNumber.size()) {
		m_GaugeCount = 0.0f;
	} else {
		if (TutorialTask::GetInstance()->GetTutorialState() >= TASK_BIRTH_BEFORE) {
			m_GaugeCount += 1.0f * m_DiameterGauge;
		}
	}
	if (m_GaugeCount >= kGaugeCountMax) {
		if (m_IsReloadDamage) {
			int r_num = Helper::GetRanNum(0, 99);
			if (r_num < 30) {
				//エネミーに3ダメージ
				m_ReloadDamage = true;
				SetPassiveActive((int)Passive::ABILITY::RELOAD_DAMAGE);
			}
		}
		if (m_IsReload) {
			StagePanel::GetInstance()->ResetAction();
			StagePanel::GetInstance()->ResetPanel();
		} else {
			if (!StagePanel::GetInstance()->AllCleanCheack()) {
				SetPassiveActive((int)Passive::ABILITY::RELOAD_LOCK);
			}
		}
		//パネル置く数
		int panel_num = 3;
		SkillManager::GetInstance()->ResetBirth();
		if (SkillManager::GetInstance()->GetDeckNum() >= 3) {
			StagePanel::GetInstance()->RandomPanel(panel_num);
		} else {
			StagePanel::GetInstance()->RandomPanel(SkillManager::GetInstance()->GetDeckNum());
		}
		m_GaugeCount = 0;
		if (TutorialTask::GetInstance()->GetTutorialState() == TASK_BIRTH_BEFORE) {		//チュートリアル専用
			TutorialTask::GetInstance()->SetTutorialState(TASK_BIRTHSKIL);
		}
		if (SkillManager::GetInstance()->GetDeckNum() == 0 && StagePanel::GetInstance()->GetAllDelete()) {
			DeckDiscard();
		}
	}
	float per = (m_GaugeCount / kGaugeCountMax);
	float size = Ease(In, Quad, 0.5f, gaugeUI->GetSize().y, basesize.y * per);
	gaugeUI->SetSize({ basesize.x,size });
}

void GameStateManager::PassiveCheck() {

	for (int& id : GotPassiveIDs) {
		GetPassive(id);
	}

	for (unique_ptr<Passive>& passive : GotPassives) {
		switch (passive->GetAbility()) {
		case Passive::ABILITY::RELOAD_UP:
			m_DiameterGauge = passive->GetDiameter();
			break;
		case Passive::ABILITY::HP_UP:
			break;
		case Passive::ABILITY::RELOAD_LOCK:
			m_IsReload = false;
			break;
		case Passive::ABILITY::POISON_GAUGEUP:
			m_poizonLong = true;
			break;
		case Passive::ABILITY::POISON_DAMAGEUP:
			m_IsVenom = true;
			break;
		case Passive::ABILITY::DRAIN_HEALUP:
			m_IsDrainUp = true;
			break;
		case Passive::ABILITY::RELOAD_DAMAGE:
			m_IsReloadDamage = true;
			break;
		case Passive::ABILITY::FIVE_POWER:
			m_FivePower = true;
			break;
		case Passive::ABILITY::TAKENDAMAGEUP:
			m_TakenDamageUp = true;
			break;
		case Passive::ABILITY::ATTACK_POISON:
			m_AttackedPoison = true;
			break;
		case Passive::ABILITY::HEAL_ATTACK:
			player->SetHealingDamage(true);
			break;
		case Passive::ABILITY::EXTEND_KNIGHT:
			m_ExtendKnight = true;
			break;
		case Passive::ABILITY::EXTEND_ROOK:
			m_ExtendRook = true;
			break;
		case Passive::ABILITY::EXTEND_QUEEN:
			m_ExtendQueen = true;
			break;
		case Passive::ABILITY::EXTEND_BISHOP:
			m_ExtendBishop = true;
			break;
		default:
			assert(0);
			break;
		}
	}
	const int PASSIVE_MAX = (int)Passive::ABILITY::MAX_ABILITY;
	NotPassiveIDs.clear();
	if (GotPassiveIDs.size() == 0) {
		for (int j = 0; j < PASSIVE_MAX; j++) {
			NotPassiveIDs.push_back(j);		//なかったら追加する
		}
		return;
	}
	for (int i = 0; i < GotPassiveIDs.size(); i++) {
		for (int j = 0; j < PASSIVE_MAX; j++) {
			bool found = std::find(GotPassiveIDs.begin(), GotPassiveIDs.end(), j) != GotPassiveIDs.end();		//最初から探す
			if (!found) {
				NotPassiveIDs.push_back(j);		//なかったら追加する
			}
		}
		break;
	}
}
//デッキの初期化
void GameStateManager::DeckInitialize() {
	StagePanel::GetInstance()->ResetPanel();
	SkillManager::GetInstance()->DeckClear();
	//デッキに入っているカードの確認
	for (int i = 0; i < m_DeckNumber.size(); i++) {
		SkillManager::GetInstance()->DeckCheck(m_DeckNumber[i], i);
	}
	//デッキの最大数確認
	SkillManager::GetInstance()->SetDeckState((int)(m_DeckNumber.size()));
}

void GameStateManager::GetPassive(int ID) {
	float posX = GotPassives.size() * 70.0f;
	GotPassives.push_back(std::move(make_unique<Passive>(ID, XMFLOAT2{ posX ,50.0f })));
}


bool GameStateManager::AttackSubAction() {
	SkillRecycle();
	return true;
}

bool GameStateManager::ResultUpdate() {
	if (!isFinish) { return false; }
	if (!TutorialTask::GetInstance()->GetViewSkill()) { return false; }
	if (!resultReport->GetIsFinish()) {
		resultReport->Update();
		return false;
	}
	if (Input::GetInstance()->TriggerButton(Input::LB) ||
		Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		_ResultType = GET_SKILL;
	}
	if (Input::GetInstance()->TriggerButton(Input::RB) ||
		Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		_ResultType = HAVE_SKILL;
	}

	if (_ResultType == GET_SKILL) {
		resultSkill->Update();

		if ((Input::GetInstance()->TriggerButton(Input::B) || Input::GetInstance()->TriggerKey(DIK_SPACE)) && !m_Choice) {
			resultSkill->InDeck(m_DeckNumber);
			resultSkill->InPassive(GotPassiveIDs);
			isChangeScene = true;
			isFinish = false;
			m_Choice = true;
			TutorialTask::GetInstance()->SetChoiceSkill(true);
		}
	} else {
		haveSkill->Update();
	}

	return true;
}

void GameStateManager::InDeck() {
	m_DeckNumber.push_back(m_NotDeckNumber[m_NotCount]);
	m_NotDeckNumber.erase(cbegin(m_NotDeckNumber) + m_NotCount);
}

bool GameStateManager::SkillRecycle() {
	//if (!m_IsRecycle) { return false; }
	//if (Helper::GetRanNum(0, 100) > 20) {
	//	return false;
	//}

	//SkillManager::GetInstance()->PushOnce2Deck(actui[0]->GetID());
	////デッキの最大数確認
	//SkillManager::GetInstance()->SetDeckState((int)(SkillManager::GetInstance()->GetDeckUISize()));
	return true;
}

void GameStateManager::DamageEffectInit(XMFLOAT2 pos) {
	for (int i = 0; i < 6; i++) {
		DamageEffect damage;
		if (i % 3 == 0) {
			damage.tex = IKESprite::Create(ImageManager::SHINE_S, { -100.f,0.f });
		} else {
			damage.tex = IKESprite::Create(ImageManager::SHINE_L, { -100.f,0.f });
		}
		damage.tex->SetAnchorPoint({ 0.5f,0.5f });
		damage.tex->SetSize({ 64.f,64.f });
		damage.angle = ((i + 1) * 60.f) * (XM_PI / 180.f);
		damage.position = pos;
		damages.push_back(std::move(damage));
	}

}

void GameStateManager::StageClearInit() {
	if (isFinish) { return; }
	haveSkill->HaveAttackSkill(m_DeckNumber, (int)m_DeckNumber.size(), m_dxCommon);
	haveSkill->HavePassiveSkill(GotPassiveIDs, (int)GotPassiveIDs.size(), m_dxCommon);
	resultSkill->SetIsBattle(isBattleFromMap);
	resultSkill->CreateResult(m_NotDeckNumber, NotPassiveIDs);
	m_PredictTimer = {};
	resultReport->SetScore(m_PosScore);
	isFinish = true;
}
//バフの生成
void GameStateManager::BirthBuff() {
	m_Buff = true;		//一旦中身はこれだけ
}
void GameStateManager::DeckReset() {
	m_DeckNumber.resize((int)(m_StartNumber.size()));
	m_DeckNumber = m_StartNumber;
	GotPassives.clear();
	GotPassiveIDs.resize((int)(m_StartPassive.size()));
	GotPassiveIDs = m_StartPassive;
}
//パワーアップのエフェクトの初期化
void GameStateManager::RandPowerUpInit() {
	float posX = (float)Helper::GetRanNum(25, 200);
	float posY = (float)Helper::GetRanNum(550, 700);
	float frame = (float)Helper::GetRanNum(30, 45);
	PowerUpEffect itr;
	itr.tex = IKESprite::Create(ImageManager::POWERUP, {});
	itr.position = { posX,posY };
	itr.tex->SetAnchorPoint({ 0.5f,0.5f });
	itr.tex->SetSize(itr.size);
	itr.tex->SetColor(itr.color);
	itr.afterpos = { itr.position.x,itr.position.y - 50.0f };
	itr.kFrame = 1 / frame;
	powerup.push_back(std::move(itr));
}

void GameStateManager::PowerUpEffectUpdate() {
	for (PowerUpEffect& power : powerup) {
		if (Helper::FrameCheck(power.frame, power.kFrame)) {
			if (m_Buff) {
				RandPowerUpInit();
			}
			power.isVanish = true;
		} else {
			power.position.y = Ease(In, Exp, power.frame, power.position.y, power.afterpos.y);
			power.color.w = Ease(In, Exp, power.frame, 1.0f, 0.0f);
			power.tex->SetPosition(power.position);
			power.tex->SetColor(power.color);
		}
	}
	powerup.remove_if([](PowerUpEffect& shine) {
		return shine.isVanish; });
}

void GameStateManager::PassiveActive() {
	if (!isPassive) { return; }
	if (Helper::FrameCheck(passiveFrame, 1.f / 60.f)) {
		if (Helper::FrameCheck(passiveAlphaFrame, 1.f / 30.0f)) {
			isPassive = false;
			passiveFrame = 0.f;
			passiveAlphaFrame = 0.f;
			passiveActive->SetColor({ 1,1,1,1 });
			passiveAct->SetColor({ 1,1,1,1});
		} else {
			float alpha = Ease(In, Quint, passiveAlphaFrame, 1.f, 0.f);
			passiveActive->SetColor({ 1,1,1,alpha });
			passiveAct->SetColor({ 1,1,1,alpha });

		}
	} else {
		XMFLOAT2 size = {};
		size.x = Ease(Out, Elastic, passiveFrame, 384.f * 0.4f, 384.f);
		size.y = Ease(Out, Elastic, passiveFrame, 74.f * 0.4f, 74.f);
		passiveActive->SetSize(size);
		XMFLOAT2 size_p = {};
		size_p.x = Ease(Out, Back, passiveFrame, 0.f, 64.f);
		size_p.y = Ease(Out, Back, passiveFrame, 0.f, 64.f);
		passiveAct->SetSize(size_p);
	}
}

void GameStateManager::DamageEffectUpdate() {
	for (DamageEffect& damage : damages) {
		if (Helper::FrameCheck(damage.frame, 1 / damage.kFrame)) {
			damage.isVanish = true;
		} else {
			damage.dia = Ease(Out, Exp, damage.frame, 0.f, 100.f);
			damage.tex->SetPosition({
				damage.position.x + sinf(damage.angle) * damage.dia,
				damage.position.y - cosf(damage.angle) * damage.dia
			});
			float rot = Ease(In, Quad, damage.frame, 0.0f, 180.f);
			damage.tex->SetRotation(rot);
			float alpha = Ease(In, Quad, damage.frame, 1.0f, 0.f);
			damage.tex->SetColor({ 1,1,1,alpha });

		}
	}
	damages.remove_if([](DamageEffect& shine) {
		return shine.isVanish; });


}

void GameStateManager::DamageCheck(int Damage) {
	if (Damage > m_MaxDamage) {
		m_MaxDamage = Damage;
		resultReport->SetDealtDamage(m_MaxDamage);
	}
}

void GameStateManager::TakenDamageCheck(int Damage) {
	m_MaxTakenDamage += Damage;
	//受けた回数
	m_TakenDamageNum++;
	resultReport->SetTakenDamage(m_MaxTakenDamage);
}
void GameStateManager::SetPassiveActive(int id) {
	isPassive = true;
	passiveAct = IKESprite::Create(ImageManager::PASSIVE_00 + id, {0.f,0.f});
	passiveAct->SetPosition( { 640.f, 110.0f });
	passiveAct->SetAnchorPoint({0.5f,0.5f});
	passiveFrame = 0.f;
	passiveAlphaFrame = 0.f;
	passiveActive->SetColor({ 1,1,1,1 });
}

//捨てたカードの再シャッフル
void GameStateManager::DeckDiscard() {
	StagePanel::GetInstance()->ResetPanel();
	SkillManager::GetInstance()->DeckClear();
	//デッキに入っているカードの確認
	for (int i = 0; i < m_DiscardNumber.size(); i++) {
		SkillManager::GetInstance()->DeckCheck(m_DiscardNumber[i], i);
	}
	//デッキの最大数確認
	SkillManager::GetInstance()->SetDeckState((int)(m_DiscardNumber.size()));

	m_DiscardNumber.clear();
}
//捨てたカードの取得
void GameStateManager::GetDiscardSkill(const int ID) {
	m_DiscardNumber.push_back(ID);
}
void GameStateManager::MissAttack() {
	m_ResetPredict = true;
	m_Delay = false;
	m_DelayTimer = {};
	m_ChargeScale = 5.0f;
}
//ゲームデータをセーブする
void GameStateManager::SaveGame() {
	//普通の柱
	std::ofstream normalofs(L"Resources/csv/GameData/GameData.csv");  // ファイルパスを指定する
	normalofs << "DeckSize" << "," << m_DeckNumber.size() << std::endl;
	for (int i = 0; i < m_DeckNumber.size(); i++) {
		normalofs << "DeckNumber" << "," << m_DeckNumber[i]
			<<std::endl;
	}
	normalofs << "PassiveSize" << "," << GotPassiveIDs.size() << std::endl;
	for (int i = 0; i < GotPassiveIDs.size(); i++) {
		normalofs << "PassiveNumber" << "," << GotPassiveIDs[i]
			<< std::endl;
	}
	normalofs << "PlayerHP" << "," << savedata.m_SaveHP << std::endl;
	normalofs << "Index" << "," << savedata.m_SaveIndex << std::endl;
	normalofs << "Hierarchy" << "," << savedata.m_SaveHierarchy << std::endl;
}

void GameStateManager::OpenGameDate() {
	//攻撃スキルデータ
	savedata.m_DeckNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/GameData/GameData.csv", "DeckSize")));
	savedata.m_OpenDeckNumber.resize(savedata.m_DeckNum);
	m_DeckNumber.resize(savedata.m_DeckNum);
	LoadCSV::LoadCsvParam_Int("Resources/csv/GameData/GameData.csv", savedata.m_OpenDeckNumber, "DeckNumber");
	for (int i = 0; i < savedata.m_DeckNum; i++) {
		m_DeckNumber[i] = savedata.m_OpenDeckNumber[i];
	}

	//パッシブスキル読み取り
	savedata.m_PassiveNum = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/GameData/GameData.csv", "PassiveSize")));
	savedata.m_OpenPassiveNumber.resize(savedata.m_PassiveNum);
	GotPassiveIDs.resize(savedata.m_PassiveNum);
	LoadCSV::LoadCsvParam_Int("Resources/csv/GameData/GameData.csv", savedata.m_OpenPassiveNumber, "PassiveNumber");
	for (int i = 0; i < savedata.m_PassiveNum; i++) {
		GotPassiveIDs[i] = savedata.m_OpenPassiveNumber[i];
	}

	//HP
	savedata.m_OpenHP = static_cast<float>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/GameData/GameData.csv", "PlayerHP")));
	//マップデータ
	savedata.m_OpenHierarchy = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/GameData/GameData.csv", "Hierarchy")));
	savedata.m_OpenIndex = static_cast<int>(std::any_cast<double>(LoadCSV::LoadCsvParam("Resources/csv/GameData/GameData.csv", "Index")));

}