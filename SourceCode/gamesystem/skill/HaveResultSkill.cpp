#include "HaveResultSkill.h"
#include "Helper.h"
#include "ImageManager.h"
#include <Input.h>
#include <Easing.h>
#include <SkillManager.h>
#include <Audio.h>
#include <GameStateManager.h>
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
}

void HaveResultSkill::Update() {
	for (HaveUI& PassiveUI : havePassive) {
	}
	if (m_SelectCount < (int)(haveSkills.size())) {
		for (auto i = 0; i < haveSkills[m_SelectCount].resultarea.size(); i++) {
			haveSkills[m_SelectCount].resultarea[i]->Update();
		}
	}
	//����
	Move();
	DeleteMove();
}

void HaveResultSkill::Draw(DirectXCommon* dxCommon) {

	IKESprite::PreDraw();
	backScreen->Draw();
	top_title->Draw();
	if (m_SelectCount < (int)(haveSkills.size())) {
		skillCheack->Draw();
	}
	selectFrame->Draw();
	for (HaveUI& resultUI : haveSkills) {
		resultUI.icon->Draw();
	}
	
	for (HaveUI& PassiveUI : havePassive) {
		PassiveUI.icon->Draw();
	}

	//resulttext->TestDraw(dxCommon);
	if (m_SelectCount < (int)(haveSkills.size())) {
		for (auto i = 0; i < haveSkills[m_SelectCount].resultarea.size(); i++) {
			haveSkills[m_SelectCount].resultarea[i]->Draw();
		}
		for (auto i = 0; i < haveSkills.size(); i++) {
			haveSkills[m_SelectCount].text_->Draw(dxCommon);
		}
	}
	else {
		for (auto i = 0; i < havePassive.size(); i++) {
			havePassive[m_SelectCount - (int)(haveSkills.size())].text_->Draw(dxCommon);
		}
	}
	IKESprite::PostDraw();
}
void HaveResultSkill::ImGuiDraw() {
	ImGui::Begin("Have");
	ImGui::Text("PosX:%f", haveSkills[haveSkills.size() - 1].position.x);
	ImGui::Text("SelectPos:%f",m_AddPosX);
	ImGui::Text("AfterPos:%f", m_AfterAddPosX);
	ImGui::Text("SelectCount:%d", m_SelectCount);
	ImGui::Text("Size:%d", (int)haveSkills.size());
	ImGui::End();
}

//�����Ă���X�L���̌���
void HaveResultSkill::HaveAttackSkill(std::vector<int> Deck,
	int DeckSize, DirectXCommon* dxCommon) {
	haveSkills.resize(DeckSize);
	for (auto i = 0; i < haveSkills.size(); i++) {
		haveSkills[i].ID = Deck[i];
		SkillManager::GetInstance()->HandResultData(Deck[i], haveSkills[i].area, haveSkills[i].DisX, haveSkills[i].DisY,haveSkills[i].Damage);//ID�ɉ������U���G���A�A�������擾����
		CreateAttackSkill(i, haveSkills[i].ID, dxCommon);
		BirthArea(i);		//�G���A���쐬(�����Ă�X�L����)
	}
}
//�����Ă���p�b�V�u
void HaveResultSkill::HavePassiveSkill(std::vector<int> Passive,
	int PassiveSize, DirectXCommon* dxCommon) {
	havePassive.resize(PassiveSize);
	for (auto i = 0; i < havePassive.size(); i++) {
		havePassive[i].ID = Passive[i];
		CreatePassiveSkill(i, havePassive[i].ID,dxCommon);
	}
}
//�U���X�L���̕\��
void HaveResultSkill::CreateAttackSkill(const int num,const int id, DirectXCommon* dxCommon) {
	XMFLOAT2 l_BasePos = { 640.0f,250.0f };
	haveSkills[num].position = { l_BasePos.x + (num * 150.0f),l_BasePos.y };
	haveSkills[num].icon = IKESprite::Create(ImageManager::ATTACK_0 + id, { 0.0f,0.0f });
	haveSkills[num].icon->SetSize({ 100.0f,100.0f });
	haveSkills[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	haveSkills[num].icon->SetPosition(haveSkills[num].position);
	haveSkills[num].icon->SetColor({ 1.3f,1.3f,1.3f,1.0f });
	haveSkills[num].Delay = SkillManager::GetInstance()->GetDelay(id);
	haveSkills[num].text_ = make_unique<TextManager>();
	haveSkills[num].text_->Initialize(dxCommon,LOAD_ATTACK);
	haveSkills[num].text_->SetConversation(TextManager::RESULT, { -250.0f,80.0f });
	haveSkills[num].baseSentence[0] = L"�X�L���F";
	haveSkills[num].baseSentence[1] = haveSkills[num].text_->GetSkillSentence(haveSkills[num].ID);
	haveSkills[num].baseSentence[2] = haveSkills[num].text_->GetSkillDamage(haveSkills[num].ID);
	haveSkills[num].text_->SetCreateSentence(haveSkills[num].baseSentence[0], haveSkills[num].baseSentence[1], haveSkills[num].baseSentence[2]);
	m_SelectCount = {};
	m_AddPosX = {};
}
//�p�b�V�u�X�L���̕\��
void HaveResultSkill::CreatePassiveSkill(const int num, const int id, DirectXCommon* dxCommon) {
	XMFLOAT2 l_BasePos = { 640.0f,250.0f };
	havePassive[num].position = { l_BasePos.x + ((num + (int)haveSkills.size()) * 150.0f),l_BasePos.y };
	havePassive[num].icon = IKESprite::Create(ImageManager::PASSIVE_00 + havePassive[num].ID, {0.0f,0.0f});
	havePassive[num].icon->SetSize({ 64.0f,64.0f });
	havePassive[num].icon->SetAnchorPoint({ 0.5f,0.5f });
	havePassive[num].icon->SetPosition(havePassive[num].position);
	havePassive[num].text_ = make_unique<TextManager>();
	havePassive[num].text_->Initialize(dxCommon,LOAD_PASSIVE);
	havePassive[num].text_->SetConversation(TextManager::RESULT, { -250.0f,80.0f });
	havePassive[num].baseSentence[0] = L"�p�b�V�u�F";
	havePassive[num].baseSentence[1] = havePassive[num].text_->GetPassiveName(havePassive[num].ID);
	havePassive[num].baseSentence[2] = havePassive[num].text_->GetPasiveSentence(havePassive[num].ID);
	havePassive[num].text_->SetCreateSentence(havePassive[num].baseSentence[0], havePassive[num].baseSentence[1], havePassive[num].baseSentence[2]);
}
//�ړ�
void HaveResultSkill::Move() {
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

	//�����Ă�X�L���̍폜
	if (m_SelectCount < (int)(haveSkills.size())) {
		if (input->TriggerButton(input->X) && haveSkills.size() != 1) {
			haveSkills.erase(cbegin(haveSkills) + m_SelectCount);
			GameStateManager::GetInstance()->DeleteDeck(m_SelectCount);
			for (int i = 0; i < haveSkills.size(); i++) {
				SetDeleteAfter(i);
			}
			for (int i = 0; i < havePassive.size(); i++) {
				SetPassiveDeleteAfter(i);
			}
			m_DeleteMove = true;
		}
	}
}
//�G���A�̐���
void HaveResultSkill::BirthArea(const int Area) {
	for (auto i = 0; i < haveSkills[Area].area.size(); i++) {
		for (auto j = 0; j < haveSkills[Area].area.size(); j++) {		
			if (haveSkills[Area].area[i][j] == 1) {		//�}�b�v�`�b�v�ԍ��ƃ^�C���̍ő吔�A�ŏ����ɉ����ĕ`�悷��
				std::unique_ptr<ResultAreaUI> newarea = std::make_unique<ResultAreaUI>();
				newarea->SetPanelNumber(i, j);
				newarea->SetDelay(haveSkills[Area].Delay);
				newarea->SetDistance(haveSkills[Area].DisX, haveSkills[Area].DisY);
				newarea->Initialize();
				haveSkills[Area].resultarea.push_back(std::move(newarea));
			}
		}
	}
}
//�f���[�g��̃C�[�W���O��̃|�W�V����
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
//�폜���ꂽ�Ƃ��̓���
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
	//�C�[�W���O�œ�����
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