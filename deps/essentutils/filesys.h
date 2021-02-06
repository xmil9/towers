//
// essentutils
// File system utilities.
//
// Jul-2019, Michael Lindner
// MIT license
//
#pragma once
#include "esl_api.h"
#include <filesystem>


namespace esl
{
///////////////////

// Returns the path of the directory where the app's executable is located in.
ESL_API std::filesystem::path appDirectory();
// Returns the path of a directory where user-specific file can be stored in.
ESL_API std::filesystem::path userDirectory();

} // namespace esl
