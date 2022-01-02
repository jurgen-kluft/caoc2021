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

extern unsigned char   day9_txt[];
extern unsigned int    day9_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day09)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        void process_input(s8*& grid, s32& sx, s32& sy, s32& mx, s32& my, s32& gridsizex, s32& gridsizey);
        void release_input(s8*& grid);

        static inline s8 low_point(s32 x, s32 y, s8* grid, s32 gridsizex, s32 gridsizey)
        {
            s8 const h = grid[y*gridsizex + x];
            
            bool lowpoint = true;            
            lowpoint = lowpoint && (h < grid[y*gridsizex + x - 1]);
            lowpoint = lowpoint && (h < grid[y*gridsizex + x + 1]);
            lowpoint = lowpoint && (h < grid[(y-1)*gridsizex + x]);
            lowpoint = lowpoint && (h < grid[(y+1)*gridsizex + x]);

            return lowpoint ? (h+1) : 0;
        }

        static inline s32 find_basin(s32 x, s32 y, s8* grid, s32 gridsizex, s32 gridsizey)
        {

            return 0;
        }

        UNITTEST_TEST(part_1)
        {
            s32 sx,sy,mx,my;
            s32 gridsizex,gridsizey;
            s8* grid;
            process_input(grid, sx, sy, mx, my, gridsizex, gridsizey);

            s32 risklevel = 0;
            for (s32 x=sx; x<mx; x++)
            {
                for (s32 y=sy; y<my; y++)
                {
                    risklevel += low_point(x,y,grid,gridsizex,gridsizey);
                }                
            }

            printf("risk level is %d\n", risklevel);

            release_input(grid);
        }

        UNITTEST_TEST(part_2)
        {
            s32 sx,sy,mx,my;
            s32 gridsizex,gridsizey;
            s8* grid;
            process_input(grid, sx, sy, mx, my, gridsizex, gridsizey);

            s32 largest_basins[3] = { -1, -1, -1 };
            for (s32 x=sx; x<mx; x++)
            {
                for (s32 y=sy; y<my; y++)
                {
                    s8 const point = grid[y*gridsizex + x];
                    if (point>=0 && point<9)
                    {
                        s32 basin_size = find_basin(x,y,grid,gridsizex,gridsizey);

                        if (largest_basins[0]==-1)
                            largest_basins[0] = basin_size;
                        else if (largest_basins[1]==-1)
                            largest_basins[1] = basin_size;
                        else if (largest_basins[2]==-1)
                            largest_basins[2] = basin_size;
                        else 
                        {
                            // keep this sorted from small to large
	                        g_qsort((void *)largest_basins, 3, sizeof(largest_basins[0]), g_cmp_s32);

                            for (s32 i=0; i<3; i++)
                            {
                                if (basin_size > largest_basins[i])
                                {
                                    largest_basins[i] = basin_size;
                                    break;
                                }
                            }
                        }
                    }
                }                
            }

            s32 const result = largest_basins[0]*largest_basins[1]*largest_basins[2];
            printf("sizes of largest 3 basins multiplied together %d\n", result);

            release_input(grid);
        }



        void release_input(s8*& grid)
        {
            alloc_t* allocator = context_t::system_alloc();
            allocator->deallocate(grid);
        }

        void process_input(s8*& grid, s32& sx, s32& sy, s32& mx, s32& my, s32& gridsizex, s32& gridsizey)
        {
            gridsizex=0;
            gridsizey=0;

            for (s32 i=0; i<day9_txt_len; ++i)
            {
                s8 const c = day9_txt[i];
                if (c == '\n')
                    break;
                gridsizex++;
            }
            for (s32 i=0; i<day9_txt_len; ++i)
            {
                s8 const c = day9_txt[i];
                if (c == '\n')
                    gridsizey++;
            }
            gridsizey++;

            gridsizex += 2;
            gridsizey += 2;
            sx = 1;
            sy = 1;
            mx = gridsizex - 1;
            my = gridsizey - 1;

            alloc_t* allocator = context_t::system_alloc();
            grid = (s8*)allocator->allocate(gridsizex*gridsizey, sizeof(void*));
            for (s32 i=0; i<(gridsizex*gridsizey); ++i)
                grid[i] = 9;
            
            s32 y = 1;
            s32 x = 1;
            for (s32 i=0; i<day9_txt_len; ++i)
            {
                s8 const c = day9_txt[i];
                if (c == '\n') {
                    x = 1;
                    y += 1;
                } else {
                    grid[y*gridsizex + x] = c - '0';
                    x++;
                }
            }
        }
    }
}
UNITTEST_SUITE_END
