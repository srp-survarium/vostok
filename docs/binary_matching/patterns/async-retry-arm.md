# A fat arm calling the function's OWN bind + async op = a RETRY statement
tags: cpp:if cpp:template | asm:call asm:rep-movsd | topic:structure-shape
symptoms: bind_t template args name the enclosing function, rep movsd 7 dwords endpoint deref, increment() row 0x8
confidence: 8/10
variants: boost-asio-placeholders.md

When an error-arm's big row ends in a call whose HANDLER template type names the
enclosing function, the original RE-ISSUES the same async op on the next endpoint - read
the bind_t in the called symbol's template args before guessing the arm.

```cpp
++iterator;                                            // its own 0x8 row (an increment() call)
if ( iterator != tcp::resolver::iterator( ) )
{
	resolver->async_resolve( *iterator, boost::bind( &async_connector::on_resolved, this, resolver, _1, _2 ) );
	return;
}
```
```asm
; deref *iterator: rep movsd 7 dwords from values[index]*0x4C into a 0x1c endpoint temp
```
Evidence: async_connector::on_resolved 63.37 -> 93.25, 22 -> 20 stmts (was guessed `m_host = iterator; connect( m_host );`).
