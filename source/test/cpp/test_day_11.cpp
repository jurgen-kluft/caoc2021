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
extern unsigned char   day11_txt[];
extern unsigned int    day11_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day11)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        template<typename T>
        static inline void set_all(T* data, u32 length, T const& set)
        {
            s32 i = 0;
            while (i<length) { data[i++]=set; }
        }

        template<typename T>
        static inline s32 count_until(T const* data, u32 length, T const& until)
        {
            s32 i = 0;
            while (i<length) { if (data[i] == until) return i; ++i; }
            return -1;
        }

        template<typename T>
        static inline s32 count_occurances(T const* data, u32 length, T const& item)
        {
            s32 i = 0;
            s32 c = 0;
            while (i<length) { c += (data[i++] == item) ? 1 : 0; }
            return c;
        }

        static void update_cell(s16 x, s16 y, u8* grid, s32 gridsizex, s32 gridsizey)
        {
            // a cell with 255 is a barrier
            // a cell with 10 is a cell that needs to flash
            // a cell with 11 is a cell that has flashed
            u8& cell = grid[y*gridsizex + x];
            if (cell == 255 || cell == 10 || cell == 11)
                return;
            cell += 1;
        }

        static bool cell_will_flash(s16 x, s16 y, u8 const* grid, s32 gridsizex, s32 gridsizey)
        {
            u8 const cell = grid[y*gridsizex + x];
            return (cell == 10);
        }

        static bool cell_is_zero(s16 x, s16 y, u8 const* grid, s32 gridsizex, s32 gridsizey)
        {
            u8 const cell = grid[y*gridsizex + x];
            return (cell == 0);
        }

        static inline bool reset_cell(s16 x, s16 y, u8* grid, s32 gridsizex, s32 gridsizey)
        {
            u8& cell = grid[y*gridsizex + x];
            if (cell == 11)
            {
                cell = 0;
                return true;
            }
            return false;
        }

        static void cell_do_flash(s16 x, s16 y, u8* grid, s32 gridsizex, s32 gridsizey)
        {
            update_cell(x-1, y-1, grid,gridsizex,gridsizey);
            update_cell(x+0, y-1, grid,gridsizex,gridsizey);
            update_cell(x+1, y-1, grid,gridsizex,gridsizey);
            update_cell(x-1, y+0, grid,gridsizex,gridsizey);
            update_cell(x+1, y+0, grid,gridsizex,gridsizey);
            update_cell(x-1, y+1, grid,gridsizex,gridsizey);
            update_cell(x+0, y+1, grid,gridsizex,gridsizey);
            update_cell(x+1, y+1, grid,gridsizex,gridsizey);

            grid[y*gridsizex + x] = 11;
        }

        static bool process_grid(s32 sx, s32 mx, s32 sy, s32 my, u8* grid, s32 gridsizex, s32 gridsizey, s32& total_flashes)
        {
            // Increment each cell by 1
            for (s16 x=sx; x<mx; ++x)
            {
                for (s16 y=sy; y<my; ++y)
                    grid[y*gridsizex + x] += 1;
            }

            do {
                s32 flashes = 0;
                for (s16 x=sx; x<mx; ++x)
                {
                    for (s16 y=sy; y<my; ++y)
                    {
                        if (cell_will_flash(x,y,grid,gridsizex,gridsizey))
                        {
                            flashes += 1;
                            cell_do_flash(x,y,grid,gridsizex,gridsizey);
                        }
                    }
                }
                if (flashes == 0)
                    break;
                total_flashes += flashes;
            } while (true);

            s32 reset_count = 0;
            for (s16 x=sx; x<mx; ++x)
            {
                for (s16 y=sy; y<my; ++y)
                {
                    if (reset_cell(x,y,grid,gridsizex,gridsizey))
                        reset_count++;
                }
            }
            return reset_count == ((mx-sx)*(my-sy));
        }

        UNITTEST_TEST(part_1)
        {
            s32 gridsizex = count_until(day11_txt, day11_txt_len, (unsigned char)'\n');
            s32 gridsizey = count_occurances(day11_txt, day11_txt_len, (unsigned char)'\n');

            // Adjust grid size x and y to have a border around the grid so that
            // we do not have to care about out-of-bounds.
            gridsizex += 2;
            gridsizey += 2;

            alloc_t* allocator = context_t::system_alloc();
            s32 const gridsize = gridsizex*gridsizey;
            u8* grid = (u8*)allocator->allocate(gridsize*sizeof(u8));
            set_all(grid, gridsize, (u8)255);

            s32 const sx = 1;
            s32 const mx = gridsizex - 1;
            s32 const sy = 1;
            s32 const my = gridsizey - 1;

            // Read in the grid data
            s32 gy = sy;
            s32 gx = sx;
            for (s32 i=0; i<day11_txt_len; ++i)
            {
                u8 const c = day11_txt[i];
                if (c == '\n')
                {
                    gy++;
                    gx = sx;
                } else {
                    grid[gy*gridsizex + gx] = c - '0';
                    gx++;
                }
            }

            // Process the grid for 100 steps
            s32 total_flashes = 0;
            for (s32 step=0; step<100; ++step)
            {
                process_grid(sx,mx,sy,my,grid,gridsizex,gridsizey,total_flashes);
            }

            printf("total number of flashes %d", total_flashes);
            
            allocator->deallocate(grid);
        }

        UNITTEST_TEST(part_2)
        {
            s32 const gridsizex = count_until(day11_txt, day11_txt_len, (unsigned char)'\n');
            s32 const gridsizey = count_occurances(day11_txt, day11_txt_len, (unsigned char)'\n');

            // Adjust grid size x and y to have a border around the grid so that
            // we do not have to care about out-of-bounds.
            s32 wbgridsizex = gridsizex + 2;
            s32 wbgridsizey = gridsizey + 2;

            alloc_t* allocator = context_t::system_alloc();
            s32 const wbgridsize = wbgridsizex*wbgridsizey;
            u8* grid = (u8*)allocator->allocate(wbgridsize*sizeof(u8));
            set_all(grid, wbgridsize, (u8)255);

            s32 const sx = 1;
            s32 const mx = wbgridsizex - 1;
            s32 const sy = 1;
            s32 const my = wbgridsizey - 1;

            // Read in the grid data
            s32 gy = sy;
            s32 gx = sx;
            for (s32 i=0; i<day11_txt_len; ++i)
            {
                u8 const c = day11_txt[i];
                if (c == '\n')
                {
                    gy++;
                    gx = sx;
                } else {
                    grid[gy*wbgridsizex + gx] = c - '0';
                    gx++;
                }
            }

            // Process the grid until all cells flash at the same time
            s32 total_flashes = 0;
            s32 all_flashes_step = 0;
            for (s32 step=0; step<4096; ++step)
            {
                if (process_grid(sx,mx,sy,my,grid,wbgridsizex,wbgridsizey,total_flashes))
                {
                    all_flashes_step = step + 1;
                    break;
                }
            }

            printf("step at which all flashed %d", all_flashes_step);
            
            allocator->deallocate(grid);
        }
    }
}
UNITTEST_SUITE_END
