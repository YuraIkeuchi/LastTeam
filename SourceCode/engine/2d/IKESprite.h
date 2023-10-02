#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "DirectXCommon.h"

/// スプライト

class IKESprite
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	/// 頂点データ構造体
	struct VertexPosUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv; // uv座標
	};


	/// 定数バッファ用データ構造体

	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; // ３Ｄ変換行列
		XMFLOAT2 offset;//オフセット値
	};

public: // 静的メンバ関数
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width,
	                             int window_height);

	/// テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	/// 描画前処理
	static void PreDraw();

	/// 描画後処理
	static void PostDraw();

	/// スプライト生成
	static std::unique_ptr<IKESprite> Create(UINT texNumber, const XMFLOAT2& position, const XMFLOAT4& color = {1, 1, 1, 1},
	                         const XMFLOAT2& anchorpoint = {0.0f, 0.0f}, bool isFlipX = false, bool isFlipY = false);

protected: // 静的メンバ変数
	// テクスチャの最大枚数
	static const int srvCount = 512;
	// 頂点数
	static const int vertNum = 4;
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	// 射影行列
	static XMMATRIX matProjection;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texBuff[srvCount];

	float closeRad;

public: // メンバ関数

	/// コンストラクタ
	IKESprite(UINT texNumber, const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT4& color,
	          const XMFLOAT2& anchorpoint, bool isFlipX, bool isFlipY);


	/// 初期化
	bool Initialize();


	/// 角度の設定
	void SetRotation(float rotation);


	/// 座標の設定
	void SetPosition(const XMFLOAT2& position);

	/// 座標の取得
	const XMFLOAT2& GetPosition() { return position; }

	/// サイズの設定
	void SetSize(const XMFLOAT2& size);
	/// 
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(float scale);

	/// <summary>
	/// サイズの設定
	/// </summary>
	/// <param name="size">サイズ</param>
	const XMFLOAT2& GetSize() { return size; }


	/// カラーの設定
	void IKESprite::SetColor(const XMFLOAT4& color);


	/// アンカーポイントの設定
	void SetAnchorPoint(const XMFLOAT2& anchorpoint);


	/// 左右反転の設定
	void SetIsFlipX(bool isFlipX);


	/// 上下反転の設定
	void SetIsFlipY(bool isFlipY);


	/// テクスチャ範囲設定
	void SetTextureRect(const XMFLOAT2& texBase, const XMFLOAT2& texSize);


	/// 描画

	void Draw();

	//開放
	static bool Finalize();
	bool CreateVertices();


	void SetRadCenter(XMFLOAT2 pos) { RadCenterPos = pos; }
	void SetRadPower(float p) { RadPowers = p; }
	//オフセットの設定
	void SetAddOffset(float addoffset) { this->addoffset = addoffset; }
	void SetCloseRad(float r) { closeRad = r; }

protected: // メンバ変数
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// テクスチャ番号
	UINT texNumber = 0;
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{};
	// スプライト幅、高さ
	XMFLOAT2 size = {100.0f, 100.0f};
	// アンカーポイント
	XMFLOAT2 anchorpoint = {0, 0};
	// ワールド行列
	XMMATRIX matWorld{};
	//オフセット
	float addoffset = 0.0f;
	//オフセット値
	XMFLOAT2 offset = { 0.0f,0.0f };
	// 色
	XMFLOAT4 color = {1, 1, 1, 1};
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texBase = {0, 0};
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = {100.0f, 100.0f};

	float RadPowers;
	XMFLOAT2 RadCenterPos;
private: // メンバ関数
	/// 頂点データ転送
	void TransferVertices();
};
