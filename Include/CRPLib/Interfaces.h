#pragma once

#include <istream>
#include <ostream>

namespace CrpLib
{
    class ICrpEntry
    {
    public:
        virtual ~ICrpEntry() = 0;

        virtual void Read(std::istream &is) = 0;

        virtual void WriteEntry(std::ostream &os) = 0;
        virtual void WriteData(std::ostream &os) = 0;
    };

    class ICrpData
    {
    public:
        virtual ~ICrpData() = 0;

        virtual void Read(std::istream &is, ICrpEntry *entry) = 0;
        virtual void Write(std::ostream &os) = 0;

        virtual int GetEntryLength() = 0;
        virtual int GetEntryCount()  = 0;
    };
} // namespace CrpLib