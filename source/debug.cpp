//---------------------------------------------------------------------------
//!	@file	debug.cpp
//!	@brief	デバッグ関連
//---------------------------------------------------------------------------
namespace debug {
//! デバッグログ
void Log(std::string str)
{
#ifdef DEBUG
    ImGui::Begin("Console");
    {
        ImGui::Text(str.c_str());
    }
    ImGui::End();
#endif   // DEBUG
}
}   // namespace debug