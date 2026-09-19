# Definition parameter qualifiers

The difference list is `parameter_differences.tsv`. Header declarations are
deliberately unchanged: declaration and definition top-level qualifiers can
differ. Neither a procedure argument list nor a plain named parameter type alone
proves the absence of source const. Compare the target variable record with the
record emitted by the known current source before claiming a discrepancy.

Cross-check against build dbfa79b0 rejected seven initially proposed removals:
the UDP disconnect enum, both TCP error callback enums/classes, and both HTTP
error callback classes already have plain variable types in our PDB despite
const source definitions. Those edits were undone; their rows preserve the
rejection evidence. The unnamed UDP on_error parameters likewise emit plain
types from const source and remain unchanged. This observation is not a general
claim that every enum/class qualifier is always erased.

Twelve changes remain supported: async and core TCP connect host/port, core
entry-point dest_host, network world order/response pointers, and HTTP wrapper
server/path/content pointers. The pre-change base records demonstrably differ
from retail for the latter ten. The two async changes were verified after build:
base host1443 has pointer attributes33802, port4c28 is const u16, agreeing with
retail1036/1479. The other ten are applied without another build as requested.

Build dbfa79b0 passed in10m15s, with zero score/presence/fold changes. All six
async procedures pair uniquely and have matching named-local lists; existing
constructor134/128B and connect750/714B size differences remain. No network-owned
compiler warnings;13 compiler warnings elsewhere,236 linker warnings and two
known Scaleform parser skips. This build does not measure the later batch.

Earlier commit c8ac5dd58 covered packet/reader qualifiers and TCP packet layout,
not a complete definition-parameter comparison of these wrappers. Historical
HTTP commit83cc08403 retained const definitions while reporting structure
matches. Neither record establishes that all parameter CV was already audited.
