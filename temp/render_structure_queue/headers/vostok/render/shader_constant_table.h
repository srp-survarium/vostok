////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_TABLE_H_INCLUDED
#define RENDER_SHADER_CONSTANT_TABLE_H_INCLUDED

/* INCLUDES */
class vostok::render::resource_intrusive_base;
class vostok::render::vector<vostok::render::shader_constant>;
class vostok::render::enum_shader_type;
typedef vostok::render::vector<vostok::intrusive_ptr<vostok::render::shader_constant_buffer,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy> >
	vostok::render::vector<vostok::render::shader_constant_buffer_ptr >;
class vostok::render::shader_constant_buffer;

/* FORWARD REFS */
class vostok::render::shader_constant;
class vostok::render::shader_constant_bindings;
class vostok::shared_string;

namespace vostok {
namespace render {

class shader_constant_table : public resource_intrusive_base {
public:
	inline								shader_constant_table	( ) { /* no source */ }
	inline	explicit					shader_constant_table	( shader_constant_table const& other ) { /* no source */ }

	inline	void						mark_registered			( ) { /* no source */ }

			void						destroy_impl			( ) const;

			void						clear					( );

			bool						parse					( ID3D11ShaderReflection* shader_reflection, enum_shader_type destination );

			void						apply_bindings			( shader_constant_bindings const& bindings );

	inline	shader_constant const*		get						( pcstr const arg_0 ) const { /* no source */ }
			shader_constant*			get						( shared_string const& name );
private:
			shader_constant*			get						( pcstr const name );

public:
	inline	bool						equal					( shader_constant_table const& arg_0 ) const { /* no source */ }

			s32							compare					( shader_constant_table const& other ) const;

	inline	bool						empty					( ) const { /* no source */ }

			bool						parse_constant_buffer	( ID3D11ShaderReflectionConstantBuffer* src_table, u32 buffer_index );

	inline	bool						is_registered			( ) const { /* no source */ }

private:
	inline	void						fatal					( pcstr arg_0 ) { /* no source */ }

public:
	inline								~shader_constant_table	( ) { /* no source */ }

	/* 0x0000 */	/* resource_intrusive_base */
	/* 0x0004 */	vector< shader_constant >		m_table;
	/* 0x0010 */	vector< shader_constant_buffer_ptr >	m_const_buffers;
	/* 0x001c */	bool							m_is_registered;
}; // class shader_constant_table

STATIC_SIZE_ASSERT(shader_constant_table, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_TABLE_H_INCLUDED
