# Structure verification: player_stealth ctor + load

Module: game_core. PR #140. Branch: match/game_core-player_stealth-ctor-load.
Source: sources/vostok/game_core/sources/player_stealth.cpp

## 1. player_stealth::player_stealth() (default ctor)

Mangled: ??0player_stealth@survarium@@QAE@XZ, size 14 bytes.
report.json fuzzy_match_percent = 100.0.

Verdict: STRUCTURE MATCH (quantity + size agree).

TARGET (rva 0x59aad0):
  ; 0 statements, 0xe bytes

BASE (rva 0x44aff0):
  ; 0 statements, 0xe bytes

Both render as a body-less ctor: 0 statements, identical 0xe byte size. The
empty source body { } with no member-initializer list is exactly what the
target shape requires. No divergence. The 100% here is over the correct
(empty) structure, not the trap case.

## 2. player_stealth::load(configs::binary_config_value const&)

Mangled: ?load@player_stealth@survarium@@QAEXABVbinary_config_value@configs@vostok@@@Z,
size 300 bytes. report.json fuzzy_match_percent = 100.0.

Verdict: STRUCTURE MATCH (quantity + size agree).

TARGET (rva 0x59aae0): ; 13 statements, 0x12c bytes
BASE   (rva 0x44b000): ; 13 statements, 0x12c bytes

Side-by-side (offset / size / target srcline -> base statement):

  off    size   target  base statement
  0x00   <0x9>  L44     {
  0x09   <0x19> L45     m_default_value             = (float)config["default_value"];
  0x22   <0x1a> L46     m_default_sound_value       = (float)config["default_sound_value"];
  0x3c   <0x1a> L47     m_stand_factor              = (float)config["stand_factor"];
  0x56   <0x1a> L48     m_crouch_factor             = (float)config["crouch_factor"];
  0x70   <0x1a> L49     m_crouch_sound_factor       = (float)config["crouch_sound_factor"];
  0x8a   <0x1a> L50     m_walk_factor               = (float)config["walk_factor"];
  0xa4   <0x1a> L51     m_walk_sound_factor         = (float)config["walk_sound_factor"];
  0xbe   <0x1a> L52     m_sprint_factor             = (float)config["sprint_factor"];
  0xd8   <0x1a> L53     m_sprint_sound_factor       = (float)config["sprint_sound_factor"];
  0xf2   <0x1a> L54     m_detection_level           = (float)config["detection_level"];
  0x10c  <0x1a> L55     m_always_visible_distance   = (float)config["always_visible_distance"];
  0x126  <0x6>  L56     }

Every statement aligns 1:1 on offset and size. 13 statements both sides:
the open brace { (L44), 11 body assignments (L45-L55), and the closing brace }
(L56, the 6-byte scope exit). The source writes the 11 field loads as flat body
assignments with no wrapping block, no early-return guard, no init list - which
is exactly the target shape. No QUANTITY and no SIZE divergence.

## Outcome

Both functions: STRUCTURE MATCH. No mislabeled-match flag warranted; the
STATE[100%|DONE] tags are accurate. No source change, no carcass restore, no
ledger downgrade. No rebuild needed (no bytes changed).
