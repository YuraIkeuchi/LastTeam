#pragma once

#include "fbxsdk.h"
#include "IKEFBXModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class IKEFbxLoader
{
private: //エイリアス
	//std::を省略
	using string = std::string;


public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static IKEFbxLoader* GetInstance();
	/// <summary>
	///初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	void Initialize(ID3D12Device* device);
	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();
	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName"></param>
	IKEFBXModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="modle"></param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseNodeRecursive(IKEFBXModel* model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(IKEFBXModel* model, FbxNode* fbxNode);

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	//頂点座標読み取り
	void ParseMeshVertices(IKEFBXModel* model, FbxMesh* fbxMesh);
	//面積情報読み取り
	void ParseMeshFaces(IKEFBXModel* model, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(IKEFBXModel* model, FbxNode* fbxNode);
	//テクスチャ読み込み
	void LoadTexture(IKEFBXModel* model, const std::string& fullpath);
	//スキニング情報の読み取り
	void ParseSkin(IKEFBXModel* model, FbxMesh* fbxMesh);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

private:
	// privateなコンストラクタ（シングルトンパターン）
	IKEFbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~IKEFbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	IKEFbxLoader(const IKEFbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const IKEFbxLoader& obj) = delete;

	//D3D12デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャ
	FbxManager* fbxManager = nullptr;
	//FBXインポータ
	FbxImporter* fbxImporter = nullptr;


public: //定数
	//モデル格納ルートパス
	static const string baseDirectory;
	//上記のテクスチャ版
	static const string texbaseDirectory;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
};
