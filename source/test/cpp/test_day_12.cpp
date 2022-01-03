#include "xbase/x_allocator.h"
#include "xbase/x_console.h"
#include "xbase/x_context.h"
#include "xbase/x_darray.h"
#include "xbase/x_integer.h"
#include "xbase/x_limits.h"
#include "xbase/x_math.h"
#include "xbase/x_printf.h"
#include "xbase/x_runes.h"
#include "xbase/x_qsort.h"
#include "xbase/x_stream.h"
#include "xbase/x_va_list.h"

#include "xaoc2021/x_aoc.h"

#include "xunittest/xunittest.h"

#include <time.h>
#include <stdio.h>

// NOTE: Make sure that the input text has an extra end-of-line added!!
extern unsigned char   day12_txt[];
extern unsigned int    day12_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day12)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(part_1)
        {
        }

        UNITTEST_TEST(part_2)
        {

        }
    }
}
UNITTEST_SUITE_END
