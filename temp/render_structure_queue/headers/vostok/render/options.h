////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_OPTIONS_H_INCLUDED
#define RENDER_OPTIONS_H_INCLUDED

/* INCLUDES */
class vostok::console_commands::console_command;
class vostok::quasi_singleton<vostok::render::options>;
class vostok::render::render_cc;
struct vostok::render::options::optinos_table;
class vostok::fs_new::virtual_path_string;
class vostok::render::enum_options_changes_result;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::fixed_vector<vostok::render::shader_macro,128>;
class vostok::memory::reader;
class vostok::render::vector<vostok::fs_new::virtual_path_string>;
class vostok::resources::queries_result;

namespace vostok {
namespace render {

class options : public quasi_singleton< options > {
public:
											options							( );

			void							set_default_values				( );

			void							register_console_commands		( );

	inline	void							save							( pcstr arg_0 ) { /* no source */ }

	inline	void							load							( pcstr arg_0 ) { /* no source */ }

			void							begin_render_options_changing	( long volatile* waiting_for );
			enum_options_changes_result		end_render_options_changing		( vector< fs_new::virtual_path_string >& out_changed_defines );

			void							load_from_config				( configs::binary_config_value const& config );

	inline	fs_new::virtual_path_string		get_current_configuration		( ) { /* no source */ }
	inline	void							save_current_configuration		( ) { /* no source */ }

			void							fill_global_macros				( fixed_vector< shader_macro, 128 >& out_defines );

private:
			void							load_impl						( memory::reader& F );

			void							on_config_loaded				( resources::queries_result& data );
	inline	void							on_config_loaded2				( resources::queries_result& arg_0 ) { /* no source */ }

public:
	inline									~options						( ) { /* no source */ }

	/* 0x0000 */	/* quasi_singleton< options > */
private:
	/* 0x0000 */	render_cc*								first_render_command;
public:
	/* 0x0004 */	console_commands::console_command*		first_command;
	/* 0x0008 */	console_commands::console_command*		last_command;
	/* 0x000c */	options::optinos_table					current;
	/* 0x0134 */	options::optinos_table					previous;
}; // class options

STATIC_SIZE_ASSERT(options, 0x25C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_OPTIONS_H_INCLUDED
