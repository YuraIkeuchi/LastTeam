#include "ParticleManager2D.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "VariableCommon.h"
#include "IKETexture.h"
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;


ID3D12Device* ParticleManager2D::device = nullptr;
ID3D12GraphicsCommandList* ParticleManager2D::cmdList = nullptr;
Camera* ParticleManager2D::camera = nullptr;
ParticleManager2D::PipelineSet ParticleManager2D::addBlendPipelineSet;
ParticleManager2D::PipelineSet ParticleManager2D::subBlendPipelineSet;
ParticleManager2D::PipelineSet ParticleManager2D::alphaBlendPipelineSet;
ComPtr<ID3D12DescriptorHeap> ParticleManager2D::descHeap;
CD3DX12_GPU_DESCRIPTOR_HANDLE ParticleManager2D::gpuDescHandleSRV;
CD3DX12_CPU_DESCRIPTOR_HANDLE ParticleManager2D::cpuDescHandleSRV;
ComPtr<ID3D12Resource> ParticleManager2D::texbuff[srvCount];
UINT ParticleManager2D::descriptorHandleIncrementSize = 0u;
std::string ParticleManager2D::directoryPath;
std::string ParticleManager2D::extensionPath = ".png";

XMMATRIX ParticleManager2D::matProjection;

static const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs) {
	XMFLOAT2 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	return result;
}

static const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs) {
	XMFLOAT2 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const float rhs) {
	XMFLOAT2 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	return result;
}

bool ParticleManager2D::Initialize() {
	IKESprite::Initialize();
}

void ParticleManager2D::Update() {
	HRESULT result;

	// 寿命が尽きたパーティクルを全削除
	particles.remove_if([](Particle2D& x) { return x.frame >= x.num_frame; });

	// 全パーティクル更新
	for (std::forward_list<Particle2D>::iterator it = particles.begin();
		it != particles.end();
		it++) {

		// 経過フレーム数をカウント
		it->frame++;
		// 進行度を0～1の範囲に換算
		float f = (float)it->num_frame / it->frame;

		// 速度に加速度を加算
		it->velocity = it->velocity + it->accel;

		//Y方向の速度に重力追加
		//it->velocity.y = it->velocity.y - it->m_GraVity;
		// 速度による移動
		it->position = it->position + it->velocity/it->DiviSpeed;

		// カラーの線形補間
		it->color.x = it->s_color.x + (it->e_color.x - it->s_color.x) / f;
		it->color.y = it->s_color.y + (it->e_color.y - it->s_color.y) / f;
		it->color.z = it->s_color.z + (it->e_color.z - it->s_color.z) / f;
		it->color.w = it->s_color.w + (it->e_color.w - it->s_color.w) / f;

		// スケールの線形補間
		it->scale = it->s_scale + (it->e_scale - it->s_scale) / f;
	}

	// 頂点バッファへデータ転送
	int vertCount = 0;
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		// パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle2D>::iterator it = particles.begin();
			it != particles.end();
			it++) {
			// 座標
			vertMap->pos = it->position;
			// スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			// 次の頂点へ
			vertMap++;
			if (++vertCount >= vertexCount) {
				break;
			}
		}
		vertBuff->Unmap(0, nullptr);
	}

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = camera->GetViewProjectionMatrix();
	constBuff->Unmap(0, nullptr);
}

void ParticleManager2D::Draw(int type) {
	UINT drawNum = (UINT)std::distance(particles.begin(), particles.end());
	if (drawNum > vertexCount) {
		drawNum = vertexCount;
	}

	// パーティクルが1つもない場合
	if (drawNum == 0) {
		return;
	}

	// nullptrチェック
	assert(cmdList);
	if (type == AlphaBlendType) {
		// パイプラインステートの設定
		cmdList->SetPipelineState(alphaBlendPipelineSet.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(alphaBlendPipelineSet.rootsignature.Get());
	}
	if (type == AddBlendType) {
		// パイプラインステートの設定
		cmdList->SetPipelineState(addBlendPipelineSet.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(addBlendPipelineSet.rootsignature.Get());
	}
	if (type == SubBlendType) {
		// パイプラインステートの設定
		cmdList->SetPipelineState(subBlendPipelineSet.pipelinestate.Get());
		// ルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(subBlendPipelineSet.rootsignature.Get());
	}


	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);
	offset.x += addoffset;
	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection; // 行列の合成	
		constMap->offset = offset;
		this->constBuff->Unmap(0, nullptr);
	}

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeap->GetGPUDescriptorHandleForHeapStart(), this->texNumber,
		descriptorHandleIncrementSize));
	// 描画コマンド
	cmdList->DrawInstanced(drawNum, 1, 0, 0);
}

void ParticleManager2D::Add(const int& life,
	const XMFLOAT2& position, const XMFLOAT2& velocity, const XMFLOAT2& accel,
	const float& start_scale, const float& end_scale,
	const XMFLOAT4& start_color, const XMFLOAT4& end_color,const float Gravity, float divi)
{
	// リストに要素を追加
	particles.emplace_front();
	// 追加した要素の参照
	Particle2D& p = particles.front();
	p.num_frame = life;
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
	p.s_color = start_color;
	p.e_color = end_color;
	p.m_GraVity = Gravity;
	p.DiviSpeed = divi;
}

void ParticleManager2D::InitializeDescriptorHeap() {
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void ParticleManager2D::InitializeGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob;	// ジオメトリシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleVS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticlePS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ジオメトリシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ParticleGS.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "gs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&gsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // スケール
			"TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 色
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// デプスの書き込みを禁止
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;//半透明
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//半透明
	// 加算ブレンディング
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;
	//// 減算ブレンディング
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	//DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&alphaBlendPipelineSet.rootsignature));
	if (FAILED(result)) {
		assert(0);
	}

	gpipeline.pRootSignature = alphaBlendPipelineSet.rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&alphaBlendPipelineSet.pipelinestate));

	if (FAILED(result)) {
		assert(0);
	}
}

std::unique_ptr<ParticleManager2D> ParticleManager2D::Create(UINT texNumber)
{
	//インスタンスを生成
	auto sprite = std::make_unique<ParticleManager2D>();
	if (sprite == nullptr) {
		return nullptr;
	}

	HRESULT result;
	//元createmodel
#pragma region
	this->texNumber = texNumber;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos) * vertexCount),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPos) * vertexCount;
	vbView.StrideInBytes = sizeof(VertexPos);

#pragma endregion

	// 初期化
	if (!sprite->Initialize()) {
		assert(0);
		return nullptr;
	}

	return sprite;
}

void ParticleManager2D::AllDelete()
{
	//要素の全削除
	particles.clear();
}
