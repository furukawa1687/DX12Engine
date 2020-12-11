//---------------------------------------------------------------------------
//!	@file	StringEncode.h
//!	@brief	文字列UTF8 ←→ UNICODE 相互変換
//---------------------------------------------------------------------------
#pragma once

namespace string {

//! UTF8→UNICODE変換
std::wstring convertUtf8ToUnicode(std::string_view src);

//! UNICODE→UTF8変換
std::string convertUnicodeToUtf8(std::wstring_view src);

}   // namespace string
