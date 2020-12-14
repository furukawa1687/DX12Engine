#pragma once

class Dx12Wrapper;
class PMDActor;
class PMDRenderer
{
	friend PMDActor;
private:
	Dx12Wrapper& _dx12;

	com_ptr< ID3D12PipelineState> _pipeline = nullptr;//PMD�p�p�C�v���C��
	com_ptr< ID3D12RootSignature> _rootSignature = nullptr;//PMD�p���[�g�V�O�l�`��

	//PMD�p���ʃe�N�X�`��(���A���A�O���C�X�P�[���O���f�[�V����)
	com_ptr<ID3D12Resource> _whiteTex = nullptr;
	com_ptr<ID3D12Resource> _blackTex = nullptr;
	com_ptr<ID3D12Resource> _gradTex = nullptr;

	ID3D12Resource* CreateDefaultTexture(size_t width,size_t height);
	ID3D12Resource* CreateWhiteTexture();//���e�N�X�`���̐���
	ID3D12Resource*	CreateBlackTexture();//���e�N�X�`���̐���
	ID3D12Resource*	CreateGrayGradationTexture();//�O���[�e�N�X�`���̐���

	//�p�C�v���C��������
	HRESULT CreateGraphicsPipelineForPMD();
	//���[�g�V�O�l�`��������
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

