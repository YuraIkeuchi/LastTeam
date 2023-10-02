#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "Camera.h"

/// <summary>
/// パーティクルマネージャ
/// </summary>
class ParticleManager {
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
	struct VertexPos {
		XMFLOAT3 pos; // xyz座標
		float scale; // スケール
		XMFLOAT4 color; // 色(RGBA)
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData {
		XMMATRIX mat;	// ビュープロジェクション行列
		XMMATRIX matBillboard;	// ビルボード行列
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
	class Particle {
		// Microsoft::WRL::を省略
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		// DirectX::を省略
		using XMFLOAT2 = DirectX::XMFLOAT2;
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMFLOAT4 = DirectX::XMFLOAT4;
		using XMMATRIX = DirectX::XMMATRIX;

	public:
		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
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
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	static void CreateCommon(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const std::string& directoryPath = "Resources/2d/effect/");


	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(int type);

	///// <summary>
	///// カメラのセット
	///// </summary>
	///// <param name="camera">カメラ</param>
	//inline void SetCamera(Camera* camera) { this->camera = camera; }

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
		const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel,
		const float& start_scale, const float& end_scale,
		const XMFLOAT4& start_color, const XMFLOAT4& end_color,const float Gravity,float divi=1.0f);

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ParticleManager</returns>
	static ParticleManager* Create(UINT texNumber);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static void InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();
	/// <summary>
	/// 加算合成パイプライン生成
	/// </summary>
	static void CreateAddBlendPipeline();

	/// <summary>
	/// 減算合成パイプライン生成
	/// </summary>
	static void CreateSubBlendPipeline();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	static void LoadTexture(UINT texNumber, const std::string& filename);

	/// <summary>
	/// モデル作成
	/// </summary>
	bool CreateModel(UINT texNumber);

	//削除
	void AllDelete();

	//setter
	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

private: // メンバ変数
	static const int srvCount = 213;
	// デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	//加算合成パイプラインセット
	static PipelineSet addBlendPipelineSet;
	//減算合成パイプラインセット
	static PipelineSet subBlendPipelineSet;
	//半透明合成パイプラインセット
	static PipelineSet alphaBlendPipelineSet;
	// カメラ
	static Camera* camera;
	//画像読み込み
	static std::string directoryPath;
	//拡張子
	static std::string extensionPath;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	//ComPtr<ID3D12Resource> texbuff[texnumber];
	static ComPtr<ID3D12Resource> texbuff[srvCount];

	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// パーティクル配列
	std::forward_list<Particle> particles;

};

