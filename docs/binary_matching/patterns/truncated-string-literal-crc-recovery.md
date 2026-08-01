# A truncated string literal is fully recoverable - the mangled name carries its length and a CRC

tags: cpp:string-literal cpp:assert asm:push-immediate | topic:data-recovery topic:mangled-names topic:strings

## Symptom
A target instruction pushes a string literal whose delinked name is cut off:

```
0x71: push  ??_C@_0CH@BANAFJDE@data?9?$DOnew_resource?9?$DOshader_sourc@
0x76: call  vostok::debug::debug_message_box
```

The readable tail (`shader_sourc`) stops mid-word, so the literal cannot be
copied out of the disassembly. This is not a delinker limitation - **MSVC itself
truncates the decoded text in `??_C@` names at 32 characters**. But the name
still pins the string exactly.

## The two fields

`??_C@_0<len>@<hash>@<text-prefix>@`

* **`<len>` is the byte count INCLUDING the terminating nul**, in MSVC's number
  encoding: 1..10 are the digits `0`..`9` (i.e. the digit is `N-1`), larger
  values are hex digits `A`..`P` (`A`=0 .. `P`=15) most-significant first.
  * `_02` -> `'2'` -> 3 bytes -> `"vs"`.
  * `_09` -> 10 bytes -> `"%s/%s.%s/"`.
  * `_0BJ` -> `1,9` -> 0x19 = 25 -> `"resources/shaders/sm_4_0"` (24 chars).
  * `_0CH` -> `2,7` -> 0x27 = 39 -> a **38**-character string.
* **`<hash>` is `~crc32(bytes_including_nul)`** - the standard reflected CRC-32
  (zlib polynomial, init 0, no final xor), then bit-inverted - written as eight
  characters, each `'A' + nibble`, **most significant nibble first**.

```python
import zlib
def literal_hash(s):                      # s WITHOUT the nul
    v = (~zlib.crc32(s.encode() + b"\0")) & 0xffffffff
    return ''.join(chr(ord('A') + ((v >> (4*(7-i))) & 0xF)) for i in range(8))

literal_hash("vs")  == "FENABIDK"          # ??_C@_02FENABIDK@vs?$AA@
literal_hash("_")   == "IDAFKMJL"
literal_hash("%s/%s.%s/") == "OBPMJJKO"
```

Calibrate on any two short literals from the same binary before trusting a
recovery - three of the four escapes you need are `?$CF`=`%`, `?1`=`/`,
`?4`=`.`, `?9`=`-`, `?$DO`=`>`, `?$AA`=nul.

## Using it
The length tells you exactly how many characters are missing, and the hash
**verifies a guess with certainty** - no ambiguity, no "probably".

Worked example (`shader_binary_source_cook::converted_shader_loaded`): prefix
`data->new_resource->shader_sourc` (32 decoded chars), `_0CH` -> 38 chars total,
so 6 characters are missing and the first is obviously `e`. Three candidates for
the remaining five:

| candidate | hash | |
|---|---|---|
| `data->new_resource->shader_source == 0` | `BANAFJDE` | **matches** |
| `data->new_resource->shader_source is 0` | `KGIIBPDB` | no |
| `data->new_resource->shader_source != 0` | `LFMACDLH` | no |

That one literal recovered a whole shipped statement pair:

```cpp
data->new_resource->shader_source = result[0].get_managed_resource( );
if ( data->new_resource->shader_source == 0 )
    debug::debug_message_box( "data->new_resource->shader_source == 0" );
```

## When guessing fails: meet in the middle
CRC-32 is invertible one byte at a time, so a tail of up to ~5 unknown bytes is
brute-forceable without enumerating the full space: run the known prefix forward
into a dict keyed by intermediate CRC for the first `k` unknown bytes, and run
the target hash *backwards* over the nul plus the last `n-k` bytes, then
intersect. `crc_unstep` inverts one byte by finding the table entry whose top
byte matches the current CRC's top byte. Beware: an unconstrained alphabet will
happily produce garbage collisions (`...shader_source@p*|y` hashes to something
else but a wider search finds junk that hashes right), so keep the alphabet and
the length tight and prefer a hand-written candidate list.

## Do not over-read
A recovered literal is only worth writing into source when the *statement* it
belongs to is also established by the line records and the surrounding asm.
The hash proves the bytes, not the spelling of the code around them.

## Related
- `multiline-statement-closing-line-record.md` - which source line the statement sits on.
- `assert-empty-stub.md` (if present) - the other shape a compiled-out check takes.
