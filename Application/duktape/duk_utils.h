/*
 * duk_utils.h
 *
 *  Created on: 3.06.2020 г.
 *      Author: icis4
 */

#ifndef DUKTAPE_DUK_UTILS_H_
#define DUKTAPE_DUK_UTILS_H_

#include "duktape/duktape.h"

#define ADD_FUNCTION(FUNCTION_NAME_STRING, FUNCTION_NAME, PARAM_COUNT) \
		duk_push_c_function(ctx, FUNCTION_NAME, PARAM_COUNT); \
		duk_put_prop_string(ctx, -2, FUNCTION_NAME_STRING)

#define ADD_DBL(DBL_NAME, DBL_VALUE) \
		duk_push_number(ctx, DBL_VALUE); \
		duk_put_prop_string(ctx, -2, DBL_NAME)

#define ADD_UINT(UINT_NAME, UINT_VALUE) \
		duk_push_uint(ctx, UINT_VALUE); \
		duk_put_prop_string(ctx, -2, UINT_NAME)

#define ADD_INT(INT_NAME, INT_VALUE) \
		duk_push_int(ctx, INT_VALUE); \
		duk_put_prop_string(ctx, -2, INT_NAME)

#define ADD_STRING(STRING_NAME, STRING_VALUE) \
		duk_push_string(ctx, STRING_VALUE); \
		duk_put_prop_string(ctx, -2, STRING_NAME)

#define ADD_BOOLEAN(BOOLEAN_NAME, BOOLEAN_VALUE) \
		duk_push_boolean(ctx, BOOLEAN_VALUE); \
		duk_put_prop_string(ctx, -2, BOOLEAN_NAME)

#endif /* DUKTAPE_DUK_UTILS_H_ */
