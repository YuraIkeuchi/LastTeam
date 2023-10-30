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
ComPtr<ID3D12Resource> ParticleManager2D::texbuff[srvCount];
std::string ParticleManager2D::extensionPath = ".png";

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


ParticleManager2D::ParticleManager2D(UINT texNumber, const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT4& color):
	IKESprite(texNumber,position,size,color,anchorpoint,isFlipX,isFlipY)
{
}

void ParticleManager2D::Update() {
	HRESULT result = S_FALSE;

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
		it->position = it->position + it->velocity / it->DiviSpeed;

		// カラーの線形補間
		it->color.x = it->s_color.x + (it->e_color.x - it->s_color.x) / f;
		it->color.y = it->s_color.y + (it->e_color.y - it->s_color.y) / f;
		it->color.z = it->s_color.z + (it->e_color.z - it->s_color.z) / f;
		it->color.w = it->s_color.w + (it->e_color.w - it->s_color.w) / f;

		// スケールの線形補間
		it->scale = it->s_scale + (it->e_scale - it->s_scale) / f;
	}

	// 頂点バッファへデータ転送
	TransferVertices();

	// 定数バッファへデータ転送
	IKESprite::TransferConstBuff();
}

void ParticleManager2D::Draw()
{
	UINT drawNum = (UINT)std::distance(particles.begin(), particles.end());
	if (drawNum > vertexCount) {
		drawNum = vertexCount;
	}

	// パーティクルが1つもない場合
	if (drawNum == 0) {
		return;
	}

	IKESprite::Draw();
}

void ParticleManager2D::Add(const int& life,
	const XMFLOAT2& position, const XMFLOAT2& velocity, const XMFLOAT2& accel,
	const float& start_scale, const float& end_scale,
	const XMFLOAT4& start_color, const XMFLOAT4& end_color, const float Gravity, float divi)
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

ParticleManager2D* ParticleManager2D::Create(UINT texNumber, const XMFLOAT2& position, const XMFLOAT4& color,
	const XMFLOAT2& anchorpoint, bool isFlipX, bool isFlipY)
{
	// 仮サイズ
	XMFLOAT2 size = { 100.0f, 100.0f };

	if (texBuff[texNumber])
	{
		// テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();
		// スプライトのサイズをテクスチャのサイズに設定
		size = { static_cast<float>(resDesc.Width), static_cast<float>(resDesc.Height) };
	}

	// Spriteのインスタンスを生成
	ParticleManager2D* sprite = new ParticleManager2D(0,{1.0f,1.0f});
	if (sprite == nullptr)
	{
		return nullptr;
	}

	// 初期化
	if (!sprite->Initialize())
	{
		assert(0);
		return nullptr;
	}

	return sprite;
}

void ParticleManager2D::TransferVertices()
{
	HRESULT result = S_FALSE;
	int vertCount = 0;	

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		// パーティクルの情報を1つずつ反映
		for (std::forward_list<Particle2D>::iterator it = particles.begin();
			it != particles.end();
			it++) {
			// 左下、左上、右下、右上
			enum { LB, LT, RB, RT };

			float left = (0.0f - anchorpoint.x) * size.x;
			float right = (1.0f - anchorpoint.x) * size.x;
			float top = (0.0f - anchorpoint.y) * size.y;
			float bottom = (1.0f - anchorpoint.y) * size.y;
			if (isFlipX)
			{
				// 左右入れ替え
				left = -left;
				right = -right;
			}

			if (isFlipY)
			{
				// 上下入れ替え
				top = -top;
				bottom = -bottom;
			}

			// 頂点データ
			VertexPosUv vertices[vertNum];

			vertices[LB].pos = { left, bottom, 0.0f }; // 左下
			vertices[LT].pos = { left, top, 0.0f }; // 左上
			vertices[RB].pos = { right, bottom, 0.0f }; // 右下
			vertices[RT].pos = { right, top, 0.0f }; // 右上

			// テクスチャ情報取得
			if (texBuff[texNumber])
			{
				D3D12_RESOURCE_DESC resDesc = texBuff[texNumber]->GetDesc();

				float tex_left = texBase.x / resDesc.Width;
				float tex_right = (texBase.x + texSize.x) / resDesc.Width;
				float tex_top = texBase.y / resDesc.Height;
				float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

				vertices[LB].uv = { tex_left, tex_bottom }; // 左下
				vertices[LT].uv = { tex_left, tex_top }; // 左上
				vertices[RB].uv = { tex_right, tex_bottom }; // 右下
				vertices[RT].uv = { tex_right, tex_top }; // 右上
			}

			memcpy(vertMap, vertices, sizeof(vertices));

			// 次の頂点へ
			vertMap++;
			if (++vertCount >= vertexCount) {
				break;
			}
		}
		vertBuff->Unmap(0, nullptr);
	}
}

void ParticleManager2D::AllDelete()
{
	//要素の全削除
	particles.clear();
}
