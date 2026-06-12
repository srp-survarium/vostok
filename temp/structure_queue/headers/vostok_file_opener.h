////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_FILE_OPENER_H_INCLUDED
#define VOSTOK_FILE_OPENER_H_INCLUDED

/* INCLUDES */
class Scaleform::GFx::FileOpener;
struct survarium::vostok_file_opener::<unnamed-type-cached_file>;

namespace survarium {

class vostok_file_opener : public Scaleform::GFx::FileOpener {
public:
	inline						vostok_file_opener	( ) { /* no source */ }

	virtual	Scaleform::File*	OpenFile			( pcstr arg_0, s32 arg_1, s32 arg_2 ) override { /* no source */ }

	virtual						~vostok_file_opener	( ) { /* no source */ }

private:
	/* 0x0000 */	/* Scaleform::GFx::FileOpener */
	/* 0x000c */	vostok_file_opener::< unnamed-type-cached_file >	cached_file;
}; // class vostok_file_opener

STATIC_SIZE_ASSERT(vostok_file_opener, 0x14);

} // namespace survarium

#endif // #ifndef VOSTOK_FILE_OPENER_H_INCLUDED
