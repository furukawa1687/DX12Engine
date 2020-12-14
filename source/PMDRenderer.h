#pragma once

class Dx12Wrapper;
class PMDActor;
class PMDRenderer
{
	friend PMDActor;
private:
	Dx12Wrapper& _dx12;

	com_ptr< ID3D12PipelineState> _pipeline = nullptr;//PMD用パイプライン
	com_ptr< ID3D12RootSignature> _rootSignature = nullptr;//PMD用ルートシグネチャ

	//PMD用共通テクスチャ(白、黒、グレイスケールグラデーション)
	com_ptr<ID3D12Resource> _whiteTex = nullptr;
	com_ptr<ID3D12Resource> _blackTex = nullptr;
	com_ptr<ID3D12Resource> _gradTex = nullptr;

	ID3D12Resource* CreateDefaultTexture(size_t width,size_t height);
	ID3D12Resource* CreateWhiteTexture();//白テクスチャの生成
	ID3D12Resource*	CreateBlackTexture();//黒テクスチャの生成
	ID3D12Resource*	CreateGrayGradationTexture();//グレーテクスチャの生成

	//パイプライン初期化
	HRESULT CreateGraphicsPipelineForPMD();
	//ルートシグネチャ初期化
	HRESULT CreateRootSignature();

	bool CheckShaderCompileResult(HRESULT result , ID3DBlob* error=nullptr);

public:
	PMDRenderer(Dx12Wrapper& dx12);
	~PMDRenderer();
	void Update();
	void Draw();
	ID3D12PipelineState* GetPipelineState();
	ID3D12RootSignature* GetRootSignature();
};

