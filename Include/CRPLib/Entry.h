#pragma once

#include "CRPLib/Common.h"
#include "CRPLib/Interfaces.h"

#include <vector>
#include <fstream>
#include <memory>

namespace CrpLib
{
    class CEntry : public ICrpEntry
    {
    private:
        eEntryID m_Id{};
        int m_Index = 0, m_Flags, m_Length = 0, m_Count = 0, m_Offset;
        size_t m_RealOffset;

        std::vector<std::unique_ptr<ICrpEntry>> m_SubEntries;
        std::unique_ptr<ICrpData> m_pData;

        void InsertSubEntry(CEntry *newEn);

    public:
        CEntry() = default;
        CEntry(eEntryID id, int index);

        void Read(std::istream &is) override;

        void WriteEntry(std::ostream &os) override;
        void WriteData(std::ostream &os) override;

        // -- accessors --

        eEntryID GetId();

        int GetIndex();

        int GetLength();

        int GetCount();

        int GetFlags();

        bool IsDataEntry();

        ICrpData *GetData();

        // -- modifiers --

        void SetLength(int length);

        void SetCount(int count);

        void SetFlags(int flags);

        void SetData(ICrpData *pData);

        void SetTargetOffset(int Offs);

        // -- worker methods --

        CEntry *GetSubEntry(int index);

        std::string GetEntryType();

        CEntry *GetPartEntry(int Level, int PartIndex);

        CEntry *GetDataEntry(eEntryID Id, int Level, int AnimIndex = 0, bool Damaged = false);

        CEntry *GetSubEntry(eEntryID Id, int Index = 0);

        // -- new methods --

        CEntry *NewSubEntry(eEntryID Id, int Index = 0, bool Allocate = true);

        CEntry *NewDataEntry(eEntryID Id, int Level, int AnimIndex = 0, bool Damaged = false, bool Anim = false,
                             bool Allocate = true);

        CEntry *NewPartEntry(int Level, int PartIndex, bool Allocate = true);
    };

}
