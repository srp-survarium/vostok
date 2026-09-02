// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_FILE_OPENER_H_INCLUDED
#define VOSTOK_FILE_OPENER_H_INCLUDED

#include "GFx/GFx_Loader.h"		// Scaleform::GFx::FileOpener (base)

namespace survarium {

class flash_factory;

class vostok_file_opener : public Scaleform::GFx::FileOpener {
public:
	inline						vostok_file_opener	( )
	{
		cached_file.raw_data		= NULL;
		cached_file.raw_data_size	= 0;
	}

private:
	friend class flash_factory;

	/* 0x0000 */	/* Scaleform::GFx::FileOpener */

public:
	// PDB spells the nested type `<unnamed-type-cached_file>` - a true
	// anonymous struct member
	/* 0x000c */	struct {
						void*		raw_data;
						u32			raw_data_size;
					}	cached_file;

	virtual	Scaleform::File*	OpenFile			( pcstr arg_0, s32 arg_1, s32 arg_2 ) override;
}; // class vostok_file_opener

STATIC_SIZE_ASSERT(vostok_file_opener, 0x14);

} // namespace survarium

#endif // #ifndef VOSTOK_FILE_OPENER_H_INCLUDED
