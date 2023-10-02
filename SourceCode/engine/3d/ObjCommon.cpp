#include "ObjCommon.h"
#include"SphereCollider.h"
#include"CollisionAttribute.h"
#include"TouchableObject.h"
#include"CollisionManager.h"
#include"Collision.h"

ObjCommon::~ObjCommon()
{
}

bool ObjCommon::Initialize()
{
	return true;
}

void ObjCommon::SetCollider()
{
}

//オブジェクトのセット
void ObjCommon::Obj_SetParam()
{
	m_Object->SetPosition(m_Position);
	m_Object->SetRotation(m_Rotation);
	m_Object->SetScale(m_Scale);
	m_Object->SetColor(m_Color);
	m_Object->Update();
	UpdateWorldMatrix();
}

//オブジェクトのセット
void ObjCommon::FollowObj_SetParam(XMMATRIX matworld)
{
	m_FollowObject->SetAddColor(m_Addcolor);
	m_FollowObject->SetDisolve(m_AddDisolve);
	m_FollowObject->SetChangeColor(m_ChangeColor);
	m_FollowObject->FollowUpdate(matworld);
	UpdateWorldMatrix();
}

//FBXのセット
void ObjCommon::Fbx_SetParam()
{
	m_fbxObject->SetPosition(m_Position);
	m_fbxObject->SetRotation(m_Rotation);
	m_fbxObject->SetScale(m_Scale);
	m_fbxObject->SetColor(m_Color);
}

void ObjCommon::Obj_Draw()
{
	PreDraw();
	m_Object->Draw();
	PostDraw();
}

void ObjCommon::FollowObj_Draw()
{
	PreDraw();
	m_FollowObject->Draw();
	PostDraw();
}

void ObjCommon::Fbx_Draw(DirectXCommon* dxCommon)
{
	m_fbxObject->Draw(dxCommon->GetCmdList());
}
