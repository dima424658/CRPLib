#include <algorithm>

#include "CRPLib/Entry.h"

namespace CrpLib
{
    CEntry::CEntry(eEntryID id, int index)
    {
        m_Id = id;
        m_Index = index;
    }

    void CEntry::Read(std::istream &is)
    {
        int tmp1, tmp2;
        int count;

        m_RealOffset = is.tellg();

        is.read(reinterpret_cast<char *>(&tmp1), sizeof(tmp1));
        is.read(reinterpret_cast<char *>(&tmp2), sizeof(tmp2));

        m_Flags = tmp2 & 0xFF;
        m_Length = tmp2 >> 8;

        if (m_Flags & 0x1)
        {
            // id + index
            m_Index = tmp1 & 0xFFFF;
            m_Id = static_cast<eEntryID>(tmp1 >> 16);
        }
        else
        {
            // just id
            m_Index = 0;
            m_Id = static_cast<eEntryID>(tmp1);
        }

        is.read(reinterpret_cast<char *>(&count), sizeof(count));
        is.read(reinterpret_cast<char *>(&m_Offset), sizeof(m_Offset));

        if (m_Length == 0)
        {
            // has subentries

            m_Offset <<= 4;
            m_pData = nullptr;

            is.seekg(m_RealOffset + m_Offset, std::ios::beg);

            m_SubEntries.resize(count);

            for (auto &entry : m_SubEntries)
            {
                entry = std::make_unique<CEntry>();
                entry->Read(is);
            }

            is.seekg(m_RealOffset + 0x10, std::ios::beg);
        }
        else
        {
            // has data

            is.seekg(m_RealOffset + m_Offset, std::ios::beg);

            m_pData = std::unique_ptr<ICrpData>(AllocateDataEntry(m_Id));
            m_pData->Read(is, this);

            is.seekg(m_RealOffset + 0x10, std::ios::beg);
        }
    }

    void CEntry::WriteEntry(std::ostream &os)
    {

        int tmp1, tmp2, tmp3;
        auto currOffset = os.tellp();
        int count = m_SubEntries.size();

        if (m_Flags & 0x1)
            tmp1 = m_Index | (static_cast<int>(m_Id) << 16);
        else
            tmp1 = static_cast<int>(m_Id);

        tmp2 = (m_Length << 8) | m_Flags;

        os.write(reinterpret_cast<char *>(&tmp1), sizeof(tmp1));
        os.write(reinterpret_cast<char *>(&tmp2), sizeof(tmp2));
        os.write(reinterpret_cast<char *>(&count), sizeof(count));

        if (m_Length == 0)
            tmp3 = (m_RealOffset - currOffset) >> 4;
        else
            tmp3 = m_RealOffset - currOffset;

        os.write(reinterpret_cast<char *>(&tmp3), sizeof(tmp3));
    }

    void CEntry::WriteData(std::ostream &os)
    {
        m_pData->Write(os);
    }

    // -- accessors --

    eEntryID CEntry::GetId()
    {
        return m_Id;
    }

    int CEntry::GetIndex()
    {
        return m_Index;
    }

    int CEntry::GetLength()
    {
        return m_Length;
    }

    int CEntry::GetCount()
    {
        return m_Count;
    }

    int CEntry::GetFlags()
    {
        return m_Flags;
    }

    bool CEntry::IsDataEntry()
    {
        // REDEFINED!
        // return (m_Length!=0);
        return m_Id != eEntryID::Article;
    }

    ICrpData *CEntry::GetData()
    {
        return m_pData.get();
    }

    // -- modifiers --

    void CEntry::SetLength(int length)
    {
        m_Length = length;
    }

    void CEntry::SetCount(int count)
    {
        m_Count = count;
    }

    void CEntry::SetFlags(int flags)
    {
        m_Flags = flags;
    }

    void CEntry::SetData(ICrpData *pData)
    {
        m_pData = std::unique_ptr<ICrpData>(pData);
    }

    void CEntry::SetTargetOffset(int Offset)
    {
        m_RealOffset = Offset;
    }

    // -- worker methods --

    CEntry *CEntry::GetSubEntry(int index)
    {
        return dynamic_cast<CEntry *>(m_SubEntries[index].get());
    }

    std::string CEntry::GetEntryType()
    {
        std::string tmp;
        tmp.resize(5);

        auto id = static_cast<int>(m_Id);
        tmp[3] = id & 0xFF;
        tmp[2] = (id >> 8) & 0xFF;
        tmp[1] = (id >> 16) & 0xFF;
        tmp[0] = (id >> 24) & 0xFF;

        if (tmp[0] == 0)
        {
            tmp[0] = tmp[2];
            tmp[1] = tmp[3];
            tmp[2] = 0;
        }
        else
        {
            tmp[4] = 0;
        }

        return tmp;
    }

    CEntry *CEntry::GetPartEntry(int Level, int PartIndex)
    {
        int FinIndex = (Level & 0xF) << 12;
        FinIndex |= (PartIndex & 0xFFF);

        return GetSubEntry(eEntryID::Part, FinIndex);
    }

    CEntry *CEntry::GetDataEntry(eEntryID Id, int Level, int AnimIndex, bool Damaged)
    {
        int FinIndex = (Level & 0xF);
        FinIndex |= ((AnimIndex & 0xFF) << 4);
        FinIndex |= (Damaged ? 0x8000 : 0);

        return GetSubEntry(Id, FinIndex);
    }

    CEntry *CEntry::GetSubEntry(eEntryID Id, int Index)
    {
        if (!IsDataEntry())
        {
            for (auto &it : m_SubEntries)
            {
                auto en = dynamic_cast<CEntry *>(it.get());
                if ((en->m_Id == Id) && (en->m_Index == Index))
                    return en;
            }
        }

        return nullptr;
    }

    // -- new methods --

    // firstly a helper:
    void CEntry::InsertSubEntry(CEntry *newEn)
    {
        auto Id = static_cast<int>(newEn->GetId());
        if (Id < 0x10000)
            Id = (Id << 16) | (newEn->GetIndex());

        auto result = std::find_if(m_SubEntries.begin(), m_SubEntries.end(),
                                   [Id](auto &it)
                                   {
                                       auto &en = dynamic_cast<CEntry &>(*it);

                                       auto enId = static_cast<int>(en.GetId());
                                       if (enId < 0x10000)
                                           enId = (enId << 16) | (en.GetIndex());

                                        return enId > Id;
                                   });

        m_SubEntries.emplace(result, newEn);
    }

    CEntry *CEntry::NewSubEntry(eEntryID Id, int Index, bool Allocate)
    {
        if (!IsDataEntry())
        {
            CEntry *newEn = new CEntry(Id, Index);
            int flags;

            switch (Id)
            {
            case eEntryID::Base:
                flags = 0xEA;
                break;
            case eEntryID::Name:
                flags = 0xFA;
                break;
            case eEntryID::Effect:
                flags = 0xF3;
                break;
            default:
                flags = 0xFA;
                if (static_cast<int>(Id) < 0x10000)
                    flags |= 0x01;
            }

            newEn->SetFlags(flags);

            if (Allocate)
                newEn->SetData(AllocateDataEntry(Id));

            InsertSubEntry(newEn);
            return newEn;
        }
        else
        {
            return nullptr;
        }
    }

    CEntry *CEntry::NewDataEntry(eEntryID Id, int Level, int AnimIndex, bool Damaged, bool Anim, bool Allocate)
    {
        if (!IsDataEntry())
        {
            int Index;
            Index = (Level & 0xF);
            Index |= ((AnimIndex & 0xFF) << 4);
            Index |= (Damaged ? 0x8000 : 0);

            CEntry *newEn = new CEntry(Id, Index);
            int flags = 0x2;

            if ((int)Id < 0x10000)
                flags |= 0x01;

            if (Damaged || Anim)
                flags |= 0x80;

            flags |= ((Level & 0xF) << 3);

            newEn->SetFlags(flags);

            if (Allocate)
                newEn->SetData(AllocateDataEntry(Id));

            InsertSubEntry(newEn);
            return newEn;
        }
        else
        {
            return nullptr;
        }
    }

    CEntry *CEntry::NewPartEntry(int Level, int PartIndex, bool Allocate)
    {
        if (!IsDataEntry())
        {
            int Index;
            Index = (Level & 0xF) << 12;
            Index |= (PartIndex & 0xFFF);

            CEntry *newEn = new CEntry(eEntryID::Part, Index);
            int flags = 0x3;
            flags |= ((Level & 0xF) << 3);

            newEn->SetFlags(flags);

            if (Allocate)
                newEn->SetData(AllocateDataEntry(eEntryID::Part));

            InsertSubEntry(newEn);
            return newEn;
        }
        else
        {
            return nullptr;
        }
    }
} // namespace