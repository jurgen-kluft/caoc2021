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

extern unsigned char   day6_txt[];
extern unsigned int    day6_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day06)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(part_1)
        {
            s64 age_group[] = {0,0,0,0,0,0,0,0,0};
            for (s32 i=0; i<day6_txt_len; i++)
            {
                if (day6_txt[i]!=',')
                {
                    s8 age = day6_txt[i] - '0';
                    age_group[age]++;
                }
            }

            s32 const ndays = 80;
            
            // Process all the age groups over n days
            for (s32 d = 0; d<ndays; ++d)
            {
                // each age group is moving down one slot
                s64 evolve[9];
                for (s32 g = 0; g<8; ++g)
                {
                    evolve[g] = age_group[g+1];
                }
                evolve[6] += age_group[0];
                evolve[8] = age_group[0];   // All the age group '0' are also spawning new fishes in the age group '8'
                
                for (s32 g = 0; g<=8; ++g)
                    age_group[g] = evolve[g];
            }

            s64 fishes = 0;
            for (s32 g = 0; g<=8; ++g)
                fishes += age_group[g];

            printf("Number of fish: %d\n", fishes);
        }

        UNITTEST_TEST(part_2)
        {
            s64 age_group[] = {0,0,0,0,0,0,0,0,0};
            for (s32 i=0; i<day6_txt_len; i++)
            {
                if (day6_txt[i]!=',')
                {
                    s8 age = day6_txt[i] - '0';
                    age_group[age]++;
                }
            }

            s32 const ndays = 256;
            
            // Process all the age groups over n days
            for (s32 d = 0; d<ndays; ++d)
            {
                // each age group is moving down one slot
                s64 evolve[9];
                for (s32 g = 0; g<8; ++g)
                {
                    evolve[g] = age_group[g+1];
                }
                evolve[6] += age_group[0];
                evolve[8] = age_group[0];   // All the age group '0' are also spawning new fishes in the age group '8'
                
                for (s32 g = 0; g<=8; ++g)
                    age_group[g] = evolve[g];
            }

            s64 fishes = 0;
            for (s32 g = 0; g<=8; ++g)
                fishes += age_group[g];

            printf("Number of fish: %d\n", fishes);

        }

    }
}
UNITTEST_SUITE_END
