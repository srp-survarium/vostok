////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_CONSTANT_BINDINGS_H_INCLUDED
#define RENDER_SHADER_CONSTANT_BINDINGS_H_INCLUDED

/* INCLUDES */
class vostok::render::vector<vostok::render::shader_constant_binding>;

/* FORWARD REFS */
class vostok::render::shader_constant_binding;

namespace vostok {
namespace render {

class shader_constant_bindings : public boost::noncopyable {
public:
	inline	void	add							( shader_constant_binding const& arg_0 ) { /* no source */ }

	inline	void	clear						( ) { /* no source */ }

	inline	vector< shader_constant_binding > const&	bindings					( ) const { /* no source */ }

	inline			shader_constant_bindings	( ) { /* no source */ }
	inline			~shader_constant_bindings	( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
private:
	/* 0x0000 */	vector< shader_constant_binding >		m_bindings;
}; // class shader_constant_bindings

STATIC_SIZE_ASSERT(shader_constant_bindings, 0xC);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_CONSTANT_BINDINGS_H_INCLUDED
