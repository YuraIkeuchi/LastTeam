#include "Onomatope.h"
#include "Easing.h"
#include "ImageManager.h"
#include "Helper.h"


void (Onomatope::* Onomatope::updateTable[])(OnomatoStruct& onomato) = {
	&Onomatope::FootUpdate,//
	&Onomatope::BurnUpdate,//
	&Onomatope::BossSpawnUpdate,//
	&Onomatope::GameOverUpdate,//
	&Onomatope::AttackChargeUpdate,//
	& Onomatope::CounterUpdate,
};

Onomatope::Onomatope() {
}

Onomatope::~Onomatope() {
}

void Onomatope::Update() {

	for (OnomatoStruct& onomato : onomatoList) {
		(this->*updateTable[(size_t)onomato.pattern])(onomato);
	}
	onomatoList.remove_if([](OnomatoStruct& onomato){
		return onomato.isFinish; });
}

void Onomatope::Draw() {
	IKESprite::PreDraw();
	for (OnomatoStruct& onomato : onomatoList) {
		onomato.Tex->Draw();
	}
	IKESprite::PostDraw();
}

void Onomatope::AddOnomato(OnomatoPattern patten, XMFLOAT2 basePos, float delay) {
	OnomatoStruct str;
	switch (patten) {
	case Foot:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_00, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 0.f,256.f });
		str.kFrameMax = 15.f;
		str.kDelayFrameMax = delay;
		str.pattern = Foot;
		break;
	case Attack01:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_01, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 0.f,256.f });
		str.kFrameMax = 45.f;
		str.kDelayFrameMax = delay;
		str.pattern = Attack01;
		break;
	case BossSpawn:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_01, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 0.f,256.f });
		str.kFrameMax = 45.f;
		str.kDelayFrameMax = delay;
		str.pattern = BossSpawn;
		break;
	case GameOver:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_01, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 0.f,256.f });
		str.kFrameMax = 45.f;
		str.kDelayFrameMax = delay;
		str.pattern = GameOver;
		break;
	case AttackCharge:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_04, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 256.f,256.f });
		str.kFrameMax = 45.f;
		str.kDelayFrameMax = delay;
		str.pattern = AttackCharge;
		break;
	case Counter:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_05, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 256.f,256.f });
		str.kFrameMax = 60.f;
		str.kDelayFrameMax = delay;
		str.pattern = Counter;
		break;
	default:
		break;
	}
	str.basePos = basePos;

	onomatoList.push_back(std::move(str));
}

void Onomatope::FootUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;

	if (onomato.frame <= 1.0f) {
		onomato.Tex->SetColor({ 1.f,1.f,1.f,1.f });
		XMFLOAT2 siz = onomato.Tex->GetSize();
		siz.x = Ease(Out, Quad, onomato.frame, 0.f, 256.f);
		onomato.Tex->SetSize(siz);
		XMFLOAT2 pos = onomato.Tex->GetPosition();
		pos.x = Ease(Out, Quad, onomato.frame, onomato.basePos.x, onomato.basePos.x +320.f);
		pos.y = Ease(Out, Quad, onomato.frame, onomato.basePos.y, onomato.basePos .y-180.0f);
		onomato.Tex->SetPosition(pos);
	} else {
		onomato.isFinish = true;
	}
}

void Onomatope::BurnUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;

	if (onomato.frame <= 1.0f) {
		onomato.Tex->SetColor({ 1.f,1.f,1.f,1.f });
		XMFLOAT2 siz = onomato.Tex->GetSize();
		siz.x = Ease(Out, Quad, onomato.frame, 0.f, 256.f);
		onomato.Tex->SetSize(siz);
		XMFLOAT2 pos = onomato.Tex->GetPosition();
		pos.x = Ease(Out, Quad, onomato.frame, onomato.basePos.x, onomato.basePos.x + 320.f);
		pos.y = Ease(Out, Quad, onomato.frame, onomato.basePos.y, onomato.basePos.y - 180.0f);
		onomato.Tex->SetPosition(pos);
	} else {
		onomato.isFinish = true;
	}

}

void Onomatope::BossSpawnUpdate(OnomatoStruct& onomato) {




}

void Onomatope::GameOverUpdate(OnomatoStruct& onomato) {
}

void Onomatope::AttackChargeUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;

	if (onomato.frame <= 1.0f) {
		float alpha = 1.0f;
		alpha = Ease(In, Quart, onomato.frame, 1.f, 0.f);
		onomato.Tex->SetColor({ 1.f,1.f,1.f,alpha });
		XMFLOAT2 siz = onomato.Tex->GetSize();
		siz.x = Ease(Out, Back, onomato.frame, 256.f, 280.f);
		siz.y = Ease(Out, Back, onomato.frame, 256.f, 280.f);
		onomato.Tex->SetSize(siz);
		XMFLOAT2 pos = onomato.Tex->GetPosition();
		pos.x = Ease(Out, Quad, onomato.frame, onomato.basePos.x, onomato.basePos.x);
		pos.y = Ease(Out, Quad, onomato.frame, onomato.basePos.y, onomato.basePos.y - 180.0f);
		onomato.Tex->SetPosition(pos);
	} else {
		onomato.isFinish = true;
	}

}

void Onomatope::CounterUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;

	if (onomato.frame <= 1.0f) {
		float alpha = 1.0f;
		alpha = Ease(In, Quart, onomato.frame, 1.f, 0.f);
		onomato.Tex->SetColor({ 1.f,1.f,1.f,alpha });
		XMFLOAT2 siz = onomato.Tex->GetSize();
		siz.x = Ease(Out, Back, onomato.frame, 256.f, 280.f);
		siz.y = Ease(Out, Back, onomato.frame, 256.f, 280.f);
		onomato.Tex->SetSize(siz);
		XMFLOAT2 pos = onomato.Tex->GetPosition();
		pos.x = Ease(Out, Quad, onomato.frame, onomato.basePos.x, onomato.basePos.x);
		pos.y = Ease(Out, Quad, onomato.frame, onomato.basePos.y, onomato.basePos.y - 180.0f);
		onomato.Tex->SetPosition(pos);
	} else {
		onomato.isFinish = true;
	}
}
