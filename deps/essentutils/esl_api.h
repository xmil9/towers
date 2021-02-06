//
// essentutils
// Import/export directives.
//
// Jun-2019, Michael Lindner
// MIT license
//
#pragma once

// Projects that use the DLL target build of essentuils have
// to define the ESL_DLL macro to make ESL_API
// resolve to '__declspec(dllimport)'.
#ifdef ESL_DLL
#  ifdef EXPORT_API
#    ifdef _MSC_VER
#      define ESL_API __declspec(dllexport)
#    endif
#  else
#    ifdef _MSC_VER
#      define ESL_API __declspec(dllimport)
#    endif
#  endif
#endif

// If not defined yet, define is as empty/nothing.
#ifndef ESL_API
#  define ESL_API
#endif
