#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

// Jsonデータ
struct JsonData {

	struct ObjectData {
		// ファイル名
		std::string fileName;
		// リスポーン名
		std::string ResPornName;
		// ギミック名
		std::string GimmickName;
		// 平行移動
		DirectX::XMVECTOR translation;
		// 回転角
		DirectX::XMVECTOR rotation;
		// スケーリング
		DirectX::XMVECTOR scaling;
	};

	// オブジェクト配列
	std::vector<ObjectData> objects;
};

//Json読み込み
class JsonLoader {
private:	//エイリアス
//std::を省略
	using string = std::string;

public:
	//パス
	static const string kDefaultBaseDirectory;
	//上記のテクスチャ版
	static const string filename;
	//テクスチャがない場合の標準テクスチャファイル名
	static const string kExtension;
public:
	static JsonData* LoadFile(const std::string& fileName);
};