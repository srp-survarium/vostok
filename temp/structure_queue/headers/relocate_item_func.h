////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RELOCATE_ITEM_FUNC_H_INCLUDED
#define RELOCATE_ITEM_FUNC_H_INCLUDED

/* INCLUDES */
class survarium::game;
struct survarium::flash_function_handler;

/* FORWARD REFS */
class survarium::flash_function_handler_params;

namespace survarium {

class relocate_item_func : public flash_function_handler , public boost::noncopyable {
public:
			explicit	relocate_item_func	( game& g );

	virtual	void		call				( flash_function_handler_params& params ) override;

	inline				~relocate_item_func	( ) { /* no source */ }

private:
	/* 0x0000 */	/* flash_function_handler */
	/* 0x0008 */	/* boost::noncopyable */
	/* 0x0008 */	game&		m_game;
}; // class relocate_item_func

STATIC_SIZE_ASSERT(relocate_item_func, 0xC);

} // namespace survarium

#endif // #ifndef RELOCATE_ITEM_FUNC_H_INCLUDED
