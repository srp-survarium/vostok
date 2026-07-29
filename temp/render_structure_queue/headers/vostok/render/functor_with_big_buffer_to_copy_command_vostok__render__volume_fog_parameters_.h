////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
#define RENDER_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED

/* INCLUDES */
class boost::function<void __cdecl(vostok::render::base_command &)>;
class boost::function<void __cdecl(vostok::render::volume_fog_parameters const &)>;
class vostok::render::base_command;
struct vostok::render::volume_fog_parameters;

namespace vostok {
namespace render {

class functor_with_big_buffer_to_copy_command< volume_fog_parameters > : public base_command , public boost::noncopyable {
public:
	inline			functor_with_big_buffer_to_copy_command<vostok::render::volume_fog_parameters>(
						boost::function< void( volume_fog_parameters const& ) > const&	arg_0,
						volume_fog_parameters const&	arg_1,
						boost::function< void( base_command& ) > const&	arg_2
					) { /* no source */ }

	virtual	void	execute			( ) override { /* no source */ }

	virtual	void	defer_execution	( ) override { /* no source */ }

	virtual			~functor_with_big_buffer_to_copy_command<vostok::render::volume_fog_parameters>( ) { /* no source */ }

	/* 0x0000 */	/* base_command */
	/* 0x0054 */	/* boost::noncopyable */
private:
	/* 0x0058 */	boost::function< void( volume_fog_parameters const& ) >	m_on_execute;
	/* 0x0078 */	boost::function< void( base_command& ) >	m_on_defer_execution;
	/* 0x0098 */	const volume_fog_parameters		m_data;
}; // class functor_with_big_buffer_to_copy_command< volume_fog_parameters >

STATIC_SIZE_ASSERT(functor_with_big_buffer_to_copy_command< volume_fog_parameters >, 0x110);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FUNCTOR_WITH_BIG_BUFFER_TO_COPY_COMMAND_H_INCLUDED
