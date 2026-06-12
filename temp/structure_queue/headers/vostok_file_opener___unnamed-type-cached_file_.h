////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FILE_OPENER__H_INCLUDED
#define VOSTOK_FILE_OPENER__H_INCLUDED

namespace survarium {

struct vostok_file_opener::< unnamed-type-cached_file > {

public:
	/* 0x0000 */	void*		raw_data;
	/* 0x0004 */	u32			raw_data_size;
}; // struct vostok_file_opener::< unnamed-type-cached_file >

STATIC_SIZE_ASSERT(vostok_file_opener::< unnamed-type-cached_file >, 0x8);

} // namespace survarium

#endif // #ifndef VOSTOK_FILE_OPENER__H_INCLUDED
