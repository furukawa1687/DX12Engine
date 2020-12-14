//---------------------------------------------------------------------------
//!	@file	PMDActor.h
//!	@brief	PMD�A�N�^�[
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

    //���_�֘A
    com_ptr<ID3D12Resource>  _vb     = nullptr;   //! ���_�V�F�[�_�[
    com_ptr<ID3D12Resource>  _ib     = nullptr;   //! �C���f�b�N�X�V�F�[�_�[
    D3D12_VERTEX_BUFFER_VIEW _vbView = {};        //! ���_�r���[
    D3D12_INDEX_BUFFER_VIEW  _ibView = {};        //! �C���f�b�N�X�r��

    com_ptr<ID3D12Resource>       _transformMat  = nullptr;   //! ���W�ϊ��s��(���̓��[���h�̂�)
    com_ptr<ID3D12DescriptorHeap> _transformHeap = nullptr;   //! ���W�ϊ��q�[�v

    //�V�F�[�_���ɓ�������}�e���A���f�[�^
    struct MaterialForHlsl
    {
        DirectX::XMFLOAT3 diffuse;       //! �f�B�t���[�Y�F
        f32               alpha;         //! �f�B�t���[�Y��
        DirectX::XMFLOAT3 specular;      //! �X�y�L�����F
        f32               specularity;   //! �X�y�L�����̋���(��Z�l)
        DirectX::XMFLOAT3 ambient;       //! �A���r�G���g�F
    };
    //����ȊO�̃}�e���A���f�[�^
    struct AdditionalMaterial
    {
        std::string texPath;   //! �e�N�X�`���t�@�C���p�X
        s32         toonIdx;   //! �g�D�[���ԍ�
        bool        edgeFlg;   //! �}�e���A�����̗֊s���t���O
    };
    //�܂Ƃ߂�����
    struct Material
    {
        u32                indicesNum;   //�C���f�b�N�X��
        MaterialForHlsl    material;
        AdditionalMaterial additional;
    };

    struct Transform
    {
        //�����Ɏ����Ă�XMMATRIX�����o��16�o�C�g�A���C�����g�ł��邽��
        //Transform��new����ۂɂ�16�o�C�g���E�Ɋm�ۂ���
        void*             operator new(size_t size);
        DirectX::XMMATRIX world;
    };

    Transform               _transform;
    DirectX::XMMATRIX*      _mappedMatrices = nullptr;
    com_ptr<ID3D12Resource> _transformBuff  = nullptr;

    //�}�e���A���֘A
    std::vector<Material>                _materials;
    com_ptr<ID3D12Resource>              _materialBuff = nullptr;
    std::vector<com_ptr<ID3D12Resource>> _textureResources;
    std::vector<com_ptr<ID3D12Resource>> _sphResources;
    std::vector<com_ptr<ID3D12Resource>> _spaResources;
    std::vector<com_ptr<ID3D12Resource>> _toonResources;

    // �{�[���֘A
    std::vector<DirectX::XMMATRIX> _boneMatrices;

    struct BoneNode
    {
        u32                    boneIdx;        //! �{�[���C���f�b�N�X
        u32                    boneType;       //! �{�[�����
        u32                    parentBone;     //! �e�{�[��
        u32                    ikParentBone;   //! IK�e�{�[��
        DirectX::XMFLOAT3      startPos;       //! �{�[����_�i��]���S�j
        std::vector<BoneNode*> children;       //! �q�m�[�h
    };

    std::unordered_map<std::string, BoneNode> _boneNodeTable;
    std::vector<std::string>                  _boneNameArray;          //! �{�[�������i�[���Ă���
    std::vector<BoneNode*>                    _boneNodeAddressArray;   //! �m�[�h���i�[���Ă���

    struct PMDIK
    {
        u16              boneIdx;      //! IK�Ώۂ̃{�[��������
        u16              targetIdx;    //! �^�[�Q�b�g�ɋ߂Â��邽�߂̃{�[���̃C���f�b�N�X
        u16              iterations;   //! ���s��
        f32              limit;        //! ��񓖂���̉�]����
        std::vector<u16> nodeIdxes;    //! �Ԃ̃m�[�h�ԍ�
    };
    std::vector<PMDIK> _ikData;

    // �ǂݍ��񂾃}�e���A�������ƂɃ}�e���A���o�b�t�@���쐬
    HRESULT CreateMaterialData();

    com_ptr<ID3D12DescriptorHeap> _materialHeap = nullptr;   //�}�e���A���q�[�v(5�Ԃ�)
    // �}�e���A�����e�N�X�`���̃r���[���쐬
    HRESULT CreateMaterialAndTextureView();

    // ���W�ϊ��p�r���[�̐���
    HRESULT CreateTransformView();

    // PMD�t�@�C���̃��[�h
    //! @param path �t�@�C���p�X
    HRESULT LoadPMDFile(const char* path);

    // �ċA�֐��ɂ��s��̏d�ˊ|��
    //! @param node �{�[���m�[�h
    //! @param mat �s��
    void RecursiveMatrixMultiply(BoneNode* node,const DirectX::XMMATRIX& mat);
    
    f32  _angle;   //�e�X�g�pY����]

    // �L�[�t���[���\����
    struct KeyFrame
    {
        u32               frameNo;
        DirectX::XMVECTOR quaternion;
        DirectX::XMFLOAT3 offset;   // �N�H�[�^�j�I��
        DirectX::XMFLOAT2 p1, p2;   //�x�W�F�̒��ԃR���g���[���|�C���g
        // �R���X�g���N�^
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

    // �x�W�F�Ȑ��̎擾
    f32 GetYFromXOnBezier(f32 x, const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b, u8 n = 12);

    std::vector<u32> _kneeIdxes;

    DWORD _startTime;

    // ���[�V�����X�V
    void MotionUpdate();

    // CCD-IK�ɂ��{�[������������
    //! @param ik �Ώ�IK�I�u�W�F�N�g
    void SolveCCDIK(const PMDIK& ik);

    // �]���藝IK�ɂ��{�[������������
    //! @param ik �Ώ�IK�I�u�W�F�N�g
    //! @details ���[�g���疖�[�܂ł̓_��3�_�����Ȃ��ꍇ
    void SolveCosineIK(const PMDIK& ik);

    // LookAt�s��ɂ��{�[������������
    //! @param ik �Ώ�IK�I�u�W�F�N�g
    //! @details IK�{�[���Ƀ��[�g�Ɩ��[�����Ȃ��ꍇ
    void SolveLookAt(const PMDIK& ik);

    // IK����
    void IKSolve(s32 frameNo);

    //IK�I���I�t�f�[�^
    struct VMDIKEnable
    {
        u32                                   frameNo;
        std::unordered_map<std::string, bool> ikEnableTable;
    };
    std::vector<VMDIKEnable> _ikEnableData;

public:
    // �R���X�g���N�^
    //! @param filepath �ǂݍ��ރ��f���t�@�C���p�X
    //! @param rederer ���f���`��V�X�e���ւ̎Q��
    PMDActor(const char* filepath, PMDRenderer& renderer);
    // �f�X�g���N�^
    ~PMDActor();

    //�N���[���͒��_����у}�e���A���͋��ʂ̃o�b�t�@������悤�ɂ���
    PMDActor* Clone();

    // VMD�t�@�C���̃��[�h
    //! @param filepaht �t�@�C���p�X
    //! @param name ���[�V�����f�[�^�̖��O
    void      LoadVMDFile(const char* filepath, const char* name);
    // �X�V
    void      Update();
    // �`��
    void      Draw();
    // �A�j���[�V�����Đ�
    void      PlayAnimation();

    // LookAt(�w�肵�����W������)
    //! @param x �������������W��x���W
    //! @param y �������������W��Y���W
    //! @param z �������������W��Z���W
    void LookAt(f32 x, f32 y, f32 z);
};
