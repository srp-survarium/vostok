////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_INLINE_H_INCLUDED
#define SCHEDULER_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// survarium::scheduler::record& survarium::scheduler::register_object(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, const bool)
scheduler::record& scheduler::register_object( scheduler::identifier* identifier, boost::function<void __cdecl(u32,u32)> const& callback, bool active )
{
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// 17
	// <0x92cf9>|0x000|0x000:'60'
	// 1
	// 2
	// <0x92d0e>|0x015|0x015:'63'
	// 1
	// <0x92d7c>|0x083|0x06e:'65'
	// 1
	// <0x92d7f>|0x086|0x003:'67'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void survarium::scheduler::register_for_update(survarium::scheduler::identifier*, boost::function<void __cdecl(unsigned int,unsigned int)> const&, const bool, const unsigned int, const unsigned int, const unsigned int)
void scheduler::register_for_update(
	scheduler::identifier*				identifier,
	boost::function<void __cdecl(u32,u32)> const&	callback,
	bool								active,
	u32									update_delta,
	u32									max_update_count,
	u32									time_start_from)
{
	// FUNCTION BODY
	// <0x92da0>|0x000|0x000:'88'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x92db6>|0x016|0x016:'94'
	// <0x92dba>|0x01a|0x004:'95'
	// ******
}

// STATE[STUB]
// void survarium::scheduler::unregister(survarium::scheduler::identifier*)
void scheduler::unregister( scheduler::identifier* identifier )
{
	// FUNCTION BODY
	// <0x92de0>|0x000|0x000:'100'
	// <0x92df7>|0x017|0x017:'101'
	// 1
	// <0x92dfb>|0x01b|0x004:'103'
	// 1
	// 2
	// <0x92e06>|0x026|0x00b:'106'
	// <0x92e3d>|0x05d|0x037:'107'
	// <0x92e55>|0x075|0x018:'108'
	// 1
	// <0x92e83>|0x0a3|0x02e:'110'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		char[32]
		account_name_type;

	typedef
		inventory_item_instance*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data const*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data*
		iterator_type;

} // namespace survarium

#endif // #ifndef SCHEDULER_INLINE_H_INCLUDED
