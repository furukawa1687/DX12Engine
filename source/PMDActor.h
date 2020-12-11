#pragma once

class Dx12Wrapper;
class PMDRenderer;

class PMDActor
{
	friend PMDRenderer;
private:
	PMDRenderer& _renderer;
	Dx12Wrapper& _dx12;
	
	//頂点関連
	com_ptr<ID3D12Resource> _vb = nullptr;
	com_ptr<ID3D12Resource> _ib = nullptr;
	D3D12_VERTEX_BUFFER_VIEW _vbView = {};
	D3D12_INDEX_BUFFER_VIEW _ibView = {};

	com_ptr<ID3D12Resource> _transformMat = nullptr;//座標変換行列(今はワールドのみ)
	com_ptr<ID3D12DescriptorHeap> _transformHeap = nullptr;//座標変換ヒープ

	//シェーダ側に投げられるマテリアルデータ
	struct MaterialForHlsl {
		DirectX::XMFLOAT3 diffuse; //ディフューズ色
		float alpha; // ディフューズα
		DirectX::XMFLOAT3 specular; //スペキュラ色
		float specularity;//スペキュラの強さ(乗算値)
		DirectX::XMFLOAT3 ambient; //アンビエント色
	};
	//それ以外のマテリアルデータ
	struct AdditionalMaterial {
		std::string texPath;//テクスチャファイルパス
		int toonIdx; //トゥーン番号
		bool edgeFlg;//マテリアル毎の輪郭線フラグ
	};
	//まとめたもの
	struct Material {
		unsigned int indicesNum;//インデックス数
		MaterialForHlsl material;
		AdditionalMaterial additional;
	};

	struct Transform {
		//内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
		//Transformをnewする際には16バイト境界に確保する
		void* operator new(size_t size);
		DirectX::XMMATRIX world;
	};

	Transform _transform;
	Transform* _mappedTransform = nullptr;
	com_ptr<ID3D12Resource> _transformBuff = nullptr;

	//マテリアル関連
	std::vector<Material> _materials;
	com_ptr<ID3D12Resource> _materialBuff = nullptr;
	std::vector<com_ptr<ID3D12Resource>> _textureResources;
	std::vector<com_ptr<ID3D12Resource>> _sphResources;
	std::vector<com_ptr<ID3D12Resource>> _spaResources;
	std::vector<com_ptr<ID3D12Resource>> _toonResources;
	
	//読み込んだマテリアルをもとにマテリアルバッファを作成
	HRESULT CreateMaterialData();
	
	com_ptr< ID3D12DescriptorHeap> _materialHeap = nullptr;//マテリアルヒープ(5個ぶん)
	//マテリアル＆テクスチャのビューを作成
	HRESULT CreateMaterialAndTextureView();

	//座標変換用ビューの生成
	HRESULT CreateTransformView();

	//PMDファイルのロード
	HRESULT LoadPMDFile(const char* path);

	float _angle;//テスト用Y軸回転
public:
	PMDActor(const char* filepath,PMDRenderer& renderer);
	~PMDActor();
	///クローンは頂点およびマテリアルは共通のバッファを見るようにする
	PMDActor* Clone();
	void Update();
	void Draw();

};

