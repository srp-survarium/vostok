////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STATISTICS_ITEM_H_INCLUDED
#define STATISTICS_ITEM_H_INCLUDED

namespace survarium {

// the canonical dump prints only the statistics_item<46,16> instantiation;
// the primary template mirrors the same-shaped vostok::ai::statistics_item
// (ai/npc_statistics.h) - the survarium namespace carries its own copy
template < int ItemSize, int ItemsCount >
struct statistics_item {
	typedef fixed_string< ItemSize >			content_type;

	inline		statistics_item		( ) { /* no source */ }
	inline		~statistics_item	( ) { /* no source */ }

public:
	/* 0x0000 */	fixed_string< 32 >							caption;
	/* 0x002c */	fixed_vector< content_type, ItemsCount >	content;
}; // struct statistics_item

// the comma inside the template-id breaks the assert macro - alias it first
namespace { typedef statistics_item< 46, 16 > statistics_item_46_16; }
STATIC_SIZE_ASSERT(statistics_item_46_16, 0x3F4);

} // namespace survarium

#endif // #ifndef STATISTICS_ITEM_H_INCLUDED
