# Emitting a DCE'd static helper: reconstruct its LTCG-inlined caller and anchor THAT
tags: cpp:static | topic:anchoring
symptoms: target static with NO direct caller (only address-taken via boost::bind), our base never emits it
confidence: 8/10
variants: static-plain-name-pairing.md, ltcg-dse-empty-ctor.md

A target-side static existing only via an address-take (boost::bind) whose binding
member function has no standalone target symbol either (LTCG inlined it; /OPT:REF
dropped ours): define the member function in the TU (the bind proves it existed there)
and anchor it - the bind's address-take emits the static out-of-line.

Evidence: inventory::serialize defined + anchored -> call_item_serialize None -> 49.5%, inventory::deserialize None -> 100, body_part_parameters::serialize None -> 87.7% (plain pointer-call anchors suffice for non-DCE'd publics).
