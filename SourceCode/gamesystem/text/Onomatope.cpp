#include "Onomatope.h"
#include "Easing.h"
#include "ImageManager.h"
#include "Helper.h"


void (Onomatope::* Onomatope::updateTable[])(OnomatoStruct& onomato) = {
	&Onomatope::FootUpdate,//
	&Onomatope::BossSpawnUpdate,//
	&Onomatope::GameOverUpdate,//
	&Onomatope::AttackChargeUpdate,//
	&Onomatope::CounterUpdate,
	&Onomatope::BurnUpdate,//
	&Onomatope::LittleBurnUpdate,
	&Onomatope::StoneUpdate,
	&Onomatope::RefrainUpdate,
	&Onomatope::GuardUpdate,
};

Onomatope::Onomatope() {
	Huu = IKESprite::Create(ImageManager::ONOMATO_08, { 640.f,800.f });
	Huu->SetAnchorPoint({ 0.5f,0.5f });
	Huu->SetSize({ 256.f,256.f });

}

Onomatope::~Onomatope() {
}

void Onomatope::Update() {

	for (OnomatoStruct& onomato : onomatoList) {
		(this->*updateTable[(size_t)onomato.pattern])(onomato);
	}
	onomatoList.remove_if([](OnomatoStruct& onomato) {
		return onomato.isFinish; });
}

void Onomatope::Draw() {
	IKESprite::PreDraw();
	for (OnomatoStruct& onomato : onomatoList) {
		onomato.Tex->Draw();
		if (onomato.pattern == Refrain) {
			Huu->Draw();
		}
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
	case BossSpawn:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_02, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 256.f,0.f });
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
		str.Tex->SetSize({ 486.f,86.f });
		str.kFrameMax = 80.f;
		str.kDelayFrameMax = delay;
		str.pattern = Counter;
		break;
	case Attack01:
		//str.Tex = IKESprite::Create(ImageManager::ONOMATO_01, basePos);
		//str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		//str.Tex->SetSize({ 0.f,256.f });
		//str.kFrameMax = 45.f;
		//str.kDelayFrameMax = delay;
		//str.pattern = Attack01;
		return;
		break;
	case Attack02:
		//str.Tex = IKESprite::Create(ImageManager::ONOMATO_06, basePos);
		//str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		//str.Tex->SetSize({ 256.f,256.f });
		//str.kFrameMax = 50.f;
		//str.kDelayFrameMax = delay;
		//str.pattern = Attack02;
		return;
		break;
	case Attack03:
		//str.Tex = IKESprite::Create(ImageManager::ONOMATO_07, basePos);
		//str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		//str.Tex->SetSize({ 256.f,256.f });
		//str.kFrameMax = 50.f;
		//str.kDelayFrameMax = delay;
		//str.pattern = Attack03;
		return;
		break;
	case Refrain:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_08, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 256.f,256.f });
		str.kFrameMax = 40.f;
		str.kDelayFrameMax = delay;
		str.pattern = Refrain;
		break;
	case Guard:
		str.Tex = IKESprite::Create(ImageManager::ONOMATO_09, basePos);
		str.Tex->SetAnchorPoint({ 0.5f,0.5f });
		str.Tex->SetSize({ 256.f,256.f });
		str.kFrameMax = 45.f;
		str.kDelayFrameMax = delay;
		str.pattern = Guard;
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
		pos.x = Ease(Out, Quad, onomato.frame, onomato.basePos.x, onomato.basePos.x + 320.f);
		pos.y = Ease(Out, Quad, onomato.frame, onomato.basePos.y, onomato.basePos.y - 180.0f);
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
	float frames = onomato.frame * 2.0f;
	Helper::Clamp(frames, 0.f, 1.f);

	float frameM = onomato.kFrameMax * 0.3f;
	static float frameZ = 0.f;
	if (onomato.frame <= 1.0f) {
		XMFLOAT2 pos = onomato.Tex->GetPosition();
		pos.y = Ease(Out, Quad, frames, 720 + 43.f, onomato.basePos.y);
		if (frames == 1.0f) {
			frameZ += 1 / frameM;
			float alpah = 0.f;
			alpah = Ease(Out, Exp, frameZ, 1.0f, 0.f);
			onomato.Tex->SetColor({ 1.f,1.f,1.f,alpah });
		}
		onomato.Tex->SetPosition(pos);
	} else {
		frameZ = 0.0f;
		onomato.isFinish = true;
	}
}

void Onomatope::LittleBurnUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;


	if (onomato.frame <= 1.0f) {
		XMFLOAT2 pos = onomato.basePos;
		pos.y = Ease(Out, SoftBack, onomato.frame, onomato.basePos.y, onomato.basePos.y + 180.0f);
		onomato.Tex->SetPosition(pos);
		onomato.Tex->SetColor({ 1.f,1.f,1.f,1.f });
	} else {
		onomato.vanishFrame += 1 / onomato.kVanishFrameMax;
		if (onomato.vanishFrame > 1.0f) {
			onomato.isFinish = true;
		} else {
			float alpha = Ease(In, Exp, onomato.vanishFrame, 1.0f, 0.f);
			onomato.Tex->SetColor({ 1.f,1.f,1.f,alpha });
		}
	}
}

void Onomatope::StoneUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;

	if (onomato.frame <= 1.0f) {
		XMFLOAT2 pos = onomato.basePos;
		pos.y = Ease(Out, Elastic, onomato.frame, onomato.basePos.y, onomato.basePos.y + 180.0f);
		onomato.Tex->SetPosition(pos);
		onomato.Tex->SetColor({ 1.f,1.f,1.f,1.f });
	} else {
		onomato.vanishFrame += 1 / onomato.kVanishFrameMax;
		if (onomato.vanishFrame > 1.0f) {
			onomato.isFinish = true;
		} else {
			float alpha = Ease(In, Exp, onomato.vanishFrame, 1.0f, 0.f);
			onomato.Tex->SetColor({ 1.f,1.f,1.f,alpha });
		}
	}
}

void Onomatope::RefrainUpdate(OnomatoStruct& onomato) {
	onomato.delayFrame += 1 / onomato.kDelayFrameMax;
	if (onomato.delayFrame < 1.0f) {
		return;
	}
	onomato.frame += 1 / onomato.kFrameMax;

	if (onomato.frame <= 1.0f) {
		XMFLOAT2 pos = onomato.basePos;
		pos.x = Ease(Out, Back, onomato.frame, onomato.basePos.x - 100.f, onomato.basePos.x - 300.f);
		pos.y = Ease(Out, Back, onomato.frame, onomato.basePos.y, onomato.basePos.y - 150.0f);
		onomato.Tex->SetPosition(pos);
		float posS= Ease(Out, Back, onomato.frame, onomato.basePos.x + 100.f, onomato.basePos.x + 300.f);
		Huu->SetPosition({ posS,pos.y });
		onomato.Tex->SetColor({ 1.f,0.5f,0.f,1.f });
		Huu->SetColor({ 1.f,1.f,0.f,1.f });
		onomato.Tex->SetRotation(-3.f);
		Huu->SetRotation(3.f);
	} else {
		onomato.vanishFrame += 1 / onomato.kVanishFrameMax;
		if (onomato.vanishFrame > 1.0f) {
			onomato.isFinish = true;
		} else {
			float alpha = Ease(In, Exp, onomato.vanishFrame, 1.0f, 0.f);
			onomato.Tex->SetColor({ 1.f,0.5f,0.f,alpha });
			Huu->SetColor({ 1.f,1.f,0.f,alpha });
		}
	}

}
void Onomatope::GuardUpdate(OnomatoStruct& onomato) {
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
	}
	else {
		onomato.isFinish = true;
	}

}