#pragma once
#include "BaseScene.h"
#include "TextManager.h"
#include<memory>
#include"InterEnemy.h"
#include "IKESprite.h"
#include <array>
/// タイトルシーン
class TitleScene : public BaseScene {

public:
	/// 初期化
	void Initialize(DirectXCommon* dxCommon) override;
	/// 終了
	void Finalize() override;
	/// 毎フレーム更新
	void Update(DirectXCommon* dxCommon) override;
	/// 描画
	void Draw(DirectXCommon* dxCommon) override;

private:
	void BackDraw(DirectXCommon* dxCommon);
	void ImGuiDraw(DirectXCommon* dxCommon);
	void FrontDraw(DirectXCommon* dxCommon);

private:
	unique_ptr<TextManager> text_;

	enum SceneType {
		PLAY,
		MAP,
		TUTORIAL
	}_SceneType = PLAY;

	unique_ptr<InterEnemy> enemy = nullptr;

	unique_ptr<IKESprite> title_;
	unique_ptr<IKESprite> player;
	float rota = 0.f;
	float rota2 = 0.f;

	float rot = 0.f * XM_PI / 180.f;
	vector<unique_ptr<IKESprite>> passives;
	vector<unique_ptr<IKESprite>> skills;

	unique_ptr<IKESprite> magic_;
	XMFLOAT2 magicbaseSize = { 256.f * 0.7f,256.f * 0.7f };
	unique_ptr<IKESprite> onomatope_;
	unique_ptr<IKESprite> onomatope2_;

	static void(TitleScene::* stateTable[])();

	int stateCount = 0;
	int kWaitCountMax = 120;
	int kOpenMagicCountMax = 60;
	int kVerseOnomatoCountMax = 45;
	int kCloseMagicCountMax = 30;

	enum class STATE :int{
		wait = 0,
		openMagic,
		verseOnomato,
		verse2Onomato,
		closeMagic

	}state = STATE::wait;

	void Wait();
	void OpenMagic();
	void VerseOnomato();
	void Verse2Onomato();
	void CloseMagic();

};