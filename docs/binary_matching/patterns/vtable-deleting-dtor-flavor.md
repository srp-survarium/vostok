# Vtable dtor slot: target holds `??_G` (scalar deleting), base emits `??_E` (vector) - link-time flavor, not anchor-steerable
tags: cpp:dtor cpp:virtual | topic:fold-icf topic:mangling
symptoms: ??_G in target unit vs ??_E in base, None row for the deleting dtor, .rdata vtable reloc at 50%
confidence: 8/10

A virtual-dtor class matched via a temp_include_all anchor pairs everything except the
deleting destructor: the target vftable references `??_G<class>` (scalar flavor), our
base emits `??_E<class>` (vector). The flavor in the vtable is a link-time choice our
toolchain makes differently; permanently bank the unit-level `None` rows (module-wide
across network's order/response headers: string/connect/send/send_queued/enqueue/
receive_*).

NEGATIVE RESULTS (tested): heap `new` + scalar `delete` of the class in the anchor TU
does NOT flip the base vftable slot to ??_G - the score stays None and the extra
operator new/delete churns unrelated ICF folds (-10 functions binary-wide). The
real-ctor-caller hope also did NOT pan out: with match_client.cpp fully matched the base
vtable STILL holds ??_E; the base then emits BOTH flavors (its ??_G lands
delinker-attributed to a sibling unit) and `pdb_fetch --view diff` pairs the two ??_G
bodies with only a compiler-emitted frame-size delta. Keep stack-constructed anchors.
