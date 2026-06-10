# ICF fold names on intrusive_ptr helpers are NOISE; real type evidence is elsewhere
tags: cpp:template | topic:fold-icf
symptoms: intrusive_ptr<random type> ctors/operators applied to the same member, contradictory template args
confidence: 9/10
variants: boost-bind-icf-sibling-fold.md, fold-misname-empty-fn.md, unit-own-symbol-vs-fold-rep.md

All intrusive_ptr instantiations of an operation are byte-identical COMDATs; ICF folds
them onto ONE arbitrary winner, so the displayed template argument means NOTHING about
the member's root type. RELIABLE evidence in the PDB: (a) S_CALLSITEINFO records (the
carcass CALL SITE INFO lines - compiler-recorded callee types, ICF-immune), (b) NAMED
locals' types, (c) mangled names of real non-COMDAT symbols.

Evidence: refuted the "current_active_object root type must be inventory_item" flag - the call-site record for on_before_active_object_changed binds the members as `resource_ptr<interactive_object> const&` (base_player::tick_active_object).
