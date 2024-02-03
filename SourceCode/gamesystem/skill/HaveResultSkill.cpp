#include "HaveResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>
#include <SkillManager.h>
#include <Audio.h>
#include <GameStateManager.h>
#include "PassiveManager.h"

HaveResultSkill::HaveResultSkill() {

}

HaveResultSkill::~HaveResultSkill() {
}

void HaveResultSkill::Initialize(DirectXCommon* dxCommon) {
	backScreen = IKESprite::Create(ImageManager::RESULTBACKSCREEN, { 0.f,0.f }, { 1.f,1.f, 1.f, 1.0f });
	backScreen->SetSize({ 1280.f,720.f });
	top_title = IKESprite::Create(ImageManager::DECKSKILLTOP, { 640.f,70.f }, { 1.f,1.f, 1.f, 1.f });
	top_title->SetSize({ 1280.f * 0.7f,128.f * 0.7f });
	top_title->SetAnchorPoint({0.5f,0.5f});
	skillCheack = IKESprite::Create(ImageManager::RESULTBACKCHECK, { 640.f,720.f });
	skillCheack->SetAnchorPoint({ 0.5f,1.f });
	selectFrame = IKESprite::Create(ImageManager::PASSIVE_FRAME, { 200.f,200.f });
	selectFrame->SetAnchorPoint({ 0.5f,0.5f });
	selectFrame->SetSize({ 128.0f,128.0f });
	m_SelectPos = { 640.0f,250.0f };
	selectFrame->SetPosition(m_SelectPos);


	black = IKESprite::Create(ImageManager::FEED, { 0.f,0.f });
	black->SetSize({1280.f,720.f});
	black->SetColor({0.f,0.f,0.f,0.5f});
	deleteDeck = IKESprite::Create(ImageManager::DECKDELETESHEET, { 640.f,360.f });
	deleteDeck->SetAnchorPoint({0.5f,0.5f});

	const int NumberCount = 2;
	const float l_Width_Cut = 256.0f;
	const float l_Height_Cut = 64.0f;

	for (auto i = 0; i < NumberCount; i++) {
		deleteDeckYes[i] = IKESprite::Create(ImageManager::DECKDELETEOK, { 640.f + 150.f,430.f});
		deleteDeckNo[i] = IKESprite::Create(ImageManager::DECKDELETENO, { 640.f - 150.f,430.f});

		int number_index_y = i / NumberCount;
		int number_index_x = i % NumberCount;
		deleteDeckYes[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });
		deleteDeckNo[i]->SetTextureRect(
			{ static_cast<float>(number_index_x) * l_Width_Cut, static_cast<float>(number_index_y) * l_Height_Cut },
			{ static_cast<float>(l_Width_Cut), static_cast<float>(l_Height_Cut) });

		deleteDeckYes[i]->SetSize({});
		deleteDeckYes[i]->SetAnchorPoint({ 0.5f,0.5f });
		//deleteDeckYes[i]->SetColor({ 1.2f,1.2f,1.2f,1 });

		deleteDeckNo[i]->SetSize({});
		deleteDeckNo[i]->SetAnchorPoint({ 0.5f,0.5f });
		//deleteDeckNo[i]->SetColor({ 1.2f,1.2f,1.2f,1 });
	}
	deleteDeck->SetSize({});

}

void HaveResultSkill::Update() {
	for (HaveUI& PassiveUI : havePassive) {
	}
	if (m_SelectCount < (int)(haveSkills.size())) {
		for (auto i = 0; i < haveSkills[m_SelectCount].resultarea.size(); i++) {
			haveSkills[m_SelectCount].resultarea[i]->Update();
		}
	}
	//動き
	Move();
	DeleteMove();
}

void HaveResultSkill::Draw(DirectXCommon* dxCommon) {

	IKESprite::PreDraw();
	backScreen->Draw();
	top_title->Draw();
	if (m_SelectCount < (int)(haveSkills.size())&&
		haveSkills.size() != 1) {
		skillCheack->Draw();
	}
	selectFrame->Draw();
	for (HaveUI& resultUI : haveSkills) {
		resultUI.icon->Draw();
	}
	
	for (HaveUI& PassiveUI : havePassive) {
		PassiveUI.icon->Draw();
	}

	if (m_SelectCount < (int)(haveSkills.size())) {
		for (auto i = 0; i < haveSkills[m_SelectCount].resultarea.size(); i++) {
			haveSkills[m_SelectCount].resultarea[i]->Draw();
		}
		SkillManager::GetInstance()->TextDraw(haveSkills[m_SelectCount].ID);
		//for (auto i = 0; i < haveSkills.size(); i++) {
		//	
		//	//haveSkills[m_SelectCount].text_->Draw(dxCommon);
		//}
	}
	else {
		if(havePassive.size() != 0)
		PassiveManager::GetInstance()->TextDraw(havePassive[m_SelectCount - (int)(haveSkills.size())].ID);
	}
	IKESprite::PreDraw();
	if (m_DeleteCheack) {
		black->Draw();
		deleteDeck->Draw();
		deleteDeckYes[nowCheack]->Draw();
		deleteDeckNo[1 - nowCheack]->Draw();
	}
	IKESprite::PostDraw();
}
void HaveResultSkill::ImGuiDraw() {
}

//持っているスキルの検索
void HaveResultSkill::HaveAttackSkill(std::vector<int> Deck,
	int DeckSize, DirectXCommon* dxCommon) {
	haveSkills.resize(DeckSize);
	for (auto i = 0; i < haveSkills.size(); i++) {
		haveSkills[i].ID = Deck[i];
		SkillManager::GetInstance()->HandResultData(Deck[i], haveSkills[i].area, haveSkills[i].DisX, haveSkills[i].DisY,haveSkills[i].Damage);//IDに応じた攻撃エリア、距離を取得する
		CreateAttackSkill(i, haveSkills[i].ID, dxCommon);
		BirthArea(i);		//エリアを作成(持ってるスキル分)
	}
}
//持っているパッシブ
void HaveResultSkill::HavePassiveSkill(std::vector<int> Passive,
	int PassiveSize, DirectXCommon* dxCommon) {
	havePassive.resize(PassiveSize);
	for (auto i = 0; i < havePassive.size(); i++) {
		havePassive[i].ID = Passive[i];
		CreatePassiveSkill(i, havePassive[i].ID,dxCommon);
	}
}
//攻撃スキルの表示
void HaveResultSkill::CreateAttackSkill(const int num,const int id, DirectXCommon* dxCommon) {
	XMFLOAT2 l_BasePos = { 640.0f,250.0f };
	haveSkills[num].position = { l_BasePos.x + (num * 150.0f),l_BasePos.y };
	haveSkills[num].icon = IKESprite::Create(ImageManager::ATTACK_0 + id, { 0.0f,0.0f });
	haveSkills[num].icon->SetSize({ 100.0f,100.0f });
	haveSkills[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	haveSkills[num].icon->SetPosition(haveSkills[num].position);
	haveSkills[num].icon->SetColor({ 1.3f,1.3f,1.3f,1.0f });
	haveSkills[num].Delay = SkillManager::GetInstance()->GetDelay(id);
	SkillManager::GetInstance()->LoadText(id);
	/*haveSkills[num].text_ = make_unique<TextManager>();
	haveSkills[num].text_->Initialize(dxCommon,LOAD_ATTACK);
	haveSkills[num].text_->SetConversation(TextManager::RESULT, { -250.0f,80.0f });
	haveSkills[num].baseSentence[0] = L"スキル：";
	haveSkills[num].baseSentence[1] = haveSkills[num].text_->GetSkillSentence(haveSkills[num].ID);
	haveSkills[num].baseSentence[2] = haveSkills[num].text_->GetSkillDamage(haveSkills[num].ID);
	haveSkills[num].text_->SetCreateSentence(haveSkills[num].baseSentence[0], haveSkills[num].baseSentence[1], haveSkills[num].baseSentence[2]);*/
	m_SelectCount = {};
	m_AddPosX = {};
}
//パッシブスキルの表示
void HaveResultSkill::CreatePassiveSkill(const int num, const int id, DirectXCommon* dxCommon) {
	XMFLOAT2 l_BasePos = { 640.0f,250.0f };
	havePassive[num].position = { l_BasePos.x + ((num + (int)haveSkills.size()) * 150.0f),l_BasePos.y };
	havePassive[num].icon = IKESprite::Create(ImageManager::PASSIVE_00 + havePassive[num].ID, {0.0f,0.0f});
	havePassive[num].icon->SetSize({ 64.0f,64.0f });
	havePassive[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	havePassive[num].icon->SetPosition(havePassive[num].position);
	PassiveManager::GetInstance()->LoadText(id);
	//havePassive[num].text_ = make_unique<TextManager>();
	//havePassive[num].text_->Initialize(dxCommon,LOAD_PASSIVE);
	//havePassive[num].text_->SetConversation(TextManager::RESULT, { -250.0f,80.0f });
	//havePassive[num].baseSentence[0] = L"パッシブ：";
	//havePassive[num].baseSentence[1] = havePassive[num].text_->GetPassiveName(havePassive[num].ID);
	//havePassive[num].baseSentence[2] = havePassive[num].text_->GetPasiveSentence(havePassive[num].ID);
	//havePassive[num].text_->SetCreateSentence(havePassive[num].baseSentence[0], havePassive[num].baseSentence[1], havePassive[num].baseSentence[2]);
}
//移動
void HaveResultSkill::Move() {
	if (DeleteCheack()) { return; }
	Input* input = Input::GetInstance();
	if (m_isMove) {
		static float frame = 0.f;
		static float addFrame = 1.f / 15.f;

		if (Helper::FrameCheck(frame, addFrame)) {
			m_isMove = false;
			frame = 0.f;
		}
		else {
			m_AddPosX = Ease(InOut, Circ, frame, m_AddPosX, 150.0f * m_SelectCount);
		}
		for (auto i = 0; i < haveSkills.size(); i++) {
			haveSkills[i].icon->SetPosition({ haveSkills[i].position.x - m_AddPosX,haveSkills[i].position.y });
		}

		for (auto i = 0; i < havePassive.size(); i++) {
			havePassive[i].icon->SetPosition({ havePassive[i].position.x - m_AddPosX,havePassive[i].position.y });
		}
	}

	if (input->TiltPushStick(input->L_LEFT) ||
		input->TiltPushStick(input->L_RIGHT) ||
		input->TriggerKey(DIK_A) ||
		input->TriggerKey(DIK_D) ||
		input->PushButton(input->LEFT) ||
		input->PushButton(input->RIGHT)) {
		if (m_isMove) { return; }
		if (input->TiltPushStick(input->L_RIGHT) ||
			input->TriggerKey(DIK_D) ||
			input->PushButton(input->RIGHT)) {
			if (m_SelectCount == ((int)(haveSkills.size()) + (int)(havePassive.size())) - 1) { return; }
			m_SelectCount++;
		}
		else {
			if (m_SelectCount == 0) { return; }
			m_SelectCount--;
		}
		for (HaveUI& itr : haveSkills) {
			for (std::unique_ptr<ResultAreaUI>& pickAreas : itr.resultarea) {
				pickAreas->ResetTimer();
			}
		}
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Cursor.wav", 0.1f);
		m_isMove = true;
	}

	//持ってるスキルの削除
	if (m_SelectCount < (int)(haveSkills.size())) {
		if ((input->TriggerKey(DIK_SPACE) || input->TriggerButton(input->X))&&
			!m_isMove &&
			haveSkills.size() != 1) {
			m_DeleteCheack = true;
			m_DeleteStart = true;
			m_Open = true;
			deleteFrame = 0.f;
			///
			//　ここに削除決定音（音入）
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/deletion.wav", 0.02f);
			///
		}
	}
}
//エリアの生成
void HaveResultSkill::BirthArea(const int Area) {
	for (auto i = 0; i < haveSkills[Area].area.size(); i++) {
		for (auto j = 0; j < haveSkills[Area].area.size(); j++) {		
			if (haveSkills[Area].area[i][j] == 1) {		//マップチップ番号とタイルの最大数、最小数に応じて描画する
				std::unique_ptr<ResultAreaUI> newarea = std::make_unique<ResultAreaUI>();
				newarea->SetPanelNumber(i, j);
				newarea->SetDelay(haveSkills[Area].Delay);
				newarea->SetId(haveSkills[Area].ID);
				newarea->SetDistance(haveSkills[Area].DisX, haveSkills[Area].DisY);
				newarea->Initialize();
				haveSkills[Area].resultarea.push_back(std::move(newarea));
			}
		}
	}
}
//デリート後のイージング後のポジション
void HaveResultSkill::SetDeleteAfter(const int num) {
	XMFLOAT2 l_BasePos = { 640.0f,250.0f };
	haveSkills[num].afterpos.x = { l_BasePos.x + (num * 150.0f) };
	
	if(havePassive.size () == 0){
		if (m_SelectCount == haveSkills.size()) {
			m_AfterAddPosX = (m_SelectCount - 1) * 150.0f;
			m_AddPosX = (m_SelectCount) * 150.0f;
		}
	}
}
void HaveResultSkill::SetPassiveDeleteAfter(const int num) {
	XMFLOAT2 l_BasePos = { 640.0f,250.0f };
	if (havePassive.size() != 0) {
		havePassive[num].afterpos.x = { l_BasePos.x + ((num + (int)haveSkills.size()) * 150.0f) };
	}
}
//削除されたときの動き
void HaveResultSkill::DeleteMove() {
	if (!m_DeleteMove) { return; }
	
	static float frame = 0.f;
	static float addFrame = 1.f / 15.f;

	if (Helper::FrameCheck(frame, addFrame)) {
		if (m_SelectCount == haveSkills.size() && havePassive.size() == 0) {
			m_SelectCount--;
		}
		m_DeleteMove = false;
		frame = 0.f;
	}
	//イージングで動かす
	for (auto i = 0; i < haveSkills.size(); i++) {
		haveSkills[i].position.x = Ease(In, Cubic, frame, haveSkills[i].position.x, haveSkills[i].afterpos.x);
		haveSkills[i].icon->SetPosition({ haveSkills[i].position.x - m_AddPosX,haveSkills[i].position.y });
	}

	if (havePassive.size() != 0) {
		for (auto i = 0; i < havePassive.size(); i++) {
			havePassive[i].position.x = Ease(In, Cubic, frame, havePassive[i].position.x, havePassive[i].afterpos.x);
			havePassive[i].icon->SetPosition({ havePassive[i].position.x - m_AddPosX,havePassive[i].position.y });
		}
	}
	else {
		if (m_SelectCount == haveSkills.size()) {
			m_AddPosX = Ease(In, Cubic, frame, m_AddPosX, m_AfterAddPosX);
			for (auto i = 0; i < haveSkills.size(); i++) {
				haveSkills[i].icon->SetPosition({ haveSkills[i].position.x - m_AddPosX,haveSkills[i].position.y });
			}
		}
	}
}

bool HaveResultSkill::DeleteCheack() {
	if (!m_DeleteCheack) {
		return false;
	}
	if (m_DeleteStart) {
		float kEndFrame = 1 / 30.f;
		if (!m_Open) {
			kEndFrame = 1 / 15.f;
		}
		if (Helper::FrameCheck(deleteFrame, kEndFrame)) {
			m_DeleteStart = false;
			if (!m_Open) {
				m_DeleteCheack = false;
			}
		} else {
			if (m_Open) {
				XMFLOAT2 size = {
				Ease(Out,Back,deleteFrame,0.f,256.f),
				Ease(Out,Back,deleteFrame,0.f,64.f)
				};
				XMFLOAT2 size_2 = {
				Ease(Out,Back,deleteFrame,0.f,640.f),
				Ease(Out,Back,deleteFrame,0.f,328.f)
				};
				deleteDeck->SetSize(size_2);
				for (int i = 0; i < 2; i++) {
					deleteDeckYes[i]->SetSize(size);
					deleteDeckNo[i]->SetSize(size);
				}
			}
			else {
				XMFLOAT2 size = {
			Ease(Out,Exp,deleteFrame,256.f,0.f),
			Ease(Out,Exp,deleteFrame,64.f,0.f)
				};
				XMFLOAT2 size_2 = {
				Ease(Out,Exp,deleteFrame,640.f,0.f),
				Ease(Out,Exp,deleteFrame,328.f,0.f)
				};
				deleteDeck->SetSize(size_2);
				for (int i = 0; i < 2; i++) {
					deleteDeckYes[i]->SetSize(size);
					deleteDeckNo[i]->SetSize(size);
				}
			}
		}
		return true;
	}
	Input* input = Input::GetInstance();
	if (input->TiltStick(input->L_LEFT) ||
		input->TiltStick(input->L_RIGHT) ||
		input->TriggerKey(DIK_A) ||
		input->TriggerKey(DIK_D) ||
		input->TriggerButton(input->LEFT) ||
		input->TriggerButton(input->RIGHT)) {

		if (input->TiltStick(input->L_RIGHT) ||
			input->TriggerKey(DIK_D) ||
			input->TriggerButton(input->RIGHT)) {
			nowCheack = 1;
		} else {
			nowCheack = 0;
		}

		///
		//　ここに選択音（音入）
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/selection.wav", 0.02f);
		///
	}


	if ((input->TriggerButton(Input::B) ||
		input->TriggerKey(DIK_SPACE))) {
		if (nowCheack==1) {
			haveSkills.erase(cbegin(haveSkills) + m_SelectCount);
			GameStateManager::GetInstance()->DeleteDeck(m_SelectCount);
			for (int i = 0; i < haveSkills.size(); i++) {
				SetDeleteAfter(i);
			}
			for (int i = 0; i < havePassive.size(); i++) {
				SetPassiveDeleteAfter(i);
			}
			m_DeleteMove = true;
			nowCheack = 0;
			///
			//　ここに削除音（音入）
			Audio::GetInstance()->PlayWave("Resources/Sound/SE/deletionWinPop.wav", 0.02f);
			m_DeleteCheack = false;
			///
		} else {
			deleteFrame = {};
			m_DeleteStart = true;
			m_Open = false;
			///
			//　ここにキャンセル音（音入）
			//　一旦なし
			///
		}
		deleteDeck->SetSize({});
		for (int i = 0; i < 2; i++) {
			deleteDeckYes[i]->SetSize({});
			deleteDeckNo[i]->SetSize({});
		}

		return true;
	}
	return true;
}
