#include "IKEObject3d.h"

#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include<sstream>
#include<string>
#include<vector>
#include "BaseCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

/// 静的メンバ変数の実体
ID3D12Device* IKEObject3d::device = nullptr;
ID3D12GraphicsCommandList* IKEObject3d::cmdList = nullptr;
IKEObject3d::PipelineSet IKEObject3d::pipelineSet;
XMMATRIX IKEObject3d::matView{};
XMMATRIX IKEObject3d::matProjection{};
XMFLOAT3 IKEObject3d::eye = { 0, 3.0f, -10.0f };
XMFLOAT3 IKEObject3d::target = { 0, 0, 0 };
XMFLOAT3 IKEObject3d::up = { 0, 1, 0 };
XMMATRIX IKEObject3d::matBillboard = XMMatrixIdentity();
XMMATRIX IKEObject3d::matBillboardY = XMMatrixIdentity();
Camera* IKEObject3d::camera = nullptr;
LightGroup* IKEObject3d::lightGroup = nullptr;


bool IKEObject3d::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, int window_width,
	int window_height, Camera* camera)
{
	// nullptrチェック
	assert(device);

	IKEObject3d::device = device;
	IKEObject3d::cmdList = cmdList;
	IKEObject3d::camera = camera;
	// グラフィックパイプラインの生成
	CreateGraphicsPipeline();

	// モデルの静的初期化
	IKEModel::StaticInitialize(device);

	// カメラ初期化
	//InitializeCamera(window_width, window_height);

	return true;
}


void IKEObject3d::PreDraw()
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	//assert(IKEObject3d::cmdList == nullptr);

	// コマンドリストをセット
	//IKEObject3d::cmdList = cmdList;

	//// パイプラインステートの設定
	//cmdList->SetPipelineState(pipelinestate.Get());
	//// ルートシグネチャの設定
	//cmdList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void IKEObject3d::PostDraw()
{
	// コマンドリストを解除
	//IKEObject3d::cmdList = nullptr;
}

IKEObject3d* IKEObject3d::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	auto object3d = new IKEObject3d();
	if (object3d == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!object3d->Initialize())
	{
		delete object3d;
		assert(0);
		return nullptr;
	}

	return object3d;
}

void IKEObject3d::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		//L"Resources/shaders/BasicVertexShader.hlsl",	// シェーダファイル名
		L"Resources/shaders/BasicVS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		//L"Resources/shaders/BasicPixelShader.hlsl",	// シェーダファイル名
		L"Resources/shaders/BasicPS.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n(static_cast<char*>(errorBlob->GetBufferPointer()),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			// xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			// uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2; // 描画対象は2つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R11G11B10_FLOAT; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R11G11B10_FLOAT; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	//CD3DX12_ROOT_PARAMETER rootparams[2];
	//rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	//rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	CD3DX12_ROOT_PARAMETER rootparams[4];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[3].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	auto samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
		&errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	if (FAILED(result))
	{
		assert(0);
	}

	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));

	if (FAILED(result))
	{
		assert(0);
	}
}


void IKEObject3d::UpdateViewMatrix()
{
	// ビュー行列の更新
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

IKEObject3d::~IKEObject3d()
{
	if (collider)
	{
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

bool IKEObject3d::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result;
	//// 定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuff));

	// 定数バッファの生成B0
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));
	//クラス名の文字列を取得
	name = typeid(*this).name();
	
	return true;
}
//頂点の数を調べる
void IKEObject3d::VertexCheck() {
	// モデルの割り当てがなければ通らない
	if (model != nullptr)
	{
		m_VertexNum = model->GetVertexNum();
	}
}
void IKEObject3d::Update()
{
	assert(camera);

	HRESULT result;
	//行列の更新
	UpdateWorldMatrix();

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();
	offset.y += addoffset;
	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->offset = offset;
	constMap->viewproj = matViewProjection;
	constMap->world = matWorld;
	constMap->cameraPos = cameraPos;
	constMap->color = color;
	constMap->uvval = uv;
	constBuffB0->Unmap(0, nullptr);
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}


void IKEObject3d::FollowUpdate(XMMATRIX matworld)
{
	assert(camera);

	HRESULT result;


	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	//行列の更新
	UpdateWorldMatrix();
	offset.y += addoffset;
	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->offset = offset;
	constMap->viewproj = matViewProjection;
	constMap->world = matWorld * matworld;
	constMap->cameraPos = cameraPos;
	constMap->color = color;
	constMap->uvval = uv;
	constBuffB0->Unmap(0, nullptr);
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}

//アフィン変換用
void IKEObject3d::AffineUpdate()
{
	assert(camera);

	HRESULT result;
	//行列の更新
	UpdateWorldMatrix();

	if (Affine)
	{
		matWorld *= matrix;
	}

	const XMMATRIX& matViewProjection = camera->GetViewProjectionMatrix();
	const XMFLOAT3& cameraPos = camera->GetEye();

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	constMap->offset = offset;
	constMap->viewproj = matViewProjection;
	constMap->world = matWorld;
	constMap->cameraPos = cameraPos;
	constMap->color = color;
	constMap->uvval = uv;
	constBuffB0->Unmap(0, nullptr);
	//当たり判定更新
	if (collider)
	{
		collider->Update();
	}
}


void IKEObject3d::Draw()
{
	// nullptrチェック
	assert(device);
	assert(IKEObject3d::cmdList);

	// モデルの割り当てがなければ描画しない
	if (model == nullptr)
	{
		return;
	}

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	// ライトの描画
	lightGroup->Draw(cmdList, 3);
	// モデル描画
	model->Draw(cmdList);
}

void IKEObject3d::UpdateWorldMatrix()
{
	assert(camera);

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	if (isBillboard && camera)
	{
		const XMMATRIX& matBillboard = camera->GetBillboardMatrix();

		matWorld = XMMatrixIdentity();
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matBillboard;
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	} else
	{
		matWorld = XMMatrixIdentity(); // 変形をリセット
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	}

	// 親オブジェクトがあれば
	if (parent != nullptr)
	{
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}
}


void IKEObject3d::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	//コリジョンマネージャーに登録
	CollisionManager::GetInstance()->AddCollider(collider);
	UpdateWorldMatrix();

	collider->Update();
}

void IKEObject3d::CollisionField()
{
	// ワールド行列更新
	this->UpdateWorldMatrix();

	// 落下処理
	if (!onGround)
	{
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.8f;
		// 加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		// 移動
		position.y += fallV.m128_f32[1];
	}

	// ワールド行列更新
	this->UpdateWorldMatrix();
	this->collider->Update();

	auto sphereCollider = dynamic_cast<SphereCollider*>(this->collider);
	//assert(sphereCollider);


	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere)
		{
		};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) override
		{
			const XMVECTOR up = { 0, 1, 0, 0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold)
			{
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ワールド行列更新
	this->UpdateWorldMatrix();
	this->collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius() + 3.f;
	ray.dir = { 0, -1, 0, 0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround)
	{
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 1.2f;
		// 接地を維持
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 2.5f + adsDistance))
		{
			onGround = true;
			position.y -= (raycastHit.m_Distance - sphereCollider->GetRadius() * 3.0f);
		}
		// 地面がないので落下
		else
		{
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態

	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit,
			sphereCollider->GetRadius() * 3.0f))
		{
			// 着地
			onGround = true;
			position.y -= (raycastHit.m_Distance - sphereCollider->GetRadius() * 3.0f);
		}
	}


	this->Update();
}
