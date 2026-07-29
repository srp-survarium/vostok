////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_CC_FLOAT_H_INCLUDED
#define RENDER_RENDER_CC_FLOAT_H_INCLUDED

/* INCLUDES */
class const vostok::console_commands::command_type;
class vostok::console_commands::cc_float;
class vostok::render::render_cc;
class vostok::render::enum_options_changes_result;

/* FORWARD REFS */
class vostok::render::shader_macro;

namespace vostok {
namespace render {

class render_cc_float : public render_cc , public console_commands::cc_float {
public:
					render_cc_float	(
						pcstr							name,
						enum_options_changes_result		changed_result,
						pcstr							define_name,
						float&							value,
						float&							prev_value,
						const float						min,
						const float						max,
						bool							serializable,
						const console_commands::command_type	arg_8 /* console_commands::command_type command_type */
					);

	virtual	void	execute			( pcstr args ) override;

	virtual	bool	is_changed		( ) const override;

	virtual	bool	fill_macro		( shader_macro& out_macro ) const override;

	virtual			~render_cc_float( ) { /* no source */ }

	/* 0x0000 */	/* render_cc */
	/* 0x0010 */	/* console_commands::cc_float */
	/* 0x0060 */	float&		m_prev_value;
}; // class render_cc_float

STATIC_SIZE_ASSERT(render_cc_float, 0x68);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_CC_FLOAT_H_INCLUDED
