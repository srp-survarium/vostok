#include "pch.h"
#include "anchor.h"

namespace survarium
{

// the zlib inflate pins live in survarium_pc_library_linkage.cpp - they must
// sit in an EXPLICIT link object so zlibN.lib (1.2.3) resolves them before the
// Scaleform prebuilt's bundled 1.2.7 copy does
// All reachability anchors retired (2026-08-22): the matched call graph must
// carry its own references now. Rows whose only reference was a pin go
// base-missing with their max held; each one is a missing real call edge.
IncludeAll::IncludeAll()
{
}


}
