////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OPTIONS_NAME_TO_LABEL_H_INCLUDED
#define OPTIONS_NAME_TO_LABEL_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<32>;

namespace survarium {

struct options_name_to_label {
	inline		options_name_to_label	( ) { /* no source */ }

public:
	/* 0x0000 */	fixed_string< 32 >		name;
	/* 0x002c */	fixed_string< 32 >		label;
}; // struct options_name_to_label

STATIC_SIZE_ASSERT(options_name_to_label, 0x58);

} // namespace survarium

#endif // #ifndef OPTIONS_NAME_TO_LABEL_H_INCLUDED
