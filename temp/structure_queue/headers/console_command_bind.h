////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef CONSOLE_COMMAND_BIND_H_INCLUDED
#define CONSOLE_COMMAND_BIND_H_INCLUDED

/* INCLUDES */
class vostok::console_commands::cc_delegate;
class survarium::key_binder;

/* FORWARD REFS */
class vostok::console_commands::save_storage;
class vostok::memory::base_allocator;

namespace survarium {

class console_command_bind : public console_commands::cc_delegate {
public:
					console_command_bind	( key_binder* binder, s32 type );

	virtual	void	save_to					( console_commands::save_storage& f, memory::base_allocator* a ) const override;

	virtual			~console_command_bind	( ) { /* no source */ }

private:
	/* 0x0000 */	/* console_commands::cc_delegate */
	/* 0x0060 */	s32				m_type;
	/* 0x0064 */	key_binder*		m_binder;
}; // class console_command_bind

STATIC_SIZE_ASSERT(console_command_bind, 0x68);

} // namespace survarium

#endif // #ifndef CONSOLE_COMMAND_BIND_H_INCLUDED
