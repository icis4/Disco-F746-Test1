/*
 *  Very simple example program
 */

#include "duktape.h"

static duk_ret_t native_print(duk_context *ctx) {
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("%s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

static duk_ret_t native_adder(duk_context *ctx) {
	int i;
	int n = duk_get_top(ctx);  /* #args */
	double res = 0.0;

	for (i = 0; i < n; i++) {
		res += duk_to_number(ctx, i);
	}

	duk_push_number(ctx, res);
	return 1;  /* one return value */
}

const char* mandel = ""
"/*\n"
" *  Mandelbrot example:\n"
" *\n"
" *    $ ./duk mandel.js\n"
" *    [...]\n"
" */\n"
"\n"
"function mandel() {\n"
"    var w = 76, h = 28, iter = 100;\n"
"    var i, j, k, c;\n"
"    var x0, y0, xx, yy, xx2, yy2;\n"
"    var line;\n"
"\n"
"    for (i = 0; i < h; i++) {\n"
"        y0 = (i / h) * 2.5 - 1.25;\n"
"\n"
"        for (j = 0, line = []; j < w; j++) {\n"
"            x0 = (j / w) * 3.0 - 2.0;\n"
"\n"
"            for (k = 0, xx = 0, yy = 0, c = '#'; k < iter; k++) {\n"
"                /* z -> z^2 + c\n"
"                 *   -> (xx+i*yy)^2 + (x0+i*y0)\n"
"                 *   -> xx*xx+i*2*xx*yy-yy*yy + x0 + i*y0\n"
"                 *   -> (xx*xx - yy*yy + x0) + i*(2*xx*yy + y0)\n"
"                 */\n"
"\n"
"                xx2 = xx*xx; yy2 = yy*yy;\n"
"\n"
"                if (xx2 + yy2 < 4.0) {\n"
"                    yy = 2*xx*yy + y0;\n"
"                    xx = xx2 - yy2 + x0;\n"
"                } else {\n"
"                    /* xx^2 + yy^2 >= 4.0 */\n"
"                    if (k < 3) { c = '.'; }\n"
"                    else if (k < 5) { c = ','; }\n"
"                    else if (k < 10) { c = '-'; }\n"
"                    else { c = '='; }\n"
"                    break;\n"
"                }\n"
"            }\n"
"\n"
"            line.push(c);\n"
"        }\n"
"\n"
"        print(line.join(''));\n"
"    }\n"
"}\n"
"\n"
"try {\n"
"    mandel();\n"
"} catch (e) {\n"
"    print(e.stack || e);\n"
"}\n"
"\n";

int duktape_main()
{
	duk_context *ctx = duk_create_heap_default();

	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
	duk_push_c_function(ctx, native_adder, DUK_VARARGS);
	duk_put_global_string(ctx, "adder");

	printf("*** JavaScript ***\n");

	duk_eval_string(ctx, "print('Hello world!');");

	duk_eval_string(ctx, "print('2+3=' + adder(2, 3));");
	// duk_eval_string(ctx, mandel);
	duk_pop(ctx);  /* pop eval result */

	duk_destroy_heap(ctx);
	printf("=== JavaScript ===\n");

	return 0;
}
