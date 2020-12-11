//---------------------------------------------------------------------------
//!	@file	debug.h
//!	@brief	デバッグ関連
//---------------------------------------------------------------------------
#pragma once

namespace debug {

//! メッセージつきassert
#define ASSERT_MESSAGE(CONDITION_, MESSAGE_) ((CONDITION_) && (MESSAGE_))

}   // namespace debug
