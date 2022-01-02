#include "xbase/x_allocator.h"
#include "xbase/x_console.h"
#include "xbase/x_darray.h"
#include "xbase/x_integer.h"
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

extern unsigned char   day8_txt[];
extern unsigned int    day8_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day08)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        struct pattern_t
        {
            u8 signals[10];
            u8 outputs[4];
        };

        array_t<pattern_t>* parse_input();
        void identify_pattern(pattern_t* pattern);
        s32 decode_output(pattern_t const* pattern);

        UNITTEST_TEST(part_1)
        {
            array_t<pattern_t>* input = parse_input();

            s32 count = 0;
            for (s32 i=0; i<input->size(); ++i)
            {
                pattern_t* pattern = input->get_item(i);

                for (s32 j=0; j<4; ++j)
                {
                    s32 numonebits = xcountBits(pattern->outputs[j]);
                    if (numonebits == 2 || numonebits == 3 || numonebits == 4 || numonebits == 7)
                    {
                        count++;
                    }
                }
            }

            printf("Number of unique signals: %d\n", count);
        }

        UNITTEST_TEST(part_2)
        {
            array_t<pattern_t>* input = parse_input();

            s32 count = 0;
            for (s32 i=0; i<input->size(); ++i)
            {
                pattern_t* pattern = input->get_item(i);
                identify_pattern(pattern);
            }

            u32 decoded = 0;
            for (s32 i=0; i<input->size(); ++i)
            {
                pattern_t* pattern = input->get_item(i);
                decoded += decode_output(pattern);
            }

            printf("Sum of decoded output values: %d\n", decoded);
        }

        void identify_pattern(pattern_t* pattern)
        {
            u8 dec[10];
            for (s32 i=0; i<10; i++)
                dec[i] = 0;

            // The standard ones we can easily identify are the numbers
            // 1, 4, 7 and 8, they have a unique number of bars.
            for (s32 i=0; i<10; i++)
            {
                u8 const n = pattern->signals[i];
                s32 numonebits = xcountBits(n);
                if (numonebits == 2)
                {
                    dec[1] = n;
                }
                else if (numonebits == 3)
                {
                    dec[7] = n;
                }
                else if (numonebits == 4)
                {
                    dec[4] = n;
                }
                else if (numonebits == 7)
                {
                    dec[8] = n;
                }
            }

            for (s32 i=0; i<10; i++)
            {                    
                u8 const number = pattern->signals[i];
                if (number == dec[1] || number == dec[4] || number == dec[7] || number == dec[8])
                    continue;

                if (xcountBits((u8)(number & dec[1])) == 2)
                {
                    // 0, 1, 3, 4, 7, 8 or 9
                    if (xcountBits(number) == 5)
                    {
                        dec[3] = number;                        // number 3
                    }
                    else
                    {
                        // 0 or 9
                        if (xcountBits((u8)(number & dec[4])) == 4)
                        {
                            dec[9] = number;                        // number 9
                        }
                        else 
                        {
                            dec[0] = number;                        // number 0
                        }
                    }
                }
                else
                {
                    // 2, 5 or 6
                    if (xcountBits(number) == 6)
                    {
                        dec[6] = number;                        // number 6
                    }
                    else 
                    {
                        if (xcountBits((u8)(number & dec[4])) == 3)
                        {
                            dec[5] = number;                        // number 5
                        }
                        else
                        {
                            dec[2] = number;                        // number 2
                        }
                    }
                }
            }

            // write the sorted numbers back into the pattern signals
            for (s32 i=0; i<10; i++)
            {
                pattern->signals[i] = dec[i];
            }
        }

        s32 decode_output(pattern_t const* pattern)
        {
            // Ok, so in dec[] we have identified the numbers, now decode output
            s32 decoded = 0;
            for (s32 i=0; i<4; ++i)
            {
                const u8 number = pattern->outputs[i];
                for (s32 j=0; j<10; ++j)
                {
                    if (pattern->signals[j] == number)
                    {
                        decoded = (decoded*10) + j;
                        break;
                    }
                }
            }
            return decoded;
        }

        array_t<pattern_t>* parse_input()
        {
            array_t<pattern_t>* input = array_t<pattern_t>::create(0, 512);

            s8 n = 0;
            s8 s = 0;
            s8 o = 0;
            bool output = false;
            pattern_t pattern;
            for (s32 i=0; i<day8_txt_len; i++)
            {
                s8 const c = day8_txt[i];
                if (c==' ')
                {
                    if (output) {
                        pattern.outputs[o++] = n;
                    } else {
                        pattern.signals[s++] = n;
                    }
                    n = 0;
                }
                else if (c >= 'a' && c <= 'g')
                {
                    s8 bit = c - 'a';
                    n = n | (1<<bit);
                }
                else if (c == '\n')
                {
                    // write final parsed output value
                    pattern.outputs[o++] = n;

                    // emit pattern                    
                    input->add_item(pattern);

                    n = 0;
                    s = 0;
                    o = 0;
                    output = false;
                }
                else if (c == '|')
                {
                    i++;
                    output = true;
                }
            }

            // write final parsed output value
            pattern.outputs[o++] = n;

            // emit pattern                    
            input->add_item(pattern);

            return input;
        }
    }
}
UNITTEST_SUITE_END
