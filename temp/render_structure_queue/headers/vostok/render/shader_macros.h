////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SHADER_MACROS_H_INCLUDED
#define RENDER_SHADER_MACROS_H_INCLUDED

/* INCLUDES */
class vostok::fixed_vector<char const *,128>;
class vostok::fixed_vector<vostok::render::shader_macro,128>;
class vostok::quasi_singleton<vostok::render::shader_macros>;
class vostok::render::shader_configuration;

/* FORWARD REFS */
class vostok::fixed_vector<vostok::fs_new::virtual_path_string,128>;

namespace vostok {
namespace render {

class shader_macros : public quasi_singleton< shader_macros > {
public:
					shader_macros					( );

			void	fill_shader_macro_list			( fixed_vector< shader_macro, 128 >& macros, shader_configuration shader_config );

	inline	void	merge_with_declared_macroses	(
						fixed_vector< fs_new::virtual_path_string, 128 > const&	arg_0,
						fixed_vector< shader_macro, 128 >&		arg_1
					) { /* no source */ }

	inline	void	append_macros_name				( fixed_vector< pcstr, 128 >& arg_0, pcstr arg_1 ) { /* no source */ }

private:
			void	register_available_macros		( );
			void	fill_global_macros				( fixed_vector< shader_macro, 128 >& defines );
			void	fill_shader_configuration_macros( fixed_vector< shader_macro, 128 >& macros, shader_configuration shader_config );
public:
	inline			~shader_macros					( ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< shader_macros > */
private:
	/* 0x0000 */	fixed_vector< pcstr, 128 >				m_available_macros;
	/* 0x0208 */	fixed_vector< shader_macro, 128 >		m_working_macro_list;
}; // class shader_macros

STATIC_SIZE_ASSERT(shader_macros, 0x11210);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SHADER_MACROS_H_INCLUDED
