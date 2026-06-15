////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef D3D1X_SHADER_H_INCLUDED
#define D3D1X_SHADER_H_INCLUDED

// survarium's fork of the Scaleform 4.x D3D1x_Shader.h (merged with the
// ShaderManager) - the shipped binary tracks a NEWER SDK than the vendored
// 4.0.15 (StaticShaderManager/ShaderInterfaceBase bases, generated shader
// descs); skeleton rebuilt from the canonical PDB type dumps.

#include "Render/Render_Shader.h"
#include "d3d1x_shaderdescs.h"

#include <d3d11.h>

namespace Scaleform {
namespace Render {
namespace D3D1x {

class HAL;
class Texture;

struct VertexShader {
	// STATE[STUB]
	inline			VertexShader	( )
	{
		// FUNCTION BODY[0x012ae0] (ICF-folded with an unrelated vostok ctor)
	}
	inline			~VertexShader	( ) { /* no source */ }

	// Init/Shutdown are out-of-line in d3d1x_shader.cpp per the rich index
			bool	Init			( ID3D11Device* arg_0, VertexShaderDesc const* arg_1 );

			void	Shutdown		( );

public:
	/* 0x0000 */	VertexShaderDesc const*		pDesc;
	/* 0x0004 */	Ptr< ID3D11VertexShader >	pProg;
	/* 0x0008 */	s32							UniformOffset[15];
}; // struct VertexShader

struct FragShader {
	// STATE[STUB]
	inline			FragShader	( )
	{
		// FUNCTION BODY[0x0b36f0]
	}
	inline			~FragShader	( ) { /* no source */ }

	// Init/Shutdown are out-of-line in d3d1x_shader.cpp per the rich index
			bool	Init		( ID3D11Device* arg_0, FragShaderDesc const* arg_1 );

			void	Shutdown	( );

public:
	/* 0x0000 */	FragShaderDesc const*		pDesc;
	/* 0x0004 */	Ptr< ID3D11PixelShader >	pProg;
	/* 0x0008 */	u32							Offset;
	/* 0x000c */	s32							UniformOffset[15];
}; // struct FragShader

struct ShaderPair {
	inline						ShaderPair		( ) { /* no source */ }

	inline	ShaderPair const*	operator ->		( ) const { /* no source */ return this; }

	inline	operator bool						( ) const { /* no source */ return false; }

public:
	/* 0x0000 */	VertexShader const*			pVS;
	/* 0x0004 */	VertexShaderDesc const*		pVDesc;
	/* 0x0008 */	FragShader const*			pFS;
	/* 0x000c */	FragShaderDesc const*		pFDesc;
	/* 0x0010 */	VertexFormat const*			pVFormat;
}; // struct ShaderPair

class SysVertexFormat : public Render::SystemVertexFormat {
public:
	// out-of-line in d3d1x_shader.cpp per the rich index
				SysVertexFormat	(
					ID3D11Device*				arg_0,
					VertexFormat const*			arg_1,
					VertexShaderDesc const*		arg_2
				);
	virtual		~SysVertexFormat( ) { /* no source */ }

private:
	/* 0x0000 */	/* Scaleform::Render::SystemVertexFormat */
	/* 0x0008 */	Ptr< ID3D11InputLayout >	pVDecl;
	/* 0x000c */	D3D11_INPUT_ELEMENT_DESC	VertexElements[8];
	/* 0x00ec */	u32							VertexElementCount;
}; // class SysVertexFormat

struct ShaderConstantRange {
	inline			ShaderConstantRange	( HAL* arg_0, float* arg_1 ) { /* no source */ }

	// out-of-line in d3d1x_shader.cpp per the rich index
			void	Update				( s32 arg_0, s32 arg_1, s32 arg_2 );

	inline	void	Finish				( bool arg_0 ) { /* no source */ }

public:
	/* 0x0000 */	float*						UniformData;
	/* 0x0004 */	D3D11_MAPPED_SUBRESOURCE	MappedBuffer;
	/* 0x0010 */	ID3D11Buffer*				pConstantBuffer;
	/* 0x0014 */	HAL*						pHal;
}; // struct ShaderConstantRange

class ShaderInterface : public Render::ShaderInterfaceBase< Uniform, ShaderPair > {
public:
	inline	HAL*				GetHAL				( ) const { /* no source */ return pHal; }

	// STATE[STUB]
	inline	explicit			ShaderInterface		( Render::HAL* arg_0 )
	{
		// FUNCTION BODY[0x0aecf0]
		VOSTOK_UNREFERENCED_PARAMETER	( arg_0 );
	}

	// BeginScene/SetStaticShader/SetTexture/Finish are out-of-line in
	// d3d1x_shader.cpp per the rich index
			void				BeginScene			( );

	inline	ShaderPair const&	GetCurrentShaders	( ) const { /* no source */ return CurShaders; }

			bool				SetStaticShader		( ShaderDesc::ShaderType arg_0, VertexFormat const* arg_1 );

			void				SetTexture			(
									const ShaderPair	arg_0,
									u32					arg_1,
									Render::Texture*	arg_2,
									ImageFillMode		arg_3,
									u32					arg_4
								);

			void				Finish				( u32 arg_0 );

private:
	/* 0x0000 */	/* Scaleform::Render::ShaderInterfaceBase< Uniform, ShaderPair > */
	/* 0x1120 */	HAL*					pHal;
	/* 0x1124 */	ShaderPair				CurShaders;
	/* 0x1138 */	ID3D11VertexShader*		pLastVS;
	/* 0x113c */	ID3D11PixelShader*		pLastFS;
	/* 0x1140 */	ID3D11InputLayout*		pLastDecl;
}; // class ShaderInterface

class ShaderManager : public Render::StaticShaderManager< ShaderDesc, VertexShaderDesc, Uniform, ShaderInterface, Texture > {
public:
	// everything unmarked is out-of-line in d3d1x_shader.cpp per the rich index
			explicit		ShaderManager		( ProfileViews* arg_0 );

			void			MapVertexFormat		(
								PrimitiveFillType		arg_0,
								VertexFormat const*		arg_1,
								VertexFormat const**	arg_2,
								VertexFormat const**	arg_3,
								VertexFormat const**	arg_4
							);

			bool			HasInstancingSupport( ) const;

			bool			Initialize			( HAL* arg_0 );

			void			Reset				( );

			void			BeginScene			( );

			void			EndScene			( );

	inline	ShaderDesc::ShaderVersion	GetShaderVersion	( ) const { /* no source */ return ShaderModel; }

	// the dtor body lives in d3d1x_hal.h per the rich index (see there)
							~ShaderManager		( );

	static	inline	u32		GetDrawableImageFlags( ) { /* no source */ return 0; }

private:
	/* 0x0000 */	/* Scaleform::Render::StaticShaderManager< ... > */
	/* 0x0018 */	FragShader					StaticFShaders[598];
	/* 0xa848 */	VertexShader				StaticVShaders[232];
	/* 0xe5e8 */	Ptr< ID3D11Device >			pDevice;
	/* 0xe5ec */	ShaderDesc::ShaderVersion	ShaderModel;
}; // class ShaderManager

} // namespace D3D1x
} // namespace Render
} // namespace Scaleform

STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::VertexShader, 0x44);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::FragShader, 0x48);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::ShaderPair, 0x14);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::SysVertexFormat, 0xF0);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::ShaderConstantRange, 0x18);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::ShaderInterface, 0x1144);
STATIC_SIZE_ASSERT(Scaleform::Render::D3D1x::ShaderManager, 0xE5F0);

#endif // #ifndef D3D1X_SHADER_H_INCLUDED
