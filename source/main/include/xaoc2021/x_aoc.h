#ifndef __XAOC2021_H__
#define __XAOC2021_H__
#include "xbase/x_target.h"
#ifdef USE_PRAGMA_ONCE
#pragma once
#endif

#include "xbase/x_runes.h"
#include "xbase/x_stream.h"

namespace xcore
{
    class mem_stream : public istream_t
    {
        xbyte const* m_buffer;
        xsize_t      m_size;
        xsize_t      m_cursor;

    public:
        mem_stream(xbyte const* data, xsize_t length) : m_buffer(data), m_size(length), m_cursor(0) {}

    protected:
        virtual bool vcanSeek() const { return true; }
        virtual bool vcanRead() const { return true; }
        virtual bool vcanWrite() const { return false; }
        virtual bool vcanZeroCopy() const { return true; }
        virtual void vflush() {}
        virtual void vclose() {}
        virtual u64  vgetLength() const { return m_size; }
        virtual void vsetLength(u64 length) {}
        virtual s64  vsetPos(s64 pos)
        {
            m_cursor = pos;
            return m_cursor;
        }
        virtual s64 vgetPos() const { return m_cursor; }
        virtual s64 vread(xbyte* buffer, s64 count)
        {
            s64 i = 0;
            while (i < count && m_cursor < m_size)
            {
                buffer[i] = m_buffer[m_cursor++];
            }
            return i;
        }

        virtual s64 vread0(xbyte const*& buffer, s64 count)
        {
            if (m_cursor < m_size)
            {
                buffer = m_buffer + m_cursor;
                if ((m_cursor + count) > m_size)
                {
                    count = m_size - m_cursor;
                }
                m_cursor += count;
            }
            else
            {
                buffer = nullptr;
                count  = 0;
            }
            return count;
        }

        virtual s64 vwrite(const xbyte* buffer, s64 count) { return -1; }
    };

} // namespace xcore

#endif // __XAOC2021_H__