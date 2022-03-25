#pragma once

#include <vector>
#include <istream>
#include <ostream>
#include <string_view>

#include "CRPLib/Entry.h"

namespace CrpLib
{
    constexpr std::string_view cCRPGeneratedBy{"Generated by CrpLib (built " __DATE__ "). Copyright(C) 2004, Arushan/AruTec Inc."};

    class CCrpFile
    {
    private:
        eHeaderID m_Id = eHeaderID::Car;
        int m_Flags    = 0x1A;
        int m_Offset   = 0x10;

        std::vector<std::unique_ptr<ICrpEntry>> m_Articles;
        std::vector<std::unique_ptr<ICrpEntry>> m_Misc;

        void InsertMiscEntry(CEntry *newEn);

    public:
        void Read(std::istream &is);
        void Write(std::ostream &os);

        // -- accessors --
        size_t GetArticleCount();
        CEntry *GetArticle(size_t index);

        size_t GetMiscCount();
        CEntry *GetMisc(size_t index);
        CEntry *GetMisc(eEntryID id, size_t index = 0);
        int FindMisc(eEntryID id, size_t index = 0);

        int GetFlags();
        void SetFlags(int flags);

        eHeaderID GetId();
        void SetId(eHeaderID id);

        // -- new methods --
        CEntry *NewArticle();
        CEntry *NewMisc(eEntryID id, size_t index = 0, bool Allocate = true);
    };

} // namespace CrpLib
