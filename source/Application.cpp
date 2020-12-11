//---------------------------------------------------------------------------
//!	@file	debug.cpp
//!	@brief	デバッグ関連
//---------------------------------------------------------------------------

//ウィンドウ定数
const unsigned int window_width  = 1280;
const unsigned int window_height = 720;

//面倒だけど書かなあかんやつ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if(msg == WM_DESTROY) {   //ウィンドウが破棄されたら呼ばれます
        PostQuitMessage(0);   //OSに対して「もうこのアプリは終わるんや」と伝える
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);   //規定の処理を行う
}

void Application::CreateGameWindow(HWND& hwnd, WNDCLASSEXW& windowClass)
{
    HINSTANCE hInst = GetModuleHandle(nullptr);
    //ウィンドウクラス生成＆登録
    windowClass.cbSize        = sizeof(WNDCLASSEX);
    windowClass.lpfnWndProc   = (WNDPROC)WindowProcedure;   //コールバック関数の指定
    windowClass.lpszClassName = L"DirectXTest";          //アプリケーションクラス名(適当でいいです)
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
        L"Direct3D12 サンプル",      // タイトル名
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

        _dx12->EndDraw();

        //フリップ
        _dx12->Swapchain()->Present(1, 0);
    }
}

bool Application::Initialize()
{
    auto result = CoInitializeEx(0, COINIT_MULTITHREADED);
    CreateGameWindow(_hwnd, _windowClass);

    //DirectX12ラッパー生成＆初期化
    _dx12.reset(new Dx12Wrapper(_hwnd));
    _pmdRenderer.reset(new PMDRenderer(*_dx12));
    _pmdActor.reset(new PMDActor("Model/初音ミク.pmd", *_pmdRenderer));

    return true;
}

void Application::Terminate()
{
    //もうクラス使わんから登録解除してや
    UnregisterClassW(_windowClass.lpszClassName, _windowClass.hInstance);
}

Application&
Application::Instance()
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