//---------------------------------------------------------------------------
//!	@file	main.cpp
//!	@brief	アプリケーションメイン
//---------------------------------------------------------------------------

//int main() {
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    auto& app = Application::Instance();
    if(!app.Initialize())
        return -1;
    app.Run();
    app.Terminate();
    return 0;
}