#pragma once
#include "IKEObject3d.h"
class TouchableObject :
	public IKEObject3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(IKEModel* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(IKEModel * model);
	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	inline void SetScale(const XMFLOAT3& scale) { this->scale = scale; }
};

