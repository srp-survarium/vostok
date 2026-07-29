////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_FUNCTOR_COMMAND_WITH_NOTIFY_H_INCLUDED
#define RENDER_FUNCTOR_COMMAND_WITH_NOTIFY_H_INCLUDED

/* INCLUDES */
class boost::function<void __cdecl(void)>;
class vostok::render::functor_command;

namespace vostok {
namespace render {

class functor_command_with_notify : public functor_command {
public:
	virtual		~functor_command_with_notify( );
	inline		functor_command_with_notify	( boost::function< void() > const& arg_0, boost::function< void() > const& arg_1 ) { /* no source */ }

	/* 0x0000 */	/* functor_command */
private:
	/* 0x0098 */	boost::function< void() >		m_on_destroy;
}; // class functor_command_with_notify

STATIC_SIZE_ASSERT(functor_command_with_notify, 0xB8);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_FUNCTOR_COMMAND_WITH_NOTIFY_H_INCLUDED
