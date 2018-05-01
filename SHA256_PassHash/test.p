#include <console>
#include <string>

#define MAX_SHA256_HASH 64
native SHA256_PassHash(const password[], const salt[], ret_hash[], ret_hash_len = sizeof(ret_hash), bool: pack = false, bool: uppercase = true);

main()
{
	new hash[MAX_SHA256_HASH + 1];

	// uppercase unpacked
	SHA256_PassHash("The quick brown fox jumps", " over the lazy dog", hash);
	printf("hash: %s\"%s\"\n", ispacked(hash) ? "!" : "", hash);

	// uppercase packed
	SHA256_PassHash("The quick brown fox jumps", " over the lazy dog", hash, _, true);
	printf("hash: %s\"%s\"\n", ispacked(hash) ? "!" : "", hash);

	// lowercase unpacked
	SHA256_PassHash("The quick brown fox jumps", " over the lazy dog", hash, _, false, false);
	printf("hash: %s\"%s\"\n", ispacked(hash) ? "!" : "", hash);

	// lowercase packed
	SHA256_PassHash("The quick brown fox jumps", " over the lazy dog", hash, _, true, false);
	printf("hash: %s\"%s\"\n", ispacked(hash) ? "!" : "", hash);
}
