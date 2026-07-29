////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_BUFFER_H_INCLUDED
#define RENDER_SHADER_CONSTANT_BUFFER_H_INCLUDED

/* INCLUDES */
struct ID3D11Buffer;
class vostok::fixed_string<64>;
class vostok::render::resource_intrusive_base;
class vostok::render::enum_shader_type;

/* FORWARD REFS */
class vostok::render::shader_constant;
class vostok::render::shader_constant_slot;

namespace vostok {
namespace render {

enum vostok::render::enum_shader_type
{
	enum_shader_type_vertex			= 0x0,
	enum_shader_type_pixel			= 0x1,
	enum_shader_type_geometry		= 0x2,
	enum_shader_types_count			= 0x3,
};
enum _D3D_CBUFFER_TYPE
{
	D3D_CT_CBUFFER					= 0x0,
	D3D_CT_TBUFFER					= 0x1,
	D3D_CT_INTERFACE_POINTERS		= 0x2,
	D3D_CT_RESOURCE_BIND_INFO		= 0x3,
	D3D10_CT_CBUFFER				= 0x0,
	D3D10_CT_TBUFFER				= 0x1,
	D3D11_CT_CBUFFER				= 0x0,
	D3D11_CT_TBUFFER				= 0x1,
	D3D11_CT_INTERFACE_POINTERS		= 0x2,
	D3D11_CT_RESOURCE_BIND_INFO		= 0x3,
};

class shader_constant_buffer : public resource_intrusive_base {
protected:
											shader_constant_buffer	(
												fixed_string< 64 > const&		name,
												enum_shader_type				dest,
												_D3D_CBUFFER_TYPE				type,
												u32								size
											);
											~shader_constant_buffer	( );

public:
			void							destroy_impl			( ) const;

	inline	void							set						( shader_constant const& arg_0 ) { /* no source */ }
private:
	inline	void							set						( shader_constant_slot const& arg_0, void* arg_1, u32 arg_2 ) { /* no source */ }
	inline	void							set						(
												shader_constant_slot const&		arg_0,
												void*							arg_1,
												u32								arg_2,
												u32								arg_3
											) { /* no source */ }

public:
	inline	void							zero					( shader_constant const& arg_0 ) { /* no source */ }
private:
	inline	void							zero					( shader_constant_slot const& arg_0, u32 arg_1 ) { /* no source */ }

public:
	inline	bool							similiar				( shader_constant_buffer const& arg_0 ) const { /* no source */ }

			void							update					( );

	inline	ID3D11Buffer*					hardware_buffer			( ) const { /* no source */ }

private:
	inline	void							set_memory				( const u32 offset, pcstr src_ptr, const u32 size ) { /* no source */ }
	inline	void							zero_memory				( const u32 arg_0, const u32 arg_1 ) { /* no source */ }

	inline	void*							access					( u32 arg_0 ) { /* no source */ }

public:
	inline	fixed_string< 64 > const&		name					( ) const { /* no source */ }

	inline	enum_shader_type&				dest					( ) { /* no source */ }
	inline	enum_shader_type const&			dest					( ) const { /* no source */ }

	inline	_D3D_CBUFFER_TYPE				type					( ) const { /* no source */ }

	inline	u32								size					( ) const { /* no source */ }

	inline	bool							is_registered			( ) const { /* no source */ }
	inline	void							mark_registered			( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
private:
	/* 0x0004 */	fixed_string< 64 >		m_name;
	/* 0x0050 */	_D3D_CBUFFER_TYPE		m_type;
	/* 0x0054 */	enum_shader_type		m_dest;
	/* 0x0058 */	void*					m_buffer_data;
	/* 0x005c */	u32						m_buffer_size;
	/* 0x0060 */	ID3D11Buffer*			m_hardware_buffer;
	/* 0x0064 */	bool					m_changed;
	/* 0x0065 */	bool					m_is_registered;
}; // class shader_constant_buffer

STATIC_SIZE_ASSERT(shader_constant_buffer, 0x68);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_BUFFER_H_INCLUDED
