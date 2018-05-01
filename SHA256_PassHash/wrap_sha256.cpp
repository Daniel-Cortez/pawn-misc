/*==============================================================================
	SHA256_PassHash standalone implementation.

	Copyright (c) 2018 Stanislav Gromov.

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for
any purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software in
	a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

3.	This notice may not be removed or altered from any source distribution.
==============================================================================*/


#include "wrap_sha256.h"

#define LONESHA256_STATIC
#include "lonesha256.h"


inline bool IsValidBufferSize(AMX *amx, cell bufaddr, cell size) throw()
{
	return (size > (cell)0 || bufaddr + size <= amx->stp);
}

inline unsigned char *GetPackedArrayCharAddr(cell arr[], cell index) throw()
{
	unsigned char *addr;
#if BYTE_ORDER == LITTLE_ENDIAN
	const size_t idx_mod_cellsize = (size_t)index & (sizeof(cell) - 1);
	addr = (unsigned char *)arr + ((size_t)index - idx_mod_cellsize) +
		(sizeof(cell) - 1) - idx_mod_cellsize;
#else // BYTE_ORDER == LITTLE_ENDIAN
	addr = (unsigned char *)arr + (size_t)index;
#endif // BYTE_ORDER == LITTLE_ENDIAN
	return addr;
}

inline bool CheckArgCount(AMX *amx, const cell params[], cell num_expected) throw()
{
	const cell num_got = params[0] / (cell)sizeof(cell);
	if (num_got >= num_expected)
		return true;
	amx_RaiseError(amx, AMX_ERR_PARAMS);
	return false;
}

// #define MAX_SHA256_HASH 64
// native SHA256_PassHash(const password[], const salt[], ret_hash[], ret_hash_len = sizeof(ret_hash), bool: pack = false, bool: uppercase = true);
static cell AMX_NATIVE_CALL n_SHA256_PassHash(AMX *amx, const cell params[]) throw()
{
	static const unsigned char chars_lower[] =
		{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	static const unsigned char chars_upper[] =
		{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	const unsigned char *chars;
	cell *cstr, *csalt, *cbuf;
	int lenstr, lensalt;
	cell lenresult;
	char *str;
	cell bufsize;
	const cell sha256bufsize = (cell)32;
	char sha256buf[(size_t)sha256bufsize];

	enum
	{
		args_size,
		arg_string,
		arg_salt,
		arg_buffer,
		arg_size,
		arg_pack,
		arg_uppercase,
		__dummy_elem_, args_expected_number = __dummy_elem_ - 1
	};
	if (!CheckArgCount(amx, params, args_expected_number))
		return 0;

	if (amx_GetAddr(amx, params[arg_string], &cstr) != AMX_ERR_NONE)
	{
err_native:
		amx_RaiseError(amx, AMX_ERR_NATIVE);
		return 0;
	}
	if (amx_GetAddr(amx, params[arg_salt], &csalt) != AMX_ERR_NONE)
		goto err_native;
	if (amx_GetAddr(amx, params[arg_buffer], &cbuf) != AMX_ERR_NONE)
		goto err_native;
	bufsize = params[arg_size];
	if (!IsValidBufferSize(amx, params[arg_buffer], bufsize))
		goto err_native;
	amx_StrLen(cstr, &lenstr);
	amx_StrLen(csalt, &lensalt);
	if ((str = (char *)alloca((size_t)lensalt + (size_t)lenstr + 1)) == NULL)
		goto err_native;
	amx_GetString(str, cstr, 0, (size_t)lenstr + (size_t)1);
	amx_GetString(str + lenstr, csalt, 0, (size_t)lensalt + (size_t)1);
	lonesha256(
		(unsigned char *)sha256buf, (unsigned char *)str,
		(unsigned long)lensalt + (unsigned long)lenstr
	);
	if (params[arg_pack])
		bufsize *= (cell)sizeof(cell);
	lenresult = sha256bufsize * (cell)2;
	if (lenresult >= bufsize)
		lenresult = bufsize - 1;
	chars = (params[arg_uppercase]) ? chars_upper : chars_lower;
	if (!params[arg_pack])
	{
		for (cell i = 0, j = 0; i < lenresult;)
		{
			const cell odd = i & 1;
			cbuf[i++] = (cell)chars[(int)(sha256buf[j] >> (odd ? 0 : 4)) & 0xf];
			j += odd;
		}
		cbuf[lenresult] = (cell)'\0';
		return lenresult;
	}
	for (cell i = 0, j = 0; i < lenresult;)
	{
		const cell odd = i & 1;
		*GetPackedArrayCharAddr(cbuf, i++) =
			chars[(int)(sha256buf[j] >> (odd ? 0 : 4)) & 0xf];
		j += odd;
	}
	*GetPackedArrayCharAddr(cbuf, lenresult) = (unsigned char)'\0';
	return lenresult;
}


static const AMX_NATIVE_INFO natives[] =
{
	{ "SHA256_PassHash", n_SHA256_PassHash }
};

int AMXEXPORT AMXAPI AMXSHA256::Init(AMX *amx) throw()
{
	return amx_Register(amx, natives, (int)(sizeof(natives)/sizeof(natives[0])));
}

int AMXEXPORT AMXAPI AMXSHA256::Cleanup(AMX *amx) throw()
{
	(void)amx;
	return AMX_ERR_NONE;
}
