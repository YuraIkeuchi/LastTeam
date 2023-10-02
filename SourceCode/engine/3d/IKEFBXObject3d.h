#pragma once

#include "IKEFBXModel.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class IKEFBXObject3d
{
protected: //エイリアス
	//Microosoft::WRL::を省略
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //静的メンバ関数
	//setter
	static void SetDevice(ID3D12Device* device) { IKEFBXObject3d::device = device; }
	static void SetCamera(Camera* camera) { IKEFBXObject3d::camera = camera; }

	/// <summary>
	///グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

public: //サブクラス
	//定数バッファ用データ構造体（座標変換行列）
	struct ConstBufferDataTransform
	{
		bool ShadeSet;
		XMMATRIX viewproj; //ビュープロジェクション行列
		XMMATRIX world; //ワールド行列
		XMFLOAT3 cameraPos; //カメラ行列（ワールド座標）
		float pad; //パディング
		XMFLOAT4 color; //色情報
	};

	// アニメーション用データ構造体
	struct Animation
	{
		FbxAnimStack* stack;
		FbxTakeInfo* info;
	};

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム
	/// </summary>
	void Update(bool Loop, int Speed, bool& Stop);
	//追従させるようの更新
	void FollowUpdate(bool Loop, int Speed, bool& Stop);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(IKEFBXModel* model) { this->model = model; }

	//アニメーションのロード
	void LoadAnimation();
	/// <summary>
	/// アニメーション
	/// </summary>
	void PlayAnimation(int Number);

	void StopAnimation();

	XMMATRIX ExtractPositionMat(XMMATRIX matworld);
	XMMATRIX ExtractRotationMat(XMMATRIX matworld);
	XMMATRIX ExtractScaleMat(XMMATRIX matworld);

	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	void SetColor(const XMFLOAT4& color) { this->color = color; }
	//ディゾルブの設定
	void SetDisolve(float Disolve) { this->Disolve = Disolve; }
	void SetAddColor(const XMFLOAT4& Addcolor) { this->Addcolor = Addcolor; }

	void SetChangeColor(bool ChangeColor) { this->ChangeColor = ChangeColor; }
	void SetShadeSet(bool ShadeSet) { this->ShadeSet = ShadeSet; }
	void SetReverse(bool Reverse) { this->m_Reverse = Reverse; }
	XMMATRIX GetWorldMat() { return WorldMat; }
	XMMATRIX GetMatrot() { return matRot; }
	XMMATRIX GetMatScl() { return matScale; }
	const int GetBoneNumber() { return BoneNumber; }
	const bool GetReverse() { return m_Reverse; }
	void GetBoneIndexMat(int index, XMMATRIX& matworld);

	void SetBoneNumber(int BoneNumber) { this->BoneNumber = BoneNumber; }
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }

	//FBXタイムの終了値
	int GetFbxTime_End() { return endTime.GetSecondCount(); }
	//
	int GetFbxTime_Current() { return currentTime.GetSecondCount(); }
protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffTransform;
	//ディゾルブ
	float Disolve = -1.0f;
	XMFLOAT4 Addcolor = {1, 1, 1, 1};
	//ローカルスケール
	XMFLOAT3 scale = {1, 1, 1};
	//X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = {0, 0, 0};
	//ローカル座標
	XMFLOAT3 position = {0, 0, 0};
	//色
	XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	IKEFBXModel* model = nullptr;
	size_t BoneSize;
	bool ChangeColor = false; //色変更

	bool ShadeSet = true;
	//定数バッファ（スキン）
	ComPtr<ID3D12Resource> constBuffSkin;
	//1フレーム
	FbxTime frameTime;
	//アニメーション開始時間
	FbxTime startTime;
	//アニメーション終了時間
	FbxTime endTime;
	//現在時間（アニメーション）
	FbxTime currentTime;
	//アニメーション再生中
	bool isPlay = false;
	//アニメーションのデータ
	std::vector<Animation> animationData;
	//逆再生かどうか
	bool m_Reverse = false;
public: //定数
	//主に追従関係
	XMMATRIX matScale, matRot, matTrans;
	XMMATRIX WorldMat;
	FbxTime SetCurrent() { return currentTime = startTime; }
	bool GetIsPlay() { return isPlay; }
	FbxTime GetCurrent() { return currentTime; }
	size_t GetBoneSize() { return BoneSize; }
	FbxTime GetEndTime() { return endTime; }
	//ボーンの最大数
	static const int MAX_BONES = 90;
	int BoneNumber = 0;
	int m_FBXTimer = 0;

	//定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};
};
