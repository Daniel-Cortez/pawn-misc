/*==============================================================================
	VectorSize implementation.

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


#include "wrap_vectorsize.h"
#include "amx.h"

#if PAWN_CELL_SIZE==32
	#define PawnFloat float
#elif PAWN_CELL_SIZE==64
	#define PawnFloat double
#else
	#error Unsupported cell size
#endif

#include <cmath>


inline bool CheckArgCount(AMX *amx, const cell params[], cell num_expected) throw()
{
	const cell num_got = params[0] / (cell)sizeof(cell);
	if (num_got >= num_expected)
		return true;
	amx_RaiseError(amx, AMX_ERR_PARAMS);
	return false;
}

// native Float:VectorSize(Float:x, Float:y, Float:z);
static cell AMX_NATIVE_CALL n_VectorSize(AMX *amx, const cell params[]) throw()
{
	float x, y, z, result;

	enum
	{
		args_size,
		arg_x,
		arg_y,
		arg_z,
		__dummy_elem_, args_expected_number = __dummy_elem_ - 1
	};
	if (!CheckArgCount(amx, params, args_expected_number))
		return 0;

	x = amx_ctof(params[arg_x]);
	y = amx_ctof(params[arg_y]);
	z = amx_ctof(params[arg_z]);
	result = std::sqrt(x * x + y * y + z * z);
	return amx_ftoc(result);
}


static const AMX_NATIVE_INFO natives[] =
{
	{ "VectorSize", n_VectorSize }
};

int AMXEXPORT AMXAPI AMXVectorSize::Init(AMX *amx) throw()
{
	return amx_Register(amx, natives, (int)(sizeof(natives)/sizeof(natives[0])));
}

int AMXEXPORT AMXAPI AMXVectorSize::Cleanup(AMX *amx) throw()
{
	(void)amx;
	return AMX_ERR_NONE;
}
