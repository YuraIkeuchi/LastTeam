#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "Camera.h"
#include "IKESprite.h"
#include <stdlib.h>

/// <summary>
/// パーティクルマネージャ
/// </summary>
class ParticleManager2D : public IKESprite{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosUv {
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv; // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; // ３Ｄ変換行列
		XMFLOAT2 offset;//オフセット値
	};

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};
	// パーティクル1粒
	class Particle2D {
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//座標
		XMFLOAT2 position = {};
		//速度
		XMFLOAT2 velocity = {};
		//加速度
		XMFLOAT2 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		// スケール
		float scale = 1.0f;
		// 初期値
		float s_scale = 1.0f;
		// 最終値
		float e_scale = 0.0f;
		// 色(RGBA)
		XMFLOAT4 color = { 1, 1, 1, 1 };
		// 色(RGBA)初期値
		XMFLOAT4 s_color = {};
		// 色(RGBA)最終値
		XMFLOAT4 e_color = {};
		//重力
		float m_GraVity = {};

		float DiviSpeed = 1.0f;
	};

private: // 定数
	static const int vertexCount = 65536;		// 頂点数
	UINT texNumber = 0;

public: // メンバ関数	
	//コンストラクタ

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="life">生存時間</param>
	/// <param name="position">初期座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_scale">開始時スケール</param>
	/// <param name="end_scale">終了時スケール</param>
	void Add(const int& life,
		const XMFLOAT2& position, const XMFLOAT2& velocity, const XMFLOAT2& accel,
		const float& start_scale, const float& end_scale,
		const XMFLOAT4& start_color, const XMFLOAT4& end_color,const float Gravity,float divi=1.0f);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ParticleManager2D</returns>
	static ParticleManager2D* Create(UINT texNumber, const XMFLOAT2& position, const XMFLOAT4& color = { 1, 1, 1, 1 },
		const XMFLOAT2& anchorpoint = { 0.5f, 0.5f }, bool isFlipX = false, bool isFlipY = false);

	void TransferVertices()override;

	//削除
	void AllDelete();

private: // メンバ変数
	const std::string& directoryPath = "Resources/2d/effect/";


	static const int srvCount = 213;
	// デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;

	// 射影行列
	static XMMATRIX matProjection;
	// カメラ
	static Camera* camera;
	//拡張子
	static std::string extensionPath;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	//ComPtr<ID3D12Resource> texbuff[texnumber];
	static ComPtr<ID3D12Resource> texbuff[srvCount];

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// パーティクル配列
	std::forward_list<Particle2D> particles;
	
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{};
	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	//オフセット
	float addoffset = 0.0f;
	//オフセット値
	XMFLOAT2 offset = { 0.0f,0.0f };
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texBase = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 100.0f, 100.0f };

	float RadPowers;
	XMFLOAT2 RadCenterPos;
};

