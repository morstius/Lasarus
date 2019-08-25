struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;


typedef long HRESULT;
typedef unsigned int UINT;

HRESULT CreateSampler(
	ID3D11Device*,
	ID3D11DeviceContext*,
	ID3D11SamplerState**
);

HRESULT Create2DTexture(
	ID3D11Device*,
	ID3D11Texture2D**,
	ID3D11RenderTargetView**,
	ID3D11ShaderResourceView**,
	const UINT,
	const UINT
);

HRESULT CreateDepthStencilView(
	ID3D11Device*,
	ID3D11DepthStencilView**,
	ID3D11DepthStencilState**,
	ID3D11Texture2D**,
	const UINT,
	const UINT
);