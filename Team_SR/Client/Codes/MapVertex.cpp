#include "stdafx.h"

//#include "MapVertex.h"
//
//
//USING(Engine)
//
//const DWORD CMapVertex::Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
//
//CMapVertex::CMapVertex(LPDIRECT3DDEVICE9 pDevice , const std::wstring FilePath)
//	: CComponent(pDevice), 
//		FilePath{ std::move(FilePath) } ,					
//	_InfosPtr{ std::make_shared<std::vector<Info>>() }
//{
//
//};
//
//HRESULT CMapVertex::ReadyComponentPrototype()
//{
//	if (FAILED(Super::ReadyComponentPrototype()))
//		return E_FAIL;
//
//	const std::wstring _MtrlFileName = FilePath + L".mtl";
//	std::wfstream _MtlStream(_MtrlFileName);
//
//	if (!_MtlStream.is_open())
//	{
//		::MessageBox(nullptr, __FUNCTIONW__,
//			L"File Open Fail", 0);
//	}
//
//
//	std::map<wstring, MtrlInfo> _MtrlInfo;
//	std::wstring Line;
//	std::wstringstream wss;
//	std::wstring ToKen;
//
//	while (_MtlStream)
//	{
//		Line.clear();
//		ToKen.clear();
//		// 한줄 리딩
//		std::getline(_MtlStream, Line);
//		wss.str(Line);
//		wss.clear();
//		wss >> ToKen;
//
//		// 머테리얼 파싱 시작
//		if (ToKen == L"newmtl")
//		{
//			MtrlInfo _Info;
//			wss >> _Info.MtrlName;
//			std::vector<std::wstring> _FileLines;
//
//			while (true)
//			{
//				std::getline(_MtlStream, Line);
//				_FileLines.push_back(Line);
//				if (Line.empty())
//					break;
//			}
//
//			std::reverse(std::begin(_FileLines), std::end(_FileLines));
//
//			while (!_FileLines.empty())
//			{
//				// 한줄씩 읽으며 파싱
//				ToKen.clear();
//				std::wstringstream CurLine;
//				CurLine.str(std::move(_FileLines.back()));
//				_FileLines.pop_back();
//
//				CurLine >> ToKen;
//
//				if (ToKen == L"Kd")
//				{
//					CurLine >> _Info.Diffuse.r;
//					CurLine >> _Info.Diffuse.g;
//					CurLine >> _Info.Diffuse.b;
//				}
//				else if (ToKen == L"map_Kd")
//				{
//					CurLine >> _Info.TextureName;
//				}
//				else if (ToKen == L"Ks")
//				{
//					CurLine >> _Info.Specular.r;
//					CurLine >> _Info.Specular.g;
//					CurLine >> _Info.Specular.b;
//				}
//				else if (ToKen == L"Ns")
//				{
//					CurLine >> _Info.Shine;
//				}
//				else if (ToKen == L"illum")
//				{
//					CurLine >> (_uint&)_Info.Illu;
//				}
//
//				if (ToKen.empty())
//				{
//
//					_MtrlInfo.insert
//					({
//						_Info.MtrlName  , _Info
//						});
//				}
//			}
//		}
//	}
//
//	return S_OK;
//}
//
//HRESULT CMapVertex::ReadyComponent(void * pArg)
//{
//	if (FAILED(Super::ReadyComponent(pArg)))
//		return E_FAIL;
//
//	return S_OK;
//}
//
//HRESULT CMapVertex::Render_MapVertex()
//{
//	
//	return S_OK;
//}
//
//void CMapVertex::Free()
//{
//
//
//
//	Super::Free();
//}
