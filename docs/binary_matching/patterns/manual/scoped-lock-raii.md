# A scoped-lock `mutex_raii raii(lock);` = ctor (acquire) at entry + dtor (release) at EVERY exit
tags: cpp:dtor cpp:local cpp:ctor | asm:call asm:lea | topic:structure-shape topic:codegen-idiom
symptoms: call ...::mutex_raii::mutex_raii (or ::lock) at entry, call ...::mutex_raii::clear (or ::unlock) at each return, one acquire / N releases
confidence: 9/10

A `T::mutex_raii raii( thing );` local acquires in its ctor and releases in its dtor (`clear()`).
Under /Od this is one `call ...::mutex_raii::mutex_raii` (or `::lock`) right after the locals are set
up, and a `call ...::mutex_raii::clear` (the dtor) at the START of EVERY return path — mid-loop
`return`s included. Count the `clear`/`unlock` calls = count the scope exits (the lock-specific case
of lexical-block-relocates-dtor.md, but the acquire/release symmetry is the diagnostic).

```cpp
mount_history_container::mutex_raii raii( history.policy() );
... return it->make_pointer_possibly_increase_destroy_count();   // releases here
... return NULL;                                                 // and here
```
```asm
call vostok::threading::simple_lock::mutex_raii::mutex_raii   ; acquire @ scope entry
...
call vostok::threading::simple_lock::mutex_raii::clear        ; release @ found-return
...
call vostok::threading::simple_lock::mutex_raii::clear        ; release @ NULL-return
```
Steerable: a stray `call ...::clear`/`::unlock` between body statements = a lock dtor at a return; one acquire must balance N releases. cite: vostok::vfs::find_in_mount_history mount_history.cpp:103.
