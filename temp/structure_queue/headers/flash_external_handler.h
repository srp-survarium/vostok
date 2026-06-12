////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_EXTERNAL_HANDLER_H_INCLUDED
#define FLASH_EXTERNAL_HANDLER_H_INCLUDED

/* INCLUDES */
struct survarium::flash_external_handler_impl;

/* FORWARD REFS */
class survarium::flash_movie;
class survarium::flash_value;

namespace survarium {

struct flash_external_handler {
	inline			flash_external_handler	( ) { /* no source */ }
	virtual			~flash_external_handler	( ) { /* no source */ }

	virtual	void	callback				(
						flash_movie*			arg_0,
						pcstr					arg_1,
						flash_value const*		arg_2,
						u32						arg_3
					) = 0;

public:
	/* 0x0004 */	flash_external_handler_impl*	impl;
}; // struct flash_external_handler

STATIC_SIZE_ASSERT(flash_external_handler, 0x8);

} // namespace survarium

#endif // #ifndef FLASH_EXTERNAL_HANDLER_H_INCLUDED
