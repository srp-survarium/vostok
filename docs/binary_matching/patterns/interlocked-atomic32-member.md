# A member fed to interlocked_* as the TARGET operand is threading::atomic32_type
tags: cpp:member | topic:structure-shape
symptoms: COMPILE_ASSERT do_not_pass_NON_VOLATILE_values_to_INTERLOCKED_functions, PDB shows plain long
confidence: 9/10

threading_functions_guard.h defines `interlocked_*(T&, ...)` template overloads whose
body is a COMPILE_ASSERT(false). For a NON-volatile long member the template (exact
match) beats the real `interlocked_exchange(atomic32_type& = long volatile&, long)`, so
the build breaks - the original header declared the member `threading::atomic32_type`
even when the PDB-dumped structure shows plain `long` (the generator drops volatile).
The VALUE operand stays non-volatile.

```cpp
threading::atomic32_type m_last_receive_time_in_ms;   // PDB said long
```
Evidence: udp_match_connection::process_incoming_packet - interlocked_exchange(m_last_receive_time_in_ms, m_last_send_attempt_time_in_ms).
