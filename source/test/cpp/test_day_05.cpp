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


extern unsigned char   day5_txt[];
extern unsigned int    day5_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day05)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        struct line_t
        {
            line_t(u16 _x1, u16 _y1, u16 _x2, u16 _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
            u16 x1,y1;
            u16 x2,y2;
        };

        class line_parser_t
        {
        public:
            line_parser_t()
            {
                // 223,805 -> 223,548
            }

            bool parse(crunes_t& line, u16& x1, u16& y1, u16& x2, u16& y2)
            {
                crunes_t format("%u,%u -> %u,%u");
                s32 const vars = vsscanf(line, format, va_r_list_t( (va_r_t(&x1)), (va_r_t(&y1)), (va_r_t(&x2)), (va_r_t(&y2))));
                return vars > 0;
            }
        };

        UNITTEST_TEST(part_1)
        {
            mem_stream    memtext(day5_txt, day5_txt_len);
            text_stream_t text(&memtext, text_stream_t::encoding_ascii);

            array_t<line_t>* lines = array_t<line_t>::create(0, 1024);

            crunes_t line;
       		line_parser_t lp;
            u16 x1, y1, x2, y2;

            u16 xmin = 0;
            u16 ymin = 0;
            u16 xmax = limits_t<u16>::minimum();
            u16 ymax = limits_t<u16>::minimum();
            while (text.readLine(line))
            {
                CHECK_TRUE(lp.parse(line, x1, y1, x2, y2));

                // Only keep horizontal and vertical lines
                if (x1 == x2 || y1 == y2)
                {
                    xmax = math::maximum(xmax, x1, x2);
                    ymax = math::maximum(ymax, y1, y2);
                    lines->add_item(line_t(x1, y1, x2, y2));
                }
            }

            // Ok we got all the horizontal and vertical lines
            u16 gridsizex = math::next_power_of_two(xmax - xmin);
            u16 gridsizey = math::next_power_of_two(ymax - ymin);

            array_t<s32>* grid = array_t<s32>::create(gridsizex*gridsizey, gridsizex*gridsizey);
            for (s32 x=0; x<gridsizex; ++x)
            {
                for (s32 y=0; y<gridsizey; ++y)
                {
                    grid->set_item(y*gridsizex + x, 0);
                }
            }
            
            // Process all lines and increment each cell that they touch
            for (s32 i=0; i<lines->size(); ++i)
            {
                line_t* line = lines->get_item(i);
                if (line->x1 == line->x2)
                {
                    // Vertical line
                    u16 const x = line->x1;
                    u16 const ys = math::minimum(line->y1, line->y2);
                    u16 const ye = math::maximum(line->y1, line->y2);
                    for (s32 y=ys; y<=ye; y++)
                    {   
                        s32* cell = grid->get_item(y*gridsizex + x);
                        (*cell)++;
                    }
                }
                else if (line->y1 == line->y2)
                {
                    // Horizontal line
                    u16 const y = line->y1;
                    u16 const xs = math::minimum(line->x1, line->x2);
                    u16 const xe = math::maximum(line->x1, line->x2);
                    for (s32 x=xs; x<=xe; x++)
                    {   
                        s32* cell = grid->get_item(y*gridsizex + x);
                        (*cell)++;
                    }
                }
            }

            // Count cells that have a count of 2 or more
            u32 count = 0;
            for (s32 x=0; x<gridsizex; ++x)
            {
                for (s32 y=0; y<gridsizey; ++y)
                {
                    s32* cell = grid->get_item(y*gridsizex + x);
                    if (*cell >= 2)
                    {
                        count++;
                    }
                }
            }

            printf("Count: %d\n", count);

            grid->destroy(grid);
            lines->destroy(lines);

            text.close();
            memtext.close();
        }

        UNITTEST_TEST(part_2)
        {
            mem_stream    memtext(day5_txt, day5_txt_len);
            text_stream_t text(&memtext, text_stream_t::encoding_ascii);

            array_t<line_t>* lines = array_t<line_t>::create(0, 1024);

            crunes_t line;
       		line_parser_t lp;
            u16 x1, y1, x2, y2;

            u16 xmin = 0;
            u16 ymin = 0;
            u16 xmax = limits_t<u16>::minimum();
            u16 ymax = limits_t<u16>::minimum();
            while (text.readLine(line))
            {
                CHECK_TRUE(lp.parse(line, x1, y1, x2, y2));

                xmax = math::maximum(xmax, x1, x2);
                ymax = math::maximum(ymax, y1, y2);
                lines->add_item(line_t(x1, y1, x2, y2));
            }

            // Ok we got all the lines
            u16 gridsizex = math::next_power_of_two(xmax - xmin);
            u16 gridsizey = math::next_power_of_two(ymax - ymin);

            array_t<s32>* grid = array_t<s32>::create(gridsizex*gridsizey, gridsizex*gridsizey);
            for (s32 x=0; x<gridsizex; ++x)
            {
                for (s32 y=0; y<gridsizey; ++y)
                {
                    grid->set_item(y*gridsizex + x, 0);
                }
            }
            
            // Process all lines and increment each cell that they touch
            for (s32 i=0; i<lines->size(); ++i)
            {
                line_t* line = lines->get_item(i);
                if (line->x1 == line->x2)
                {
                    // Vertical line
                    u16 const x = line->x1;
                    u16 const ys = math::minimum(line->y1, line->y2);
                    u16 const ye = math::maximum(line->y1, line->y2);
                    for (s32 y=ys; y<=ye; y++)
                    {   
                        s32* cell = grid->get_item(y*gridsizex + x);
                        (*cell)++;
                    }
                }
                else if (line->y1 == line->y2)
                {
                    // Horizontal line
                    u16 const y = line->y1;
                    u16 const xs = math::minimum(line->x1, line->x2);
                    u16 const xe = math::maximum(line->x1, line->x2);
                    for (s32 x=xs; x<=xe; x++)
                    {   
                        s32* cell = grid->get_item(y*gridsizex + x);
                        (*cell)++;
                    }
                }
                else 
                {
                    // Diagonally (45 degrees) line
                    // This means that an increment in x is also a single unit change in y
                    u16 const xs = math::minimum(line->x1, line->x2);
                    u16 const xe = (line->x1 == xs) ? line->x2 : line->x1;
                    u16 const ys = (line->x1 == xs) ? line->y1 : line->y2;
                    u16 const ye = (line->x1 == xs) ? line->y2 : line->y1;

                    s32 y = ys;
                    s32 dy = (ys < ye) ? 1 : -1;
                    for (s32 x=xs; x<=xe; x++)
                    {   
                        s32* cell = grid->get_item(y*gridsizex + x);
                        (*cell)++;
                        y += dy;
                    }
                }
            }

            // Count cells that have a count of 2 or more
            u32 count = 0;
            for (s32 x=0; x<gridsizex; ++x)
            {
                for (s32 y=0; y<gridsizey; ++y)
                {
                    s32* cell = grid->get_item(y*gridsizex + x);
                    if (*cell >= 2)
                    {
                        count++;
                    }
                }
            }

            printf("Count: %d\n", count);

            grid->destroy(grid);
            lines->destroy(lines);

            text.close();
            memtext.close();
        }

    }
}
UNITTEST_SUITE_END
