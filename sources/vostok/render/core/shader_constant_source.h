// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_SOURCE_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_SOURCE_H_INCLUDED
#include <new>

namespace vostok {
namespace render {

class shader_constant_source {
public:
	shader_constant_source( void* pointer, u32 size ) :
		m_pointer( pointer ),
		m_size( size )
	{
	}

	shader_constant_source( shader_constant_source const& other ) :
		m_pointer( other.pointer( ) ),
		m_size( other.size( ) )
	{
	}

	shader_constant_source& operator=( shader_constant_source const& other )
	{
		this->~shader_constant_source( );
		new (this) shader_constant_source( other );
		return *this;
	}

	void* pointer( ) const { return m_pointer; }
	u32 size( ) const { return m_size; }

private:
	void* const m_pointer;
	u32 const m_size;
};

inline bool operator==(
	shader_constant_source const& left,
	shader_constant_source const& right
)
{
	return left.pointer( ) == right.pointer( ) && left.size( ) == right.size( );
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_SOURCE_H_INCLUDED
