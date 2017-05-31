#pragma once
#include <d3d11.h>
#include <fstream>
#include <stdint.h>

using namespace std;

namespace sky
{
	class IncludeFX : public ID3DInclude
	{
	public:
		IncludeFX(const char* shaderDir)
			:m_ShaderDir(shaderDir){}

		virtual ~IncludeFX() {}
		STDMETHOD(Open)(THIS_ D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			try
			{
				string finalPath = "";
				finalPath.append(m_ShaderDir);
				finalPath.append(pFileName);

				ifstream fileStream(finalPath, std::ifstream::ate | std::ifstream::binary);
				streampos fileSize = fileStream.tellg();

				if (fileSize)
				{
					char* buf = new char[fileSize];
					fileStream.read(buf, fileSize);

					*ppData = buf;
					*pBytes = static_cast<UINT>( fileSize.seekpos());
				}
				else
				{
					*ppData = nullptr;
					*pBytes = 0;
				}
				return S_OK;
			}
			catch (int err)
			{ 
				err = 1;
				return E_FAIL;
			}
		}
		STDMETHOD(Close)(THIS_ LPCVOID pData)
		{
			char* buf = (char*)pData;
			if (buf)
			{
				delete[] buf;
				buf = nullptr;
			}

			return S_OK;
		}
	private:
		string m_ShaderDir;
	};
}