# network_core: sequence_number operators + serialize/deserialize

Branch: match/network_core-async_connector-match worktree (vostok_2), base a9fd7422.

## Scope
Give `sequence_number<T>` real bodies for the comparison/arithmetic operators,
serialize/deserialize, and pre/post increment-decrement, plus fix the
`operator++(s32)` LNK1257 crash (post-increment returned by value with no `return`).

## Target symbols (only these are standalone; the rest inline into callers)
- `bool sequence_number<unsigned short>::operator<(...)` (rva 0x131910)
- `bool sequence_number<unsigned short>::operator<=(...)` (rva 0x132ae0)
- `int operator-<unsigned short>(...)` (rva 0x1280a0 / 0x137b20 carcass)

serialize / deserialize / operator++ / operator-- have NO standalone symbol; they
are always inlined into their callers (fill_packet_header, enqueue_impl,
update_acknowledgements, ...). They only score through those callers.

## operator< / operator<= (RFC1982-style serial comparison)
Target asm (operator<, condensed):
```
if (a >= b) goto .1                    ; a = this->m_number, b = other.m_number
if ((u32)(a + 0x8000) > b) goto true   ; ja (unsigned)
.1: if (b >= a) goto false
    if ((u32)(b + 0x8000) <= a) goto true  ; jbe
false: 0   true: 1
```
operator<= is byte-identical except the first compare is `jg` (`>`) instead of
`jge` (`>=`), i.e. the first clause uses `<=`.

Source (matches the two-clause shape):
```cpp
return ( m_number <  other.m_number && u32( m_number ) + 0x8000 >  other.m_number ) ||
       ( other.m_number < m_number  && u32( other.m_number ) + 0x8000 <= m_number );
// operator<= : first '<' becomes '<='
```

## operator-  (signed serial distance)
Target asm: `right.operator<=(left)` then either `s16(left.m_number - right.m_number)`
(the `+0x10000 & 0x8000FFFF` sign-extend dance is the `s16` cast) or `-(right - left)`.
```cpp
if ( right <= left )
    return s16( left.m_number - right.m_number );
return -( right - left );
```
Needs friend access to `m_number` -> added a `template<U> friend operator-` to the class.

## serialize / deserialize / ++ / --
Derived from caller asm (fill_packet_header 0x555bc0, enqueue_impl 0x556f.., etc.):
- `serialize(udp_match_packet&)` -> `packet.append( m_number )`
- `serialize(pbyte&)` -> `*(T*)pointer = m_number; pointer += sizeof(T)`
- `deserialize(packet_reader&)` -> `return sequence_number( reader.r<T>() )`
- `operator++()` -> `++m_number; return *this;`
- `operator++(s32)` -> `sequence_number r(*this); ++m_number; return r;`  (fixes LNK1257)
- `--` mirror.

## Instantiation note
The standalone operator</operator<=/operator- symbols only emit once a real caller
in udp_match_connection.cpp uses them (enqueue_impl post-increment, etc.). With all
udp_match_connection helpers still STUB they were absent from the base index; wiring
enqueue_impl's is_ordered block (post-increment) brings them in.
