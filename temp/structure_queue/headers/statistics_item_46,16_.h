////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATISTICS_ITEM_H_INCLUDED
#define STATISTICS_ITEM_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<32>;
class vostok::fixed_vector<vostok::fixed_string<46>,16>;

namespace survarium {

struct statistics_item< 46, 16 > {
	inline		statistics_item<46,16>	( ) { /* no source */ }
	inline		~statistics_item<46,16>	( ) { /* no source */ }

public:
	/* 0x0000 */	fixed_string< 32 >		caption;
	/* 0x002c */	fixed_vector< fixed_string< 46 >, 16 >	content;
}; // struct statistics_item< 46, 16 >

STATIC_SIZE_ASSERT(statistics_item< 46, 16 >, 0x3F4);

} // namespace survarium

#endif // #ifndef STATISTICS_ITEM_H_INCLUDED
