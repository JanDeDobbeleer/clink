// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"

#include "core/str.h"
#include "core/settings.h"
#include "core/os.h"

#include <list>
#include <assert.h>

//------------------------------------------------------------------------------
extern bool g_force_load_debugger;

//------------------------------------------------------------------------------
void host_cmd_enqueue_lines(std::list<str_moveable>& lines)
{
    assert(false);
}

//------------------------------------------------------------------------------
bool host_has_deprecated_argmatcher(const char* command)
{
    return false;
}

//------------------------------------------------------------------------------
void start_logger()
{
    assert(false);
}

//------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    argc--, argv++;

    bool timer = false;

#ifdef DEBUG
    settings::TEST_set_ever_loaded();
#endif

    os::set_shellname(L"clink_test_harness");

    while (argc > 0)
    {
        if (!strcmp(argv[0], "-?") || !strcmp(argv[0], "--help"))
        {
            puts("Options:\n"
                 "  -?        Show this help.\n"
                 "  -d        Load Lua debugger.\n"
                 "  -t        Show execution time.");
            return 1;
        }
        else if (!strcmp(argv[0], "-d"))
        {
            g_force_load_debugger = true;
        }
        else if (!strcmp(argv[0], "-t"))
        {
            timer = true;
        }
        else if (!strcmp(argv[0], "--"))
        {
        }
        else
        {
            break;
        }

        argc--, argv++;
    }

    DWORD start = GetTickCount();

    clatch::colors::initialize();

    const char* prefix = (argc > 0) ? argv[0] : "";
    int result = (clatch::run(prefix) != true);

    if (timer)
    {
        DWORD elapsed = GetTickCount() - start;
        printf("\nElapsed time %u.%03u seconds.\n", elapsed / 1000, elapsed % 1000);
    }

    return result;
}
