// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"
#include "globber.h"
#include "os.h"
#include "path.h"

//------------------------------------------------------------------------------
globber::globber(const char* pattern)
: m_files(true)
, m_directories(true)
, m_dir_suffix(true)
, m_hidden(false)
, m_dots(false)
{
    // Windows: Expand if the path to complete is drive relative (e.g. 'c:foobar')
    // Drive X's current path is stored in the environment variable "=X:"
    str<MAX_PATH> rooted;
    if (pattern[0] && pattern[1] == ':' && pattern[2] != '\\' && pattern[2] != '/')
    {
        char env_var[4] = { '=', pattern[0], ':', 0 };
        if (os::get_env(env_var, rooted))
        {
            rooted << "/";
            rooted << (pattern + 2);
            pattern = rooted.c_str();
        }
    }

    wstr<MAX_PATH> wglob(pattern);
    m_handle = FindFirstFileW(wglob.c_str(), &m_data);
    if (m_handle == INVALID_HANDLE_VALUE)
        m_handle = nullptr;

    path::get_directory(pattern, m_root);
}

//------------------------------------------------------------------------------
globber::~globber()
{
    if (m_handle != nullptr)
        FindClose(m_handle);
}

//------------------------------------------------------------------------------
bool globber::next(str_base& out, bool rooted)
{
    if (m_handle == nullptr)
        return false;

    str<MAX_PATH> file_name(m_data.cFileName);

    const wchar_t* c = m_data.cFileName;
    if (c[0] == '.' && (!c[1] || (c[1] == '.' && !c[2])) && !m_dots)
        goto skip_file;

    int attr = m_data.dwFileAttributes;
// MODE4
    if (attr & FILE_ATTRIBUTE_REPARSE_POINT)
        goto skip_file;
// MODE4

    if ((attr & FILE_ATTRIBUTE_HIDDEN) && !m_hidden)
        goto skip_file;

    if ((attr & FILE_ATTRIBUTE_DIRECTORY) && !m_directories)
        goto skip_file;

    if (!(attr & FILE_ATTRIBUTE_DIRECTORY) && !m_files)
        goto skip_file;

    out.clear();
    if (rooted)
        out << m_root;

    path::append(out, file_name.c_str());

    if (attr & FILE_ATTRIBUTE_DIRECTORY && m_dir_suffix)
        out << "\\";

    next_file();
    return true;

skip_file:
    next_file();
    return next(out, rooted);
}

//------------------------------------------------------------------------------
void globber::next_file()
{
    if (FindNextFileW(m_handle, &m_data))
        return;

    FindClose(m_handle);
    m_handle = nullptr;
}
