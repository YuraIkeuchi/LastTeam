#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Camera.h"
#include"DirectXCommon.h"


//ブレンドタイプ
enum BlendType
{
	AlphaBlendType,
	AddBlendType,
	SubBlendType,
};

class IKETexture
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv; // uv座標
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color; // 色 (RGBA)
		XMMATRIX mat; // ３Ｄ変換行列
		bool clips;
		float Cinter;
		XMFLOAT2 offset;//オフセット値
		float Tiling;//タイリング
	};

private: // 定数
	static const int division = 50; // 分割数
	static const float radius; // 底面の半径
	static const float prizmHeight; // 柱の高さ
	static const int planeCount = division * 2 + division * 2; // 面の数
	static const int vertexCount = 4; // 頂点数
	static const int indexCount = 3 * 2;
public: // 静的メンバ関数
	/// 静的初期化
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width,
		int window_height, Camera* camera = nullptr);

	/// テクスチャ読み込み
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	/// 描画前処理
	static void PreDraw(int DrawNumber);
	static void PreDraw2(DirectXCommon* dxcomn, int DrawNumber);

	/// 描画後処理
	static void PostDraw();
	static void PostDraw2(DirectXCommon* dxcomn);
	//テクスチャ生成
	static IKETexture* Create(UINT texNumber, const XMFLOAT3& position, const XMFLOAT3& size, const XMFLOAT4& color);
	/// 視点座標の取得
	static const XMFLOAT3& GetEye() { return eye; }

	/// 視点座標の設定
	static void SetEye(const XMFLOAT3& eye);
	/// <summary>
/// カメラセット
/// </summary>
/// <returns></returns>
	static void SetCamera(Camera* camera)
	{
		IKETexture::camera = camera;
	}

	/// 注視点座標の取得
	static const XMFLOAT3& GetTarget() { return target; }

	/// 注視点座標の設定
	static void SetTarget(const XMFLOAT3& target);

	/// ベクトルによる移動
	static void CameraMoveVector(const XMFLOAT3& move);

	/// スプライト生成
	static IKETexture* Create(UINT texNumber, const XMFLOAT2& position, const XMFLOAT4& color = { 1, 1, 1, 1 },
		const XMFLOAT2& anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);


private: // 静的メンバ変数
	static const int srvCount = 213;
	// デバイス
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature[3];
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate[3];
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	// テクスチャバッファ
	static ComPtr<ID3D12Resource> texbuff[srvCount];
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// ビルボード行列
	static XMMATRIX matBillboard;
	// Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	// カメラ
	static Camera* camera;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	static VertexPosNormalUv vertices[vertexCount];
	// 頂点インデックス配列
	static unsigned short indices[indexCount];
	static ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	static ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	static ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト
private:
	UINT texNumber = 0;
	bool ClipF;
	float CenterPos;
private: // 静的メンバ関数
	/// デスクリプタヒープの初期化
	static bool InitializeDescriptorHeap();

	/// カメラ初期化
	static void InitializeCamera(int window_width, int window_height);

	//シェーダファイル読み込み
	static bool LoadShader();
	/// グラフィックパイプライン生成//それぞれ
	static bool AlphaInitializeGraphicsPipeline();
	static bool AddInitializeGraphicsPipeline();
	static bool SubInitializeGraphicsPipeline();
	/// モデル作成
	static void CreateModel();
	void TransferVertices();
	/// ビュー行列を更新
	static void UpdateViewMatrix();

public: // メンバ関数
	void TextureCreate();
	//コンストラクタ
	IKETexture(UINT texNumber, const XMFLOAT3& position, const XMFLOAT3& size, const XMFLOAT4& color);
	bool Initialize();
	/// 毎フレーム処理
	void Update();
	//アフィン変換用
	void AffineUpdate();
	//行列の更新
	void UpdateWorldMatrix();

	/// 描画
	void Draw();
	void Draw2(DirectXCommon* dxcomn);
	//開放
	static void Finalize();

	/// 座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	/// 座標の設定
	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	void SetPosition(float x, float y, float z)
	{
		this->position.x = x;
		this->position.y = y;
		this->position.z = z;
	}

	void SetIsBillboard(bool isBillboard);
	void SetColor(const XMFLOAT4& color) { this->color = color; }
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }
	const XMFLOAT3& GetScale() { return scale; }
	//タイリングの設定
	void SetTiling(float Tiling) { this->Tiling = Tiling; }
	//オフセットの設定
	void SetAddOffset(XMFLOAT2 addoffset) { this->addoffset = addoffset; }

	//アフィン変換行列を乗算する
	void AddMatrix(XMMATRIX matrix)
	{
		Affine = true;
		this->matrix = matrix;
	}

	//ワールド行列取得
	const XMMATRIX GetMatrix()
	{
		XMFLOAT3 l_scale = {};
		l_scale.x = 1 / scale.x;
		l_scale.y = 1 / scale.y;
		l_scale.z = 1 / scale.z;

		XMMATRIX l_mat = matWorld;
		l_mat.r[0].m128_f32[0] *= l_scale.x;
		l_mat.r[0].m128_f32[1] *= l_scale.x;
		l_mat.r[0].m128_f32[2] *= l_scale.x;
		l_mat.r[1].m128_f32[0] *= l_scale.y;
		l_mat.r[1].m128_f32[1] *= l_scale.y;
		l_mat.r[1].m128_f32[2] *= l_scale.y;
		l_mat.r[2].m128_f32[0] *= l_scale.z;
		l_mat.r[2].m128_f32[1] *= l_scale.z;
		l_mat.r[2].m128_f32[2] *= l_scale.z;
		return l_mat;
	}
	void SetCinter(float center) { CenterPos = center; }
	void SetClipF(bool f) { ClipF = f; }
private: // メンバ変数


	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	// ローカル座標
	XMFLOAT3 position = { 0, 0, 0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// ビルボード
	bool isBillboard = false;

	enum DrawType
	{
		ALPHA,
		ADD,
		SUB
	};


	//アフィン変換用
	bool Affine = false;

	//親子構造用
	XMMATRIX matrix = {};

	//オフセット値
	XMFLOAT2 offset = { 0.0f,0.0f };
	float Tiling = 1.0f;
	XMFLOAT2 addoffset = {};
};
