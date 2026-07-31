#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_TABLE_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_TABLE_H_INCLUDED

#include <vostok/intrusive_ptr.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/render_include.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/core/shader_constant.h>
#include <vostok/render/core/shader_constant_buffer.h>
#include <vostok/render/core/shader_defines.h>

namespace vostok {

class shared_string;

namespace render {

class shader_constant_bindings;

class shader_constant_table : public resource_intrusive_base {
public:
	typedef vector<shader_constant> constants_type;
	typedef vector<shader_constant_buffer_ptr> buffers_type;

	shader_constant_table( ) :
		m_is_registered( false )
	{
	}

	explicit shader_constant_table( shader_constant_table const& other ) :
		m_table( other.m_table ),
		m_const_buffers( other.m_const_buffers ),
		m_is_registered( false )
	{
		// FUNCTION BODY[0x124850]
	}

	void mark_registered( )
	{
		R_ASSERT( !m_is_registered );
		m_is_registered = true;
	}

	void destroy_impl( ) const;
	void clear( );
	bool parse( ID3D11ShaderReflection* reflection, enum_shader_type destination );
	void apply_bindings( shader_constant_bindings const& bindings );

	shader_constant const* get( pcstr name ) const
	{
		return const_cast<shader_constant_table*>( this )->get( name );
	}

	shader_constant* get( shared_string const& name );

	bool equal( shader_constant_table const& other ) const
	{
		if ( m_table.size( ) != other.m_table.size( ) )
			return false;

		u32 size = m_table.size( );
		for ( u32 it = 0; it < size; ++it ) {
			if ( m_table[it] != other.m_table[it] )
				return false;
		}

		return true;
	}

	s32 compare( shader_constant_table const& other ) const;
	bool empty( ) const { return m_table.empty( ); }
	bool parse_constant_buffer(
		ID3D11ShaderReflectionConstantBuffer* source,
		u32 buffer_index
	);
	bool is_registered( ) const { return m_is_registered; }
	~shader_constant_table( ) { }

private:
	shader_constant* get( pcstr name );
	void fatal( pcstr msg )
	{
		VOSTOK_UNREFERENCED_PARAMETER( msg );
		FATAL( msg );
	}

public:
	constants_type m_table;
	buffers_type m_const_buffers;
	bool m_is_registered;
};

typedef intrusive_ptr<
	shader_constant_table,
	resource_intrusive_base,
	threading::single_threading_policy
> shader_constant_table_ptr;

typedef intrusive_ptr<
	shader_constant_table const,
	resource_intrusive_base const,
	threading::single_threading_policy
> shader_constant_table_const_ptr;

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_TABLE_H_INCLUDED
