# Read the unit's OWN recovered symbol for access letters, not the ICF fold rep
tags: cpp:virtual | topic:fold-icf topic:mangling
symptoms: rich index shows an unrelated fold rep mangling, report.json lists the unit's own symbol
confidence: 8/10
variants: mangled-access-virtual.md, icf-fold-names-noise.md

A trivial virtual that ICF-folds shows the FOLD REPRESENTATIVE's mangling in the rich
index (e.g. landing::is_ready_for_transition folds to `?is_datatype@particle_action@...
@@UBE_NXZ`, public), but the delinker's per-unit target `.h`.obj keeps the function's OWN
recovered symbol (report.json lists it: `?is_ready_for_transition@jump_logic_state_landing
@survarium@@EBE_NXZ`, private). Trust the unit's own symbol for the access char - they
can disagree.
