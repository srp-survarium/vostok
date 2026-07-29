////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FUNCTOR_COMMAND_H_INCLUDED
#define RENDER_FUNCTOR_COMMAND_H_INCLUDED

/* INCLUDES */
class boost::function<void __cdecl(void)>;
class boost::function<void __cdecl(vostok::render::base_command &)>;
class vostok::render::base_command;

namespace vostok {
namespace render {

class functor_command : public base_command , public boost::noncopyable {
public:
					functor_command	(
						boost::function< void() > const&	on_execute,
						boost::function< void( base_command& ) > const&	on_defer_execution
					);

	virtual	void	execute			( ) override;

	virtual	void	defer_execution	( ) override;

	virtual			~functor_command( ) { /* no source */ }

	/* 0x0000 */	/* base_command */
	/* 0x0054 */	/* boost::noncopyable */
private:
	/* 0x0058 */	boost::function< void() >		m_on_execute;
	/* 0x0078 */	boost::function< void( base_command& ) >	m_on_defer_execution;
}; // class functor_command

STATIC_SIZE_ASSERT(functor_command, 0x98);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FUNCTOR_COMMAND_H_INCLUDED
