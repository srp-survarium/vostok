////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef MAX_ANGULAR_VELOCITY_COMMAND_H_INCLUDED
#define MAX_ANGULAR_VELOCITY_COMMAND_H_INCLUDED

/* INCLUDES */
class const vostok::console_commands::command_type;
class const vostok::console_commands::execution_filter;
class vostok::console_commands::cc_float;
struct vostok::engine_user::engine;

namespace survarium {

class max_angular_velocity_command : public console_commands::cc_float {
public:
					max_angular_velocity_command	(
						pcstr			name,
						const float		min,
						const float		max,
						bool			serializable,
						const console_commands::command_type	arg_4 /* console_commands::command_type command_type */,
						const console_commands::execution_filter	arg_5 /* console_commands::execution_filter execution_filter */
					);

	inline	void	set_engine						( engine_user::engine& arg_0 ) { /* no source */ }

	virtual	void	execute							( pcstr arg_0 ) override { /* no source */ }

	virtual			~max_angular_velocity_command	( ) { /* no source */ }

private:
	/* 0x0000 */	/* console_commands::cc_float */
	/* 0x0050 */	engine_user::engine*	m_engine;
	/* 0x0054 */	float					m_value;
}; // class max_angular_velocity_command

STATIC_SIZE_ASSERT(max_angular_velocity_command, 0x58);

} // namespace survarium

#endif // #ifndef MAX_ANGULAR_VELOCITY_COMMAND_H_INCLUDED
