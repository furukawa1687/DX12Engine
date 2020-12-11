//---------------------------------------------------------------------------
//!	@file	StringEncode.cpp
//!	@brief	文字列UTF8 ←→ UNICODE 相互変換
//---------------------------------------------------------------------------
#include <system_error>

namespace string {

//---------------------------------------------------------------------------
//! UTF8→UNICODE変換
//---------------------------------------------------------------------------
std::wstring convertUtf8ToUnicode(std::string_view src)
{
    auto const           dest_size = ::MultiByteToWideChar(CP_UTF8, 0u, src.data(), static_cast<int>(src.size()), nullptr, 0u);
    std::vector<wchar_t> dest(dest_size);
    if(::MultiByteToWideChar(CP_UTF8, 0u, src.data(), static_cast<int>(src.size()), dest.data(), static_cast<int>(dest.size())) == 0) {
        throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
    return std::wstring(dest.begin(), dest.end());
}

//---------------------------------------------------------------------------
//! UNICODE→UTF8変換
//---------------------------------------------------------------------------
std::string convertUnicodeToUtf8(std::wstring_view src)
{
    auto const        dest_size = ::WideCharToMultiByte(CP_UTF8, 0u, src.data(), static_cast<int>(src.size()), nullptr, 0, nullptr, nullptr);
    std::vector<char> dest(dest_size);
    if(::WideCharToMultiByte(CP_UTF8, 0u, src.data(), static_cast<int>(src.size()), dest.data(), static_cast<int>(dest.size()), nullptr, nullptr) == 0) {
        throw std::system_error{ static_cast<int>(::GetLastError()), std::system_category() };
    }
    return std::string(dest.begin(), dest.end());
}

}   // namespace string