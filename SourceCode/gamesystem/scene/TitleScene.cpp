#include "TitleScene.h"
#include "input.h"
#include "ImageManager.h"
#include <StagePanel.h>
#include "MobEnemy.h"
#include "GameStateManager.h"
#include "TextManager.h"
#include <StageBack.h>
#include <Helper.h>

void (TitleScene::* TitleScene::stateTable[])() = {
	&TitleScene::Wait,//動きの合間
	&TitleScene::OpenMagic,//動きの合間
	&TitleScene::VerseOnomato,//瞬間移動
	&TitleScene::Verse2Onomato,//瞬間移動
	&TitleScene::CloseMagic
};
//初期化
void TitleScene::Initialize(DirectXCommon* dxCommon) {
	//共通の初期化
	BaseInitialize(dxCommon);
	dxCommon->SetFullScreen(true);

	Audio::GetInstance()->LoopWave(AUDIO_MAIN, 0.02f);
	if (!s_GameLoop) {
		SceneChanger::GetInstance()->Initialize();
		s_GameLoop = true;
	}

	nowHierarchy = 0;
	clearHierarchy = 1;
	nowIndex = 1;
	s_LastStage = false;

	////�X�e�[�W�̏�
//// プレイヤー生成
//{
//	auto player = GameObject::CreateObject<Player>();	// �v���C���[����
//	player->LoadResource();
//	player->InitState({ -4.0f,0.1f,2.0f });
//	player->Initialize();
//	player->SetTitleFlag(true);

//	GameStateManager::GetInstance()->SetPlayer(player);
//}
	player_ = make_unique<Player>();
	player_->LoadResource();
	player_->SetTitleFlag(true);
	player_->InitState({ -PANEL_SIZE * 2.f,0.1f,PANEL_SIZE });
	player_->Initialize();
	player_->SkipInitialize();
	StagePanel::GetInstance()->LoadResource();
	StagePanel::GetInstance()->SetPlayer(player_.get());
	StagePanel::GetInstance()->Initialize();
	//背景画像
	StageBack::GetInstance()->LoadResource();

	////敵
	InterEnemy::SetPlayer(player_.get());
	enemy = make_unique<MobEnemy>();
	enemy->Initialize();

	//カード
	title_ = IKESprite::Create(ImageManager::TITLETEXT, { 0.0f,0.0f });
	player = IKESprite::Create(ImageManager::MAP_CHARA, { 0.0f,0.0f });
	player->SetAnchorPoint({ 0.5f,1.0f });
	player->SetSize({ 256.f,256.f });
	player->SetPosition({ 640.f - 128.f,360.f + 128.f });
	for (int i = 0; i < 11; i++) {
		unique_ptr<IKESprite> passive = IKESprite::Create(ImageManager::PASSIVE_00 + i, { 0,0 });
		passive->SetSize({ 86.f,86.f });
		//passive->SetAnchorPoint({0.5f,0.5f});
		passive->SetPosition({ 128.0f * i,20.0f });
		passives.push_back(std::move(passive));
	}

	for (int i = 0; i < 11; i++) {
		unique_ptr<IKESprite> skill = IKESprite::Create(ImageManager::ATTACK_0 + i, { 0,0 });
		skill->SetSize({ 86.f,86.f });
		//passive->SetAnchorPoint({0.5f,0.5f});
		skill->SetPosition({ 128.0f * i,614.0f });
		skills.push_back(std::move(skill));
	}
	magic_ = IKESprite::Create(ImageManager::PLAYERPREDICT2D, { 700.f,380.f });
	magic_->SetAnchorPoint({ 0.5f,0.5f });
	magic_->SetSize(magicbaseSize);
	magic_->SetSize({});
	onomatope_ = IKESprite::Create(ImageManager::ONOMATO_03, { 700.f,380.f });
	onomatope_->SetAnchorPoint({ 0.f,0.5f });
	onomatope_->SetSize({});
	//onomatope_
	onomatope2_ = IKESprite::Create(ImageManager::ONOMATO_07, { 0.0f,0.0f });
	onomatope2_->SetSize({});
	onomatope2_->SetAnchorPoint({ 0.f,0.5f });



	GameStateManager::GetInstance()->DeckReset();
	GameStateManager::GetInstance()->SetGameStart(true);
	GameStateManager::GetInstance()->SetNoDeath(true);
	GameStateManager::GetInstance()->SetNoDamage(true);
}
//更新
void TitleScene::Update(DirectXCommon* dxCommon) {
	Input* input = Input::GetInstance();
	camerawork->Update(camera); 
	ParticleUpdate();
	player_->Update();
	//各クラス更新
	camerawork->Update(camera);

	(this->*stateTable[static_cast<size_t>(state)])();

	lightGroup->Update();
	StagePanel::GetInstance()->Update();
	SceneChanger::GetInstance()->Update();
	enemy->Update();

	rota2 += 3.f * XM_PI / 180.f;
	float ease_size = Ease(InOut, Cubic, abs(sinf(rota2)), 0.97f, 1.f);
	float ease_size2 = Ease(InOut, Quint, abs(sinf(rota2)), 0.97f, 1.f);
	player->SetSize({ 256.f * ease_size,256.f * ease_size2 });

	for (unique_ptr<IKESprite>& passive : passives) {
		XMFLOAT2 pos = passive->GetPosition();
		pos.x += 2.f;
		if (pos.x >= 1280.f) {
			pos.x = -128.f;
		}
		passive->SetPosition(pos);
	}
	for (unique_ptr<IKESprite>& skill : skills) {
		XMFLOAT2 pos = skill->GetPosition();
		pos.x -= 2.f;
		if (pos.x <= -128.f) {
			pos.x = 1280.f;
		}
		skill->SetPosition(pos);
	}
	if ((input->TriggerButton(input->B) || input->TriggerKey(DIK_SPACE)) && (!SceneChanger::GetInstance()->GetChangeStart())) {			//バトル
		SceneChanger::GetInstance()->SetChangeStart(true);
		_SceneType = PLAY;
		Audio::GetInstance()->PlayWave("Resources/Sound/SE/Button.wav", 0.15f);
	}

	if (SceneChanger::GetInstance()->GetChange()) {			//真っ暗になったら変わる
		player_->PlayerSave();
		SceneManager::GetInstance()->ChangeScene("MAP");
		SceneChanger::GetInstance()->SetChange(false);
	}

	if (input->TriggerButton(input->X)) {
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	if (input->TriggerButton(input->Y)) {
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
}
//描画
void TitleScene::Draw(DirectXCommon* dxCommon) {
	//ポストエフェクトをかけるか
	if (PlayPostEffect) {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		postEffect->Draw(dxCommon->GetCmdList());
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	} else {
		postEffect->PreDrawScene(dxCommon->GetCmdList());
		postEffect->Draw(dxCommon->GetCmdList());
		postEffect->PostDrawScene(dxCommon->GetCmdList());
		dxCommon->PreDraw();
		BackDraw(dxCommon);
		FrontDraw(dxCommon);
		ImGuiDraw(dxCommon);
		dxCommon->PostDraw();
	}
}
//前面描画
void TitleScene::FrontDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	title_->Draw();
	IKESprite::PostDraw();
	SceneChanger::GetInstance()->Draw();

}

//背面描画
void TitleScene::BackDraw(DirectXCommon* dxCommon) {
	IKESprite::PreDraw();
	//title_[TITLE_BACK]->Draw();
	StageBack::GetInstance()->Draw(dxCommon);
	player->Draw();

	magic_->Draw();
	onomatope_->Draw();
	onomatope2_->Draw();

	for (unique_ptr<IKESprite>& passive : passives) {
		passive->Draw();
	}
	for (unique_ptr<IKESprite>& skill : skills) {
		skill->Draw();
	}

	for (ShineEffect& shine : shines) {
		shine.tex->Draw();
	}
	IKESprite::PostDraw();

	//enemy->Draw(dxCommon);
}
//ImGui描画
void TitleScene::ImGuiDraw(DirectXCommon* dxCommon) {
}
//解放
void TitleScene::Finalize() {
}

void TitleScene::Wait() {
	stateCount++;
	float frame = (float)stateCount / (float)30.f;
	Helper::Clamp(frame, 0.f, 1.0f);
	float alpha = Ease(In, Quad, frame, 1.f, 0);
	onomatope2_->SetColor({ 1,1,1,alpha });

	if (stateCount >= kWaitCountMax) {
		state = STATE::openMagic;
		stateCount = 0;
	}
}
void TitleScene::OpenMagic() {
	stateCount++;
	float frame = (float)stateCount / (float)kOpenMagicCountMax;
	XMFLOAT2 size = {
		Ease(Out,Back,frame,0.f,magicbaseSize.x),
		magicbaseSize.y
	};
	float rotMagic = Ease(In, Circ, frame, 0.f, 90.f);
	magic_->SetSize(size);
	magic_->SetRotation(rotMagic);
	if (frame >= 1.0f) {
		int r_num = Helper::GetRanNum(0, 99);
		if (r_num < 50) {
			state = STATE::verseOnomato;
		} else {
			state = STATE::verse2Onomato;
		}
		onomatope2_->SetColor({ 1,1,1,1 });
		onomatope2_->SetPosition({ 0.f, -200.f });
		stateCount = 0;
	}
}
void TitleScene::VerseOnomato() {
	stateCount++;
	float frame = (float)stateCount / (float)kVerseOnomatoCountMax;
	float frame2 = frame * 2.0f;
	Helper::Clamp(frame2, 0.f, 1.0f);
	XMFLOAT2 size = {
		256.f,
		Ease(Out,Exp,frame2,0.f,256.f)
	};
	XMFLOAT2 pos = {
	Ease(Out,Quad,frame,700.f,1450.f),
	380.f
	};
	if (pos.x >= 1280.f&& !isEffect) {
		for (int i = 0; i < 5; i++) {
			ShineInit({ 1280.f,pos.y });
		}
		isEffect = true;
	}
	onomatope_->SetSize(size);
	onomatope_->SetPosition(pos);
	if (stateCount >= kVerseOnomatoCountMax) {
		isEffect = false;
		state = STATE::closeMagic;
		stateCount = 0;
	}
}

void TitleScene::Verse2Onomato() {
	stateCount++;
	float frame = (float)stateCount / (float)kVerseOnomatoCountMax;
	float frame2 = frame * 2.0f;
	Helper::Clamp(frame2, 0.f, 1.0f);
	XMFLOAT2 size = {
		256.f,
		Ease(Out,Exp,frame2,0.f,256.f)
	};
	XMFLOAT2 pos = {
	875.f,
	Ease(In,Bounce,frame,-200.f,450.f)
	};

	onomatope2_->SetSize(size);
	onomatope2_->SetPosition(pos);
	if (stateCount >= kVerseOnomatoCountMax) {
			for (int i = 0; i < 5; i++) {
				Shine2Init({ pos.x+128.f,pos.y+128.f });
			}

		state = STATE::closeMagic;
		stateCount = 0;
	}
}
void TitleScene::CloseMagic() {
	stateCount++;
	float frame = (float)stateCount / (float)kCloseMagicCountMax;
	XMFLOAT2 size = {
		magicbaseSize.x,
		Ease(Out,Exp,frame,magicbaseSize.y,0)
	};
	magic_->SetSize(size);
	if (frame >= 1.0f) {
		state = STATE::wait;
		stateCount = 0;
	}
}

void TitleScene::ShineInit(XMFLOAT2 pos) {
	const float marzin = 20.0f;
	XMFLOAT2 r_vel = {
		-5.0f,
		(float)rand() / RAND_MAX * marzin - marzin / 2.0f
	};

	ShineEffect shine;
	shine.tex = IKESprite::Create(ImageManager::SHINE_S, { -100.f,0.f });
	shine.tex->SetAnchorPoint({ 0.5f,0.5f });
	shine.vel = r_vel;
	shine.position = pos;
	shine.size = { 32.f,32.f };
	shine.tex->SetSize(shine.size);

	shines.push_back(std::move(shine));
}

void TitleScene::Shine2Init(XMFLOAT2 pos) {
	const float marzin = 20.0f;
	XMFLOAT2 r_vel = {
		(float)rand() / RAND_MAX * marzin - marzin / 2.0f,
		(float)rand() / RAND_MAX * marzin - marzin / 2.0f
	};

	ShineEffect shine;
	shine.tex = IKESprite::Create(ImageManager::SMOKE, { -100.f,0.f });
	shine.tex->SetAnchorPoint({ 0.5f,0.5f });
	shine.vel = r_vel;
	shine.position = pos;
	shine.size = { 32.f,32.f };
	shine.tex->SetSize(shine.size);

	shines.push_back(std::move(shine));
}

void TitleScene::ParticleUpdate() {
	for (ShineEffect& shine : shines) {
		if (Helper::FrameCheck(shine.frame, 1 / shine.kFrame)) {
			shine.isVanish = true;
		} else {
			shine.position.x += shine.vel.x;
			shine.position.y += shine.vel.y;
			shine.tex->SetPosition({shine.position});

			float alpha = Ease(In, Quad, shine.frame, 1.0f, 0.f);
			shine.tex->SetColor({ 1,1,1,alpha });
		}
	}
	shines.remove_if([](ShineEffect& shine) {
		return shine.isVanish; });




}
