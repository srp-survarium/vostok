////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ACCOUNT_LIST_ITEM_H_INCLUDED
#define ACCOUNT_LIST_ITEM_H_INCLUDED

namespace survarium {

struct account_list_item {
public:
	/* 0x0000 */	u32						account_id;
	/* 0x0004 */	fixed_string< 32 >		account_name;
	/* 0x0030 */	bool					online;

	inline	bool	operator==			( const u32 arg_0 ) { /* no source */ return account_id == arg_0; }
}; // struct account_list_item

STATIC_SIZE_ASSERT(account_list_item, 0x34);

} // namespace survarium

#endif // #ifndef ACCOUNT_LIST_ITEM_H_INCLUDED
