#ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_BINDINGS_H_INCLUDED
#define VOSTOK_RENDER_CORE_SHADER_CONSTANT_BINDINGS_H_INCLUDED

#include <algorithm>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/shader_constant_binding.h>

namespace vostok {
namespace render {

class shader_constant_bindings : private boost::noncopyable {
public:
	typedef vector<shader_constant_binding> bindings_type;

	void add( shader_constant_binding const& binding )
	{
		bindings_type::iterator const end = m_bindings.end( );
		bindings_type::iterator const found = std::find(
			m_bindings.begin( ),
			end,
			binding
		);
		if ( found == end )
			m_bindings.push_back( binding );
	}

	void clear( ) { m_bindings.clear( ); }
	bindings_type const& bindings( ) const { return m_bindings; }

private:
	bindings_type m_bindings;
};

STATIC_SIZE_ASSERT( shader_constant_bindings, 0xC );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SHADER_CONSTANT_BINDINGS_H_INCLUDED
