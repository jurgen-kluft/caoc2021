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

#include "xtext/x_parser2.h"
#include "xtext/x_text_stream.h"

#include "xaoc2021/x_aoc.h"

#include "xunittest/xunittest.h"

#include <time.h>
#include <stdio.h>

// NOTE: Make sure that the input text has an extra end-of-line added!!
extern unsigned char   day10_txt[];
extern unsigned int    day10_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day10)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        static inline bool detect_error(u8 const* begin, u8 const* end, u32& error_score)
        {
            s32 const stack_capacity = 256;
            u8 stack[stack_capacity];
            s32 stack_size = 0;


            while (begin < end)
            {
                u8 const c = *begin++;
                if (c == '[' || c == '{' || c == '<' || c == '(')
                {
                    ASSERT(stack_size < stack_capacity);
                    stack[stack_size++] = c;
                }
                else
                {
                    u8 const open = stack[--stack_size];
                    u8 const close = c;
                    if ( (open=='[' && close==']') || (open=='{' && close=='}') || (open=='<' && close=='>') || (open=='(' && close==')') )
                    {
                        // correct open/close pair
                    }
                    else
                    {
                        // wrong open/close pair found
                        // return the wrongly closed character
                        switch (close)
                        {
                            case ')': error_score = 3; break;
                            case ']': error_score = 57; break;
                            case '}': error_score = 1197; break;
                            case '>': error_score = 25137; break;
                        }
                        return true;
                    }
                }
            }

            error_score = 0;
            return false;
        }

        static inline void complete_navline(u8 const* begin, u8 const* end, u64& completion_score)
        {
            s32 const stack_capacity = 256;
            u8 stack[stack_capacity];
            s32 stack_size = 0;

            while (begin < end)
            {
                u8 const c = *begin++;
                if (c == '[' || c == '{' || c == '<' || c == '(')
                {
                    ASSERT(stack_size < stack_capacity);
                    stack[stack_size++] = c;
                }
                else
                {
                    --stack_size;
                }
            }

            // the stack contains the remaining open characters that still need to be closed
            completion_score = 0;
            while (stack_size > 0)
            {
                u8 const c = stack[--stack_size];
                switch(c)
                {
                    case '(': completion_score *= 5; completion_score += 1; break;
                    case '[': completion_score *= 5; completion_score += 2; break;
                    case '{': completion_score *= 5; completion_score += 3; break;
                    case '<': completion_score *= 5; completion_score += 4; break;
                }
            }

        }

        UNITTEST_TEST(part_1)
        {
            // keep track of the total error score
            u32 total_error = 0;

            u8 const* navline_begin = day10_txt;
            for (u32 i=0; i<day10_txt_len; ++i)
            {
                u8 const c = day10_txt[i];
                if (c == '\n')
                {
                    // end of line
                    u8 const* navline_end = &day10_txt[i];

                    u32 error_score = 0;
                    if (detect_error(navline_begin, navline_end, error_score))
                    {
                        total_error += error_score;
                    }
                    navline_begin = navline_end + 1;
                }
            }

            printf("total error score %d\n", total_error);
        }

        UNITTEST_TEST(part_2)
        {
            // keep track of the total error score
            s32 number_of_lines = 0;
            for (u32 i=0; i<day10_txt_len; ++i)
            {
                u8 const c = day10_txt[i];
                if (c == '\n')
                {
                    number_of_lines++;
                }
            }

            s32 const scores_capacity = 100;
            u64 scores[scores_capacity];
            s32 scores_size = 0;

            u8 const* navline_begin = day10_txt;
            for (u32 i=0; i<day10_txt_len; ++i)
            {
                u8 const c = day10_txt[i];
                if (c == '\n')
                {
                    // end of line
                    u8 const* navline_end = &day10_txt[i];

                    u32 error_score = 0;
                    if (!detect_error(navline_begin, navline_end, error_score))
                    {
                        u64 completion_score = 0;
                        complete_navline(navline_begin, navline_end, completion_score);
                        scores[scores_size++] = completion_score;
                    }
                    navline_begin = navline_end + 1;
                }
            }

            g_qsort(scores, scores_size);
            u64 const mid_score = scores[scores_size / 2];

            printf("mid completion score %lld\n", mid_score);
        }
    }
}
UNITTEST_SUITE_END
