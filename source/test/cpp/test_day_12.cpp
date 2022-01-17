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
extern unsigned char day12_txt[];
extern unsigned int  day12_txt_len;

using namespace xcore;

UNITTEST_SUITE_BEGIN(day12)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        struct name_t
        {
            char m_name[8]; // The name of the cave

            void set(char const* other)
            {
                for (s32 i = 0; i < 8; ++i)
                {
                    m_name[i] = other[i];
                    if (other[i] == '\0')
                        break;
                }
            }

            bool is_equal(char const* other) const
            {
                for (s32 i = 0; i < 8; ++i)
                {
                    if (m_name[i] != other[i])
                        return false;
                    if (m_name[i] == '\0')
                        return true;
                }
                return true;
            }
        };

        struct cave_t
        {
            name_t  m_name;
            s32     m_index;      // The index of the current cave
            s32     m_num_paths;  // How many paths we can travel out of this cave
            s32     m_max_paths;  // The maximum number of paths
            s32     m_max_visits; // Maximum number of visits to this cave
            cave_t* m_paths[13];  // The paths to other caves from this cave
        };

        static bool is_large_cave(cave_t const* cave)
        {
            const char c = cave->m_name.m_name[0];
            return c >= 'A' && c <= 'Z';
        }
        static bool is_start_cave(cave_t const* cave) { return cave->m_name.is_equal("start"); }
        static bool is_end_cave(cave_t const* cave) { return cave->m_name.is_equal("end"); }

        s32 const max_caves = 32;
        struct cave_system_t
        {
            cave_t m_caves[max_caves];
            s16    m_num_caves;
            s16    m_max_caves;
            s16    m_start_cave;
            s16    m_end_cave;

            cave_t* find_or_add_cave(char const* name)
            {
                s32 i = 0;
                for (; i < m_num_caves; ++i)
                {
                    if (m_caves[i].m_name.is_equal(name))
                        return &m_caves[i];
                }

                // add a new cave
                cave_t* cave = &m_caves[i];
                cave->m_name.set(name);
                cave->m_index      = i;
                cave->m_num_paths  = 0;
                cave->m_max_paths  = 13;
                cave->m_max_visits = 1;
                if (is_large_cave(cave))
                    cave->m_max_visits = 1 << 30;

                if (is_start_cave(cave))
                    m_start_cave = cave->m_index;
                else if (is_end_cave(cave))
                    m_end_cave = cave->m_index;

                i += 1;
                return cave;
            }
        };

        struct route_t
        {
            cave_system_t const* m_cave_system;            // The cave system we are travelling through
            s32                  m_cave_visits[max_caves]; // How many times have we visited a certain cave
            s16                  m_cave;                   // Current cave we are at
            s16                  m_trace_size;             // The number of caves in our route
            s16                  m_trace[128];             // The route we have travelled
        };

        // Copy current state of route to another
        static void clone_route(route_t const& src, route_t& dst)
        {
            dst.m_cave_system = src.m_cave_system;
            for (s32 i = 0; i < max_caves; ++i)
            {
                dst.m_cave_visits[i] = src.m_cave_visits[i];
            }
            dst.m_cave       = src.m_cave;
            dst.m_trace_size = src.m_trace_size;
            for (s32 i = 0; i < src.m_trace_size; ++i)
            {
                dst.m_trace[i] = src.m_trace[i];
            }
        }

        UNITTEST_TEST(part_1) {}

        UNITTEST_TEST(part_2) {}
    }
}
UNITTEST_SUITE_END
