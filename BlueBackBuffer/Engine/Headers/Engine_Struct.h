#pragma once

#ifndef __ENGINE_DEFINE_H__

#ifdef ENGINE_EXPORT
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define __ENGINE_DEFINE_H__
#endif