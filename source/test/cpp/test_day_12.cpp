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

            const char* get() const
            {
                return m_name;
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
            name_t m_name;
            s8     m_index;      // The index of the current cave
            s8     m_num_paths;  // How many paths we can travel out of this cave
            s8     m_max_paths;  // The maximum number of paths
            s8     m_max_visits; // Maximum number of visits to this cave
            s8     m_path[32];   // The paths to other caves from this cave
        };

        static bool is_large_cave(cave_t const* cave)
        {
            const char c = cave->m_name.m_name[0];
            return c >= 'A' && c <= 'Z';
        }

        void initialize(cave_t * c, s8 i, const char* name)
        {
            c->m_name.set(name);
            c->m_index      = i;
            c->m_num_paths  = 0;
            c->m_max_paths  = 32;
            c->m_max_visits = 1;
            for (s32 i = 0; i < c->m_max_paths; ++i)
                c->m_path[i] = -1;
            if (is_large_cave(c))
                c->m_max_visits = -1;
        }

        void add_path(cave_t * cave, cave_t * dst)
        {
            cave->m_path[cave->m_num_paths] = dst->m_index;
            cave->m_num_paths += 1;
        }

        static bool is_start_cave(cave_t const* cave) { return cave->m_name.is_equal("start"); }
        static bool is_end_cave(cave_t const* cave) { return cave->m_name.is_equal("end"); }

        s32 const max_caves = 32;
        struct cave_system_t
        {
            cave_t m_cave[max_caves];
            u8     m_num_caves;
            u8     m_max_caves;
            s8     m_start_cave;
            s8     m_end_cave;
        };

        void initialize(cave_system_t * cs)
        {
            cs->m_num_caves  = 0;
            cs->m_max_caves  = max_caves;
            cs->m_start_cave = -1;
            cs->m_end_cave   = -1;
            for (s32 i = 0; i < cs->m_max_caves; ++i)
                initialize(&cs->m_cave[i], i, "");
        }

        cave_t* get_start_cave(cave_system_t * cs)
        {
            if (cs->m_start_cave == -1)
                return nullptr;
            return &cs->m_cave[cs->m_start_cave];
        }

        cave_t* get_end_cave(cave_system_t * cs)
        {
            if (cs->m_end_cave == -1)
                return nullptr;
            return &cs->m_cave[cs->m_end_cave];
        }

        cave_t* get_cave(cave_system_t * cs, s8 i) { return &cs->m_cave[i]; }

        cave_t* find_or_add_cave(cave_system_t * cs, char const* name)
        {
            s32 i = 0;
            for (; i < cs->m_num_caves; ++i)
            {
                if (cs->m_cave[i].m_name.is_equal(name))
                    return &cs->m_cave[i];
            }

            // add a new cave
            cave_t* cave = &cs->m_cave[i];
            initialize(cave, i, name);

            if (is_start_cave(cave))
                cs->m_start_cave = cave->m_index;
            else if (is_end_cave(cave))
                cs->m_end_cave = cave->m_index;

            cs->m_num_caves += 1;
            return cave;
        }

        static void parse_cave_system(cave_system_t * cs)
        {
            char    name[8];
            s32     name_len = 0;
            cave_t* path[2];
            s32     cursor = 0;
            while (cursor < day12_txt_len)
            {
                char c = day12_txt[cursor++];
                if (c == '\n' || c == '\r')
                {
                    if (name_len > 0)
                    {
                        path[1] = find_or_add_cave(cs, name);
                        add_path(path[0], path[1]);
                        add_path(path[1], path[0]);
                        name_len = 0;
                    }
                }
                else if (c == '-')
                {
                    path[0]  = find_or_add_cave(cs, name);
                    name_len = 0;
                }
                else
                {
                    name[name_len++] = c;
                    name[name_len]   = '\0';
                }
            }
        }

        struct route_t
        {
            s16 m_cave;                   // Current cave we are at
            s16 m_length;                 // The number of caves in our route
            s8  m_cave_visits[max_caves]; // How many times have we visited a certain cave
            s8  m_trace[512];             // The route we have travelled
        };

        static void route_init(cave_system_t * cs, route_t * r)
        {
            r->m_cave   = -1;
            r->m_length = 0;
            for (s32 i = 0; i < max_caves; ++i)
                r->m_cave_visits[i] = cs->m_cave[i].m_max_visits;
        }

        static void route_visit(route_t * r, cave_t * c)
        {
            r->m_cave               = c->m_index;
            r->m_trace[r->m_length] = c->m_index;
            r->m_length += 1;
            ASSERT(r->m_length < 512);

            if (r->m_cave_visits[c->m_index] > 0)
                r->m_cave_visits[c->m_index] -= 1;
        }

		static void route_print(cave_system_t * cs, route_t* r)
		{
            for (s32 i = 0; i < r->m_length; ++i)
            {
                cave_t* c = get_cave(cs, r->m_trace[i]);
				printf("%s", c->m_name.get());
                if (i < r->m_length - 1)
                    printf("-");
            }
            printf("\n");
		}

        static bool route_can_visit(route_t * r, cave_t * c)
        {
            if (r->m_cave_visits[c->m_index] < 0)
                return true;
            if (r->m_cave_visits[c->m_index] > 0)
                return true;
            return false;
        }

        // Copy current state of route to another
        static void route_copy(route_t const* src, route_t* dst)
        {
            dst->m_cave       = src->m_cave;
            dst->m_length = src->m_length;
            for (s32 i = 0; i < max_caves; ++i)
                dst->m_cave_visits[i] = src->m_cave_visits[i];
            for (s32 i = 0; i < src->m_length; ++i)
                dst->m_trace[i] = src->m_trace[i];
        }

        struct routes_t
        {
            s16     m_num;
            s16     m_max;
            route_t m_route[16384];
        };

        static void routes_init(cave_system_t * cs, routes_t * routes)
        {
            routes->m_num = 0;
            routes->m_max = 16384;
        }

		static route_t* routes_new(routes_t * rs)
		{
			route_t* r = &rs->m_route[rs->m_num];
			rs->m_num += 1;
			return r;
		}

        static void routes_add(routes_t * rs, route_t * r)
        {
            route_copy(r, &rs->m_route[rs->m_num]);
            rs->m_num += 1;
            ASSERT(rs->m_num < rs->m_max);
        }

        static void routes_remove(routes_t * rs, s16 i)
        {
            if (i < (rs->m_num - 1))
                route_copy(&rs->m_route[rs->m_num - 1], &rs->m_route[i]);
            rs->m_num -= 1;
        }

        static void traverse_cave_system(cave_system_t * cs, routes_t * final_routes)
        {
			routes_t* open_routes = (routes_t*)context_t::system_alloc()->allocate(sizeof(routes_t));
			routes_init(cs, open_routes);

            // start the route at the start cave and for all possible paths from this cave
            // start a new route
            cave_t* start = get_start_cave(cs);
            cave_t* end   = get_end_cave(cs);

            route_t start_route;
            route_init(cs, &start_route);
            route_visit(&start_route, start);
            routes_add(open_routes, &start_route);
            while (open_routes->m_num > 0)
            {
                s16 const n = open_routes->m_num - 1;
                route_t* r = &open_routes->m_route[n];
                cave_t* c = &cs->m_cave[r->m_cave];

                // if we have reached the end cave, add this route to the final routes
                if (c->m_index == end->m_index)
                {
                    routes_add(final_routes, r);
                }
                else
                {
                    // for each possible path from this cave
                    for (s32 i = 0; i < c->m_num_paths; ++i)
                    {
                        cave_t* next = &cs->m_cave[c->m_path[i]];
                        if (route_can_visit(r, next))
                        {
							route_t* new_route = routes_new(open_routes);
							route_copy(r, new_route);
                            route_visit(new_route, next);
                        }
                    }
                }
                routes_remove(open_routes, n);
            }

			context_t::system_alloc()->deallocate(open_routes);
		}

        UNITTEST_TEST(part_1)
        {
            cave_system_t cs;
            initialize(&cs);
            parse_cave_system(&cs);

			routes_t* final_routes = (routes_t*)context_t::system_alloc()->allocate(sizeof(routes_t));
            routes_init(&cs, final_routes);
            traverse_cave_system(&cs, final_routes);

            printf("number of final routes %d\n", final_routes->m_num);
            //for (s32 i = 0; i < final_routes->m_num; ++i)
            //{
            //    route_print(&cs, &final_routes->m_route[i]);
            //}

			context_t::system_alloc()->deallocate(final_routes);
        }

        UNITTEST_TEST(part_2) {}
    }
}
UNITTEST_SUITE_END
