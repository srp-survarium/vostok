#ifndef VOSTOK_RENDER_BASE_UI_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_BASE_UI_RENDERER_H_INCLUDED

#include "engine_wrapper.h"

namespace vostok {
namespace render {
namespace ui {

struct vertex
{
	inline vertex	( ) { }

	inline vertex	( float const x, float const y, float const z, u32 const color, float const u, float const v ) :
		m_position	( x, y, z, 0.f ),
		m_color		( color ),
		m_uv		( u, v )
	{
	}

	float4			m_position;
	u32				m_color;
	float2			m_uv;
};

struct shader {
};

struct VOSTOK_NOVTABLE command : public engine::command
{
//.	virtual void CreateUIGeom() = 0;
//.	virtual void DestroyUIGeom() = 0;

	virtual void set_shader(shader& shader) = 0;
//.	virtual void SetScissor(Irect* rect=NULL) = 0;
//.	virtual void GetActiveTextureResolution(Fvector2 &res) = 0;

	virtual void push_point(float x, float y, float z, u32 c, float u, float v) = 0;

//.	virtual void	CacheSetXformWorld	(const Fmatrix& M) = 0;
//.	virtual void	CacheSetCullMode	(CullMode) = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( command )
};

struct VOSTOK_NOVTABLE renderer 
{
	virtual	command* create_command	(u32 max_verts, int prim_type, int point_type) = 0;
	virtual	command* create_command	(vertex* begin, vertex* end, int prim_type, int point_type) = 0;
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( renderer )
};

} // namespace ui
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_BASE_UI_RENDERER_H_INCLUDED