#pragma once
#include"IKEObject3d.h"
#include"IKEModel.h"
#include"IKEFBXObject3d.h"
#include"IKEFBXModel.h"

#include"DirectXCommon.h"
#include"Audio.h"
#include"ModelManager.h"
#include"VolumManager.h"
#include<memory>
using namespace std;         //  名前空間指定

class ObjCommon :
	public IKEObject3d{
public:
	virtual ~ObjCommon();
protected:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
public:
	//当たり判定
	virtual void SetCollider();
	//初期化
	virtual bool Initialize() = 0;
	//ステータスセット
	virtual void Obj_SetParam();
	virtual void FollowObj_SetParam(XMMATRIX matworld);
	virtual void Fbx_SetParam();
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Obj_Draw();
	virtual void FollowObj_Draw();
	virtual void Fbx_Draw(DirectXCommon* dxCommon);

	virtual void Draw(DirectXCommon* dxCommon) = 0;

public:
	//gettersetter
	const XMFLOAT3& GetPosition() { return m_Position; }
	const XMFLOAT3& GetRotation() { return m_Rotation; }
	const XMFLOAT3& GetScale() { return m_Scale; }
	const XMFLOAT4& GetColor() { return m_Color; }
	const XMMATRIX& GetMatRot() { return m_MatRot; }


	void SetPosition(const XMFLOAT3& position) { m_Position = position; }
	void SetRotation(const XMFLOAT3& rotation) { m_Rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { m_Scale = scale; }
	void SetColor(const XMFLOAT4& color) { m_Color = color; }
protected:
	//共通変数(座標とか)
	unique_ptr<IKEObject3d> m_Object;
	IKEModel* m_Model;

	//共通変数(座標とか)
	unique_ptr<IKEObject3d> m_FollowObject;
	IKEModel* m_FollowModel;

	unique_ptr<IKEFBXObject3d> m_fbxObject;
	IKEFBXModel* m_fbxModel;

	XMMATRIX m_MatRot;
	XMFLOAT3 m_Position = {0.0f,0.0f,0.0f};
	XMFLOAT3 m_Rotation = { 0.0f,0.0f,0.0f };
	XMFLOAT3 m_Scale = { 1.0f,1.0f,1.0f };
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	XMFLOAT4 m_Addcolor = { 0.0f,0.0f,0.0f,1.0f };
	//アニメーション関係
	//アニメーション管理用
	bool m_LoopFlag = true;
	int m_AnimationSpeed = 1;
	bool m_StopFlag = false;

	int m_AnimationType = 0;
	bool m_ChangeColor = false;
	float m_AddDisolve = 0.0f;
};