#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_BINDING_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_BINDING_H_INCLUDED

#include <vostok/render/core/shader_constant_defines.h>
#include <vostok/render/core/shader_constant_source.h>
#include <vostok/shared_string.h>

namespace vostok {
namespace render {

class shader_constant_binding {
public:
	template < typename T >
	shader_constant_binding( shared_string const& name, T const* source ) :
		m_source( const_cast<T*>( source ), constant_type_traits<T>::size ),
		m_name( name ),
		m_type( static_cast<enum_constant_type>( constant_type_traits<T>::type ) ),
		m_class_id( static_cast<enum_constant_class>( constant_type_traits<T>::class_id ) )
	{
		// FUNCTION BODY[0x66d40] for math::float4
		// FUNCTION BODY[0x67540] for float
	}

	shader_constant_binding( shader_constant_binding const& other ) :
		m_source( other.source( ) ),
		m_name( other.name( ) ),
		m_type( other.type( ) ),
		m_class_id( other.class_id( ) )
	{
	}

	shader_constant_binding& operator=( shader_constant_binding const& other )
	{
		this->~shader_constant_binding( );
		new (this) shader_constant_binding( other );
		return *this;
	}

	shared_string const& name( ) const { return m_name; }
	shader_constant_source const& source( ) const { return m_source; }
	enum_constant_class class_id( ) const { return m_class_id; }
	enum_constant_type type( ) const { return m_type; }
	~shader_constant_binding( ) { }

private:
	shader_constant_source m_source;
	shared_string m_name;
	enum_constant_type m_type;
	enum_constant_class m_class_id;
};

inline bool operator==(
	shader_constant_binding const& left,
	shader_constant_binding const& right
)
{
	return left.name( ) == right.name( );
}

STATIC_SIZE_ASSERT( shader_constant_binding, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_BINDING_H_INCLUDED
