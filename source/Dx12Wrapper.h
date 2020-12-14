//---------------------------------------------------------------------------
//!	@file	dx12Wrapper.h
//!	@brief	DirectX12ラッパー関数
//---------------------------------------------------------------------------
#pragma once

class Dx12Wrapper
{
    SIZE _winSize;

    //DXGIまわり
    com_ptr<IDXGIFactory4>   _dxgiFactory = nullptr;   //DXGIインターフェイス
    com_ptr<IDXGISwapChain4> _swapchain   = nullptr;   //スワップチェイン

    //DirectX12まわり
    com_ptr<ID3D12Device>              _dev          = nullptr;   //デバイス
    com_ptr<ID3D12CommandAllocator>    _cmdAllocator = nullptr;   //コマンドアロケータ
    com_ptr<ID3D12GraphicsCommandList> _cmdList      = nullptr;   //コマンドリスト
    com_ptr<ID3D12CommandQueue>        _cmdQueue     = nullptr;   //コマンドキュー

    //表示に関わるバッファ周り
    com_ptr<ID3D12Resource>         _depthBuffer = nullptr;   //深度バッファ
    std::vector<ID3D12Resource*>    _backBuffers;             //バックバッファ(2つ以上…スワップチェインが確保)
    com_ptr<ID3D12DescriptorHeap>   _rtvHeaps = nullptr;      //レンダーターゲット用デスクリプタヒープ
    com_ptr<ID3D12DescriptorHeap>   _dsvHeap  = nullptr;      //深度バッファビュー用デスクリプタヒープ
    std::unique_ptr<D3D12_VIEWPORT> _viewport;                //ビューポート
    std::unique_ptr<D3D12_RECT>     _scissorrect;             //シザー矩形

    //シーンを構成するバッファまわり
    com_ptr<ID3D12Resource> _sceneConstBuff = nullptr;

    struct SceneData
    {
        DirectX::XMMATRIX view;   //ビュー行列
        DirectX::XMMATRIX proj;   //プロジェクション行列
        DirectX::XMFLOAT3 eye;    //視点座標
    };
    SceneData*                    _mappedSceneData;
    com_ptr<ID3D12DescriptorHeap> _sceneDescHeap = nullptr;

    //フェンス
    com_ptr<ID3D12Fence> _fence    = nullptr;
    UINT64               _fenceVal = 0;

    //最終的なレンダーターゲットの生成
    HRESULT CreateFinalRenderTargets();
    //デプスステンシルビューの生成
    HRESULT CreateDepthStencilView();

    //スワップチェインの生成
    HRESULT CreateSwapChain(const HWND& hwnd);

    //DXGIまわり初期化
    HRESULT InitializeDXGIDevice();

    //コマンドまわり初期化
    HRESULT InitializeCommand();

    //ビュープロジェクション用ビューの生成
    HRESULT CreateSceneView();

    //ロード用テーブル
    using LoadLambda_t = std::function<HRESULT(const std::wstring& path, DirectX::TexMetadata*, DirectX::ScratchImage&)>;
    std::map<std::string, LoadLambda_t> _loadLambdaTable;
    //テクスチャテーブル
    std::unordered_map<std::string, com_ptr<ID3D12Resource>> _textureTable;
    //テクスチャローダテーブルの作成
    void CreateTextureLoaderTable();
    //テクスチャ名からテクスチャバッファ作成、中身をコピー
    ID3D12Resource* CreateTextureFromFile(const char* texpath);

    com_ptr<ID3D12DescriptorHeap> CreateDescriptorHeapForImgui();
    com_ptr<ID3D12DescriptorHeap> _heapForImgui;

public:
    Dx12Wrapper(HWND hwnd);
    ~Dx12Wrapper();

    void Update();
    void BeginDraw();
    void EndDraw();
    void ExecuteCommand();
    ///テクスチャパスから必要なテクスチャバッファへのポインタを返す
    ///@param texpath テクスチャファイルパス
    com_ptr<ID3D12Resource> GetTextureByPath(const char* texpath);

    com_ptr<ID3D12DescriptorHeap> GetHeapForImgui();


    com_ptr<ID3D12Device>              Device();        //デバイス
    com_ptr<ID3D12GraphicsCommandList> CommandList();   //コマンドリスト
    com_ptr<IDXGISwapChain4>           Swapchain();     //スワップチェイン

    void SetScene();
};
