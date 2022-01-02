#include "xbase/x_allocator.h"
#include "xbase/x_console.h"
#include "xbase/x_darray.h"
#include "xbase/x_limits.h"
#include "xbase/x_math.h"
#include "xbase/x_printf.h"
#include "xbase/x_runes.h"
#include "xbase/x_stream.h"
#include "xbase/x_va_list.h"

#include "xtext/x_parser2.h"
#include "xtext/x_text_stream.h"

#include "xaoc2021/x_aoc.h"

#include "xunittest/xunittest.h"

#include <time.h>
#include <stdio.h>

extern unsigned char   day7_txt[];
extern unsigned int    day7_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day07)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        inline static s32 sum(s32 n)
        {
            return n * (n + 1) / 2;
        }

        UNITTEST_TEST(part_1)
        {
            array_t<s32>* submarines = array_t<s32>::create(0, 1024);

            s32 position = 0;
            for (s32 i=0; i<day7_txt_len; i++)
            {
                if (day7_txt[i]!=',')
                {
                    s8 d = day7_txt[i] - '0';
                    position = (position * 10) + d;
                } else {
                    submarines->add_item(position);
                    position = 0;
                }
            }
            submarines->add_item(position);

            s32 minpos = limits_t<s32>::maximum();
            s32 maxpos = limits_t<s32>::minimum();
            for (s32 i=0; i<submarines->size(); i++)
            {
                minpos = math::minimum(*submarines->get_item(i), minpos);
                maxpos = math::maximum(*submarines->get_item(i), maxpos);
            }

            s32 minfuel = limits_t<s32>::maximum();
            for (s32 p=minpos; p<=maxpos; ++p)
            {
                s32 fuel = 0;
                for (s32 i=0; i<submarines->size(); i++)
                {
                    s32* sm = submarines->get_item(i);
                    fuel += math::abs(*sm - p);
                }
                minfuel = math::minimum(minfuel, fuel);
            }

            printf("Minimal fuel solution: %d\n", minfuel);
        }

        UNITTEST_TEST(part_2)
        {
            array_t<s32>* submarines = array_t<s32>::create(0, 1024);

            s32 position = 0;
            for (s32 i=0; i<day7_txt_len; i++)
            {
                if (day7_txt[i]!=',')
                {
                    s8 d = day7_txt[i] - '0';
                    position = (position * 10) + d;
                } else {
                    submarines->add_item(position);
                    position = 0;
                }
            }
            submarines->add_item(position);

            s32 minpos = limits_t<s32>::maximum();
            s32 maxpos = limits_t<s32>::minimum();
            for (s32 i=0; i<submarines->size(); i++)
            {
                minpos = math::minimum(*submarines->get_item(i), minpos);
                maxpos = math::maximum(*submarines->get_item(i), maxpos);
            }

            s32 minfuel = limits_t<s32>::maximum();
            for (s32 p=minpos; p<=maxpos; ++p)
            {
                s32 fuel = 0;
                for (s32 i=0; i<submarines->size(); i++)
                {
                    s32* sm = submarines->get_item(i);
                    fuel += sum(math::abs(*sm - p));
                }
                minfuel = math::minimum(minfuel, fuel);
            }

            printf("Minimal fuel solution: %d\n", minfuel);
        }

    }
}
UNITTEST_SUITE_END
