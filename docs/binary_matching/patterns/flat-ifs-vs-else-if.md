# `else if` merges chain-jmp + test into ONE row; flat sequential `if`s split them
tags: cpp:if | asm:jmp asm:test | topic:structure-shape
symptoms: mid-chain row test-sized too SMALL target-side, next row LOG-block-sized too BIG, ONLY-target row at the LOG line
confidence: 9/10
variants: inverted-guard-early-return.md, early-returns-vs-else-join.md

The /Od statement table attributes `} else if ( C )` as ONE row (else-jmp AND the C test)
with the arm's body row starting at the LOG; the flat spelling gives C its OWN test row
and the LOG a full row - one statement MORE, with a telltale size REDISTRIBUTION:

```
6: <0x5>  | <0xf>  | } else if ( bytes_transferred == 0 )  SIZE  (target bare jmp; base jmp+test)
7: <0xa>  | <0x77> | LOG_ERROR( ... )                      SIZE  (target test only; base test+LOG)
8: <0x7b> | --     | ONLY target (the LOG line)
```
```cpp
if ( A ) { ...; return; }
if ( C ) {
	LOG_ERROR( ... );        // flat second if - C gets its own row
```
Evidence: tcp_packet_socket::on_packet_has_been_sent (9+1 -> 10/10), on_packet_received (17+1 -> 18/18) via PR #291.
