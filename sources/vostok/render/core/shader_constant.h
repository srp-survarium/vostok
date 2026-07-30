#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_H_INCLUDED

#include <new>
#include <vostok/render/core/shader_constant_host.h>
#include <vostok/render/core/shader_constant_slot.h>
#include <vostok/render/core/shader_constant_source.h>

namespace vostok {
namespace render {

class shader_constant {
public:
	explicit shader_constant( shader_constant_host const& host ) :
		m_source( 0, 0 ),
		m_host( host )
	{
	}

	shader_constant( shader_constant const& other ) :
		m_slot( other.slot( ) ),
		m_source( other.source( ) ),
		m_host( other.host( ) )
	{
	}

	shader_constant& operator=( shader_constant const& other )
	{
		this->~shader_constant( );
		new (this) shader_constant( other );
		return *this;
	}

	void set_source( shader_constant_source const& source )
	{
		m_source = source;
	}

	shader_constant_slot const& slot( ) const { return m_slot; }
	shader_constant_source const& source( ) const { return m_source; }
	shader_constant_host const& host( ) const { return m_host; }
	shader_constant_host& get_host( ) const
	{
		return const_cast<shader_constant_host&>( m_host );
	}

	shader_constant_slot m_slot;
	shader_constant_source m_source;
	shader_constant_host const& m_host;
};

inline bool operator==( shader_constant const& left, shader_constant const& right )
{
	return left.host( ) == right.host( )
		&& left.source( ) == right.source( )
		&& left.slot( ) == right.slot( );
}

inline bool operator!=( shader_constant const& left, shader_constant const& right )
{
	return !(left == right);
}

inline s32 compare( shader_constant const&, shader_constant const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x12ea00]
	return 0;
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_H_INCLUDED
