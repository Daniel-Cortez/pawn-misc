SA-MP-compatible SHA256_PassHash implementation with 2 optional extra parameters `pack` and `uppercase` for returning the string in packed/unpacked form and in uppercase/lowercase respectively.

```Pawn
#define MAX_SHA256_HASH 64
native SHA256_PassHash(const password[], const salt[], ret_hash[], ret_hash_len = sizeof(ret_hash), bool:pack = false, bool:uppercase = true);
```
