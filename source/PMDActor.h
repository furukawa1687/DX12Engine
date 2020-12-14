//---------------------------------------------------------------------------
//!	@file	PMDActor.h
//!	@brief	PMDアクター
//---------------------------------------------------------------------------
#pragma once

class Dx12Wrapper;
class PMDRenderer;
class PMDActor
{
    friend PMDRenderer;

private:
    u32               _duration = 0;
    PMDRenderer&      _renderer;
    Dx12Wrapper&      _dx12;
    DirectX::XMMATRIX _localMat;

    //頂点関連
    com_ptr<ID3D12Resource>  _vb     = nullptr;   //! 頂点シェーダー
    com_ptr<ID3D12Resource>  _ib     = nullptr;   //! インデックスシェーダー
    D3D12_VERTEX_BUFFER_VIEW _vbView = {};        //! 頂点ビュー
    D3D12_INDEX_BUFFER_VIEW  _ibView = {};        //! インデックスビュ

    com_ptr<ID3D12Resource>       _transformMat  = nullptr;   //! 座標変換行列(今はワールドのみ)
    com_ptr<ID3D12DescriptorHeap> _transformHeap = nullptr;   //! 座標変換ヒープ

    //シェーダ側に投げられるマテリアルデータ
    struct MaterialForHlsl
    {
        DirectX::XMFLOAT3 diffuse;       //! ディフューズ色
        f32               alpha;         //! ディフューズα
        DirectX::XMFLOAT3 specular;      //! スペキュラ色
        f32               specularity;   //! スペキュラの強さ(乗算値)
        DirectX::XMFLOAT3 ambient;       //! アンビエント色
    };
    //それ以外のマテリアルデータ
    struct AdditionalMaterial
    {
        std::string texPath;   //! テクスチャファイルパス
        s32         toonIdx;   //! トゥーン番号
        bool        edgeFlg;   //! マテリアル毎の輪郭線フラグ
    };
    //まとめたもの
    struct Material
    {
        u32                indicesNum;   //インデックス数
        MaterialForHlsl    material;
        AdditionalMaterial additional;
    };

    struct Transform
    {
        //内部に持ってるXMMATRIXメンバが16バイトアライメントであるため
        //Transformをnewする際には16バイト境界に確保する
        void*             operator new(size_t size);
        DirectX::XMMATRIX world;
    };

    Transform               _transform;
    DirectX::XMMATRIX*      _mappedMatrices = nullptr;
    com_ptr<ID3D12Resource> _transformBuff  = nullptr;

    //マテリアル関連
    std::vector<Material>                _materials;
    com_ptr<ID3D12Resource>              _materialBuff = nullptr;
    std::vector<com_ptr<ID3D12Resource>> _textureResources;
    std::vector<com_ptr<ID3D12Resource>> _sphResources;
    std::vector<com_ptr<ID3D12Resource>> _spaResources;
    std::vector<com_ptr<ID3D12Resource>> _toonResources;

    // ボーン関連
    std::vector<DirectX::XMMATRIX> _boneMatrices;

    struct BoneNode
    {
        u32                    boneIdx;        //! ボーンインデックス
        u32                    boneType;       //! ボーン種類
        u32                    parentBone;     //! 親ボーン
        u32                    ikParentBone;   //! IK親ボーン
        DirectX::XMFLOAT3      startPos;       //! ボーン基準点（回転中心）
        std::vector<BoneNode*> children;       //! 子ノード
    };

    std::unordered_map<std::string, BoneNode> _boneNodeTable;
    std::vector<std::string>                  _boneNameArray;          //! ボーン名を格納しておく
    std::vector<BoneNode*>                    _boneNodeAddressArray;   //! ノードを格納しておく

    struct PMDIK
    {
        u16              boneIdx;      //! IK対象のボーンを示す
        u16              targetIdx;    //! ターゲットに近づけるためのボーンのインデックス
        u16              iterations;   //! 試行回数
        f32              limit;        //! 一回当たりの回転制限
        std::vector<u16> nodeIdxes;    //! 間のノード番号
    };
    std::vector<PMDIK> _ikData;

    // 読み込んだマテリアルをもとにマテリアルバッファを作成
    HRESULT CreateMaterialData();

    com_ptr<ID3D12DescriptorHeap> _materialHeap = nullptr;   //マテリアルヒープ(5個ぶん)
    // マテリアル＆テクスチャのビューを作成
    HRESULT CreateMaterialAndTextureView();

    // 座標変換用ビューの生成
    HRESULT CreateTransformView();

    // PMDファイルのロード
    //! @param path ファイルパス
    HRESULT LoadPMDFile(const char* path);

    // 再帰関数による行列の重ね掛け
    //! @param node ボーンノード
    //! @param mat 行列
    void RecursiveMatrixMultiply(BoneNode* node,const DirectX::XMMATRIX& mat);
    
    f32  _angle;   //テスト用Y軸回転

    // キーフレーム構造体
    struct KeyFrame
    {
        u32               frameNo;
        DirectX::XMVECTOR quaternion;
        DirectX::XMFLOAT3 offset;   // クォータニオン
        DirectX::XMFLOAT2 p1, p2;   //ベジェの中間コントロールポイント
        // コンストラクタ
        KeyFrame(u32 fno, DirectX::XMVECTOR& q, DirectX::XMFLOAT3& ofs, DirectX::XMFLOAT2& ip1, const DirectX::XMFLOAT2& ip2)
        : frameNo(fno)
        , quaternion(q)
        , offset(ofs)
        , p1(ip1)
        , p2(ip2)
        {
        }
    };
    std::unordered_map<std::string, std::vector<KeyFrame>> _motionData;

    // ベジェ曲線の取得
    f32 GetYFromXOnBezier(f32 x, const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b, u8 n = 12);

    std::vector<u32> _kneeIdxes;

    DWORD _startTime;

    // モーション更新
    void MotionUpdate();

    // CCD-IKによるボーン方向を解決
    //! @param ik 対象IKオブジェクト
    void SolveCCDIK(const PMDIK& ik);

    // 余弦定理IKによるボーン方向を解決
    //! @param ik 対象IKオブジェクト
    //! @details ルートから末端までの点が3点しかない場合
    void SolveCosineIK(const PMDIK& ik);

    // LookAt行列によるボーン方向を解決
    //! @param ik 対象IKオブジェクト
    //! @details IKボーンにルートと末端しかない場合
    void SolveLookAt(const PMDIK& ik);

    // IK解決
    void IKSolve(s32 frameNo);

    //IKオンオフデータ
    struct VMDIKEnable
    {
        u32                                   frameNo;
        std::unordered_map<std::string, bool> ikEnableTable;
    };
    std::vector<VMDIKEnable> _ikEnableData;

public:
    // コンストラクタ
    //! @param filepath 読み込むモデルファイルパス
    //! @param rederer モデル描画システムへの参照
    PMDActor(const char* filepath, PMDRenderer& renderer);
    // デストラクタ
    ~PMDActor();

    //クローンは頂点およびマテリアルは共通のバッファを見るようにする
    PMDActor* Clone();

    // VMDファイルのロード
    //! @param filepaht ファイルパス
    //! @param name モーションデータの名前
    void      LoadVMDFile(const char* filepath, const char* name);
    // 更新
    void      Update();
    // 描画
    void      Draw();
    // アニメーション再生
    void      PlayAnimation();

    // LookAt(指定した座標を向く)
    //! @param x 向かせたい座標のx座標
    //! @param y 向かせたい座標のY座標
    //! @param z 向かせたい座標のZ座標
    void LookAt(f32 x, f32 y, f32 z);
};
