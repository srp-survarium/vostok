////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.06.2026
////////////////////////////////////////////////////////////////////////////

// Temp build device for the carcass rebuild (see
// docs/binary_matching/library_carcass_rebuild.md): every rebuilt game header
// that no enabled TU includes yet is #included here so the compiler validates
// it. Add use_game_*() calls when /OPT:REF reachability is needed too.
// Delete this TU once the module's real TUs cover all headers.

#include "pch.h"

#include "flash_factory.h"
#include "flash_movie.h"
