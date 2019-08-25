struct ID3D10Blob;

typedef long HRESULT;
typedef wchar_t WCHAR;
typedef const char* LPCSTR;
typedef ID3D10Blob ID3DBlob;

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(
	const WCHAR* szFileName,
	LPCSTR szEntryPoint,
	LPCSTR szShaderModel,
	ID3DBlob** ppBlobOut
);