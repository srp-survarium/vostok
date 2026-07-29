////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_HOST_H_INCLUDED
#define RENDER_SHADER_CONSTANT_HOST_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_slot;
class vostok::render::shader_constant_source;
class vostok::shared_string;
class vostok::render::enum_constant_type;

/* FORWARD REFS */
class vostok::math::int2;

namespace vostok {
namespace render {

enum vostok::render::enum_constant_type
{
	rc_float		= 0x0000,
	rc_int			= 0x0001,
	rc_bool			= 0x0002,
	rc_INVALID		= 0xffff,
};

class shader_constant_host {
public:
	inline										shader_constant_host	( shared_string const& name, enum_constant_type type ) { /* no source */ }

	inline	void								set						( float arg_0 ) { /* no source */ }
	inline	void								set						( float2 const& arg_0 ) { /* no source */ }
	inline	void								set						( float3 const& arg_0 ) { /* no source */ }
	inline	void								set						( float4 const& arg_0 ) { /* no source */ }
	inline	void								set						( float4x4 const& arg_0 ) { /* no source */ }
	inline	void								set						( s32 arg_0 ) { /* no source */ }
	inline	void								set						( math::int2 const& arg_0 ) { /* no source */ }

	inline	void								reset_update_markers	( ) { /* no source */ }

	inline	shader_constant_source const&		source					( ) const { /* no source */ }

	inline	shader_constant_slot const&			shader_slot				( u32 arg_0 ) const { /* no source */ }
	inline	shader_constant_slot&				shader_slot				( u32 arg_0 ) { /* no source */ }

	inline	shared_string const&				name					( ) const { /* no source */ }

	inline	enum_constant_type					type					( ) const { /* no source */ }

	inline										~shader_constant_host	( ) { /* no source */ }

	/* 0x0000 */	shader_constant_source		m_source;
	/* 0x0008 */	shader_constant_slot		m_shader_slots[3];
	/* 0x0020 */	shared_string				m_name;
	/* 0x0024 */	u32							m_update_markers[3];
	/* 0x0030 */	enum_constant_type			m_type;
}; // class shader_constant_host

STATIC_SIZE_ASSERT(shader_constant_host, 0x38);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_HOST_H_INCLUDED
