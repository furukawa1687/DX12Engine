//---------------------------------------------------------------------------
//!	@file	Appliction.cpp
//!	@brief	アプリケーション関連
//---------------------------------------------------------------------------

//ウィンドウ定数
const unsigned int window_width  = 1280;
const unsigned int window_height = 720;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
//面倒だけど書かなあかんやつ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if(msg == WM_DESTROY) {   //ウィンドウが破棄されたら呼ばれます
        PostQuitMessage(0);   //OSに対して「もうこのアプリは終わるんや」と伝える
        return 0;
    }
    ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
    return DefWindowProc(hwnd, msg, wparam, lparam);   //規定の処理を行う
}

void Application::CreateGameWindow(HWND& hwnd, WNDCLASSEXW& windowClass)
{
    HINSTANCE hInst = GetModuleHandle(nullptr);
    //ウィンドウクラス生成＆登録
    windowClass.cbSize        = sizeof(WNDCLASSEX);
    windowClass.lpfnWndProc   = (WNDPROC)WindowProcedure;   //コールバック関数の指定
    windowClass.lpszClassName = L"DX12Engine";          //アプリケーションクラス名(適当でいいです)
    windowClass.hInstance     = GetModuleHandle(0);         //ハンドルの取得
    RegisterClassExW(&windowClass);                          //アプリケーションクラス(こういうの作るからよろしくってOSに予告する)
    
    //----------------------------------------------------------
    // ウィンドウを作成
    //----------------------------------------------------------
    u32 style   = WS_OVERLAPPEDWINDOW;   // ウィンドウスタイル
    u32 styleEx = 0;                     // 拡張ウィンドウスタイル
    s32 x       = CW_USEDEFAULT;
    s32 y       = CW_USEDEFAULT;
    s32 w       = CW_USEDEFAULT;
    s32 h       = CW_USEDEFAULT;

    // ウィンドウサイズをUIサイズを考慮して補正
    {
        RECT windowRect{ 0, 0, 1280, 720 };

        AdjustWindowRectEx(&windowRect, style, false, styleEx);
        w = windowRect.right - windowRect.left;
        h = windowRect.bottom - windowRect.top;
    }

        // ウィンドウを作成
    hwnd = CreateWindowExW(
        styleEx,                     // 拡張ウィンドウスタイル
        windowClass.lpszClassName,   // ウィンドウクラス名
        L"DX12Engine",      // タイトル名
        style,                       // ウィンドウスタイル
        x,                           // X座標
        y,                           // Y座標
        w,                           // ウィンドウの幅
        h,                           // ウィンドウの高さ
        nullptr,                     // 親ウィンドウ(なし)
        nullptr,                     // メニューハンドル(なし)
        windowClass.hInstance,       // アプリケーションインスタンスハンドル
        nullptr);                    // WM_CREATEへの引数(任意)
}

SIZE Application::GetWindowSize() const
{
    SIZE ret;
    ret.cx = window_width;
    ret.cy = window_height;
    return ret;
}

void Application::Run()
{
    ShowWindow(_hwnd, SW_SHOW);   //ウィンドウ表示
    float        angle = 0.0f;
    MSG          msg   = {};
    unsigned int frame = 0;
    while(true) {
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //もうアプリケーションが終わるって時にmessageがWM_QUITになる
        if(msg.message == WM_QUIT) {
            break;
        }

        // imgui関連
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        
        //全体の描画準備
        _dx12->BeginDraw();

        //PMD用の描画パイプラインに合わせる
        _dx12->CommandList()->SetPipelineState(_pmdRenderer->GetPipelineState());
        //ルートシグネチャもPMD用に合わせる
        _dx12->CommandList()->SetGraphicsRootSignature(_pmdRenderer->GetRootSignature());

        _dx12->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        _dx12->SetScene();

        _pmdActor->Update();
        _pmdActor->Draw();

        ImGui::Begin("Rendering Test Menu");
        ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
        
        ImGui::End();
        ImGui::Render();


        _dx12->CommandList()->SetDescriptorHeaps(1, _dx12->GetHeapForImgui().GetAddressOf());
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), _dx12->CommandList().Get());

        _dx12->EndDraw();
        //フリップ
        _dx12->Swapchain()->Present(0, 0);
    }
}

bool Application::Initialize()
{
    auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
    CreateGameWindow(_hwnd, _windowClass);

    //DirectX12ラッパー生成＆初期化
    _dx12.reset(new Dx12Wrapper(_hwnd));
    _pmdRenderer.reset(new PMDRenderer(*_dx12));
    //imguiの初期化
    if(ImGui::CreateContext() == nullptr) {
        assert(0);
        return false;
    }
    bool blnResult = ImGui_ImplWin32_Init(_hwnd);
    if(!blnResult) {
        assert(0);
        return false;
    }
    blnResult = ImGui_ImplDX12_Init(_dx12->Device().Get(),                                                   //DirectX12デバイス
                                    1,                                                                 //frames_in_flightと説明にはあるがflightの意味が掴めず(後述)
                                    DXGI_FORMAT_R8G8B8A8_UNORM,                                        //書き込み先RTVのフォーマット
                                    _dx12->GetHeapForImgui().Get(),                                    //imgui用デスクリプタヒープ
                                    _dx12->GetHeapForImgui()->GetCPUDescriptorHandleForHeapStart(),    //CPUハンドル
                                    _dx12->GetHeapForImgui()->GetGPUDescriptorHandleForHeapStart());   //GPUハンドル


    _pmdActor.reset(new PMDActor("model/初音ミク.pmd", *_pmdRenderer));
    _pmdActor->LoadVMDFile("motion/squat2.vmd", "pose");
    _dx12->ExecuteCommand();
    _pmdActor->PlayAnimation();
    return true;
}

void Application::Terminate()
{
    //もうクラス使わんから登録解除してや
    UnregisterClassW(_windowClass.lpszClassName, _windowClass.hInstance);
}

Application& Application::Instance()
{
    static Application instance;
    return instance;
}

Application::Application()
{
}

Application::~Application()
{
}