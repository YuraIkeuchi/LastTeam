#pragma once
#include "IKESprite.h"

class PostEffect : public IKESprite
{
private:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA_POST
	{
		XMFLOAT3 Color; //色
		float sepia; //セピア
		bool isTone; //トーンの有無
		float frame; //水面表現
		XMFLOAT2 P1;
		XMFLOAT2 P2;
		XMFLOAT2 P3;
		float RadPower;
		XMFLOAT2 CenterPos;
		float closeRad;
	};

public:
	//コンストラクタ
	PostEffect();
	/// <summary>
  /// パイプライン生成
  /// </summary>
  /// <param name="cmdList"></param>
	void CreateGraphicsPipeline(const wchar_t* vsShaderName, const wchar_t* psShaderName);
	//初期化
	void Initialize();

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//ImGui
	void ImGuiDraw();

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
private: //静的メンバ変数
	static const float clearColor[4];
public:
	void SetSepia(float addsepia) { this->addsepia = addsepia; }
private: //メンバ変数
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff[2];
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
	//セピア
	static float addsepia;
	int tex = 0;

	XMFLOAT2 P1 = {0.10f, 0.05f};
	XMFLOAT2 P2 = {0.50f, 0.5f};
	XMFLOAT2 P3 = {2.00f, 1.00f};
	
};
