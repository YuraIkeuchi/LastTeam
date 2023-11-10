#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string>
#include "IKEModel.h"
#include "Camera.h"
#include "LightGroup.h"

#include "CollisionInfo.h"

class BaseCollider;

/// 3Dオブジェクト
class IKEObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス	

	// パイプラインセット
	struct PipelineSet
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelinestate;
	};

	// 定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMFLOAT2 offset;//オフセット値
		XMMATRIX viewproj;    // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		float pad;//パディング
		XMFLOAT4 color;//色情報
		float uvval;
		bool LightEffect;//ライトの影響受けるか
	};

private: // 定数


public: //静的メンバ関数

	//静的初期化
	static bool StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height, Camera* camera = nullptr);


	//グラフィックパイプラインの生成
	static void CreateGraphicsPipeline();


	//描画前処理
	static void PreDraw();
	//描画後処理
	static void PostDraw();
	//3Dオブジェクト生成
	static IKEObject3d* Create();

	static void SetCamera(Camera* camera) {
		IKEObject3d::camera = camera;
	}

	static void SetLightGroup(LightGroup* lightGroup) {
		IKEObject3d::lightGroup = lightGroup;
	}
	//ベクトルによる移動
	static void CameraMoveVector(const XMFLOAT3& move);


	// ベクトルによる視点移動
	static void CameraMoveEyeVector(const XMFLOAT3& move);

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// テクスチャあり用パイプライン
	static PipelineSet pipelineSet;
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
	// ライト
	static LightGroup* lightGroup;


private:// 静的メンバ関数	
	//カメラ初期化
	static void InitializeCamera(int window_width, int window_height);


	//ビュー行列を更新
	static void UpdateViewMatrix();

public: // メンバ関数

	IKEObject3d() = default;

	virtual ~IKEObject3d();

	virtual	bool Initialize();

	void VertexCheck();

	//毎フレーム処理
	virtual void Update();

	virtual void AffineUpdate();

	//追従させるものの更新
	void FollowUpdate(XMMATRIX matworld);
	// 描画
	virtual void Draw();
	//行列の更新
	void UpdateWorldMatrix();


	//座標の取得
	const XMFLOAT3& GetPosition() { return position; }

	//回転の取得
	const XMFLOAT3& GetRotation() { return rotation; }

	//拡大縮小の取得
	const XMFLOAT3& GetScale() { return scale; }
	const XMMATRIX& GetMatWorld() { return matWorld; }
	XMMATRIX GetMatrot() { return matRot; }
	XMMATRIX GetMatScl() { return matScale; }

	/// <summary>
/// モデルを取得
/// </summary>
	inline IKEModel* GetModel() { return model; }

	//座標の設定
	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	void SetUvScrollY(float uvs) { uv = uvs; }

	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	//スケールの設定
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }
	//色の設定
	void SetColor(const XMFLOAT4& color) { this->color = color; }
	//色を変化させるかどうか
	void SetChangeColor(bool ChangeColor) { this->ChangeColor = ChangeColor; }
	void SetFog(bool Fog) { this->Fog = Fog; }
	void SetLightEffect(bool LightEffect) { this->LightEffect = LightEffect; }
	void SetAddColor(const XMFLOAT4& Addcolor) { this->Addcolor = Addcolor; }
	//オフセットの設定
	void SetAddOffset(float addoffset) { this->addoffset = addoffset; }
	//タイリングの設定
	void SetTiling(float Tiling) { this->Tiling = Tiling; }
	//ディゾルブの設定
	void SetDisolve(float Disolve) { this->Disolve = Disolve; }
	//ディゾルブの設定
	void SetLightPower(float lightPower) { this->lightPower = lightPower; }
	//モデルのセット
	void SetModel(IKEModel* model) { this->model = model; }

	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	const size_t& GetVertexNum() { return m_VertexNum; }
	//当たり判定セット
	void SetCollider(BaseCollider* collider);

	void CollisionField();

	//コールバック
	virtual void OnCollision(const CollisionInfo& info) {}


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

	//ワールド行列取得
	const XMMATRIX GetMatrix2(XMMATRIX matWorld2)
	{
		XMFLOAT3 l_scale = {};
		l_scale.x = 1 / scale.x;
		l_scale.y = 1 / scale.y;
		l_scale.z = 1 / scale.z;

		XMMATRIX l_mat = matWorld * matWorld2;
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

protected: // メンバ変数
	ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	//オフセット値
	XMFLOAT2 offset = { 0.0f,0.0f };
	float addoffset = 0.0f;
	float Tiling = 1.0f;
	float Disolve = -1.5f;
	bool ChangeColor = false;//色変更
	bool Fog = false;//フォグ
	bool LightEffect = true;//ライトの影響を受けるか
	float lightPower = 15.0f;
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	//加算する色
	XMFLOAT4 Addcolor = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX matWorld;
	// 親オブジェクト
	IKEObject3d* parent = nullptr;
	// モデル
	IKEModel* model = nullptr;
	// ビルボード
	bool isBillboard = false;
	//クラス名
	const char* name = nullptr;

	//アフィン変換用
	bool Affine = false;
	//頂点の数
	size_t m_VertexNum = 0;
	//親子構造用
	DirectX::XMMATRIX matrix = {};
private:
	XMMATRIX matScale, matRot, matTrans;
	float uv;
public:
	//コライダー
	BaseCollider* collider = nullptr;
private:
	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	DirectX::XMVECTOR fallV = {};
	float radius_adjustment = 0.0f;
	//FBX
	int FallGroundTime = 0;

private:
	std::string modelname;
public:
	void SetModeName(std::string name) { modelname = name; }
	std::string GetModelName() { return modelname; }
};

