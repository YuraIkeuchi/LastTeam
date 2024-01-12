#pragma once
#include "IKESprite.h"
#include <DirectXMath.h>
#include <memory>
#include <DrawNumber.h>
static const int RESULT_HEIGHT = 4;
static const int RESULT_WIDTH = 8;
class ResultAreaUI {
public:
	ResultAreaUI();
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
private:

	//パネル
	struct Panel {
		std::unique_ptr<IKESprite> sprite = nullptr;
		XMFLOAT2 position = { 0,0 };
		XMFLOAT4 color = { 1,1,1,1 };
		bool resultarea = false;
	};

public:
	//初期化
	bool Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//ImGui
	void ImGuiDraw();

public:
	void ResetTimer();

	//gettersetter
	void SetDelay(int delay) {
		m_Delay = delay;
	}

	void SetPanelNumber(const int Width, const int Height) {
		m_Width = Width;
		m_Height = Height;
	}
	void SetDistance(const int DistanceX, const int DistanceY){
		m_DistanceX = DistanceX;
		m_DistanceY = DistanceY;
	}
	bool SetPredict(bool Flag) {
		return panels.resultarea = Flag;
	}
private:
	//パネル
	Panel panels;
	int m_Width = {};
	int m_Height = {};
	int m_DistanceX = {};
	int m_DistanceY = {};
	XMFLOAT2 prePos = {};
	//
	float finishDelay = 0.f;
	//待機
	bool isJump = false;
	std::array<std::unique_ptr<DrawNumber>, 3> _DelayTimer;
	std::array<int, 3> _Delays;
	int m_Delay = 0;
	int m_Frame = 0;

	std::unique_ptr<IKESprite> player_panel;
};