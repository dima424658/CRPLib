#include <algorithm>

#include "CRPLib/CrpFile.h"

namespace CrpLib
{
    void CCrpFile::Read(std::istream &is)
    {
        int tmp, articleCount, miscCount;

        is.read(reinterpret_cast<char *>(&m_Id), sizeof(m_Id));

        if (m_Id != eHeaderID::Car && m_Id != eHeaderID::Track)
            throw std::runtime_error{"Unknown header ID"};

        is.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));

        m_Flags = tmp & 0x1F;
        articleCount = tmp >> 5;

        is.read(reinterpret_cast<char *>(&miscCount), sizeof(miscCount));

        is.read(reinterpret_cast<char *>(&m_Offset), sizeof(m_Offset));
        m_Offset <<= 4;

        is.seekg(m_Offset, std::ios::beg);

        m_Articles.resize(articleCount);
        for (auto &article : m_Articles)
        {
            article = new CEntry();
            article->Read(is);
        }

        m_Misc.resize(miscCount);
        for (auto &misc : m_Misc)
        {
            misc = new CEntry();
            misc->Read(is);
        }
    }

    void CCrpFile::Write(std::ostream &os)
    {
#ifdef SAVE_COMPRESS
        CMemFile file(131072);
#else
        std::fstream file;
#endif

        int tmp, count, entryOffset, dataOffset;

        // verify count/length for all!
        for (auto &it : m_Articles)
        {
            auto &arti = dynamic_cast<CEntry &>(*it);

            for (int j = 0; j < arti.GetCount(); j++)
            {
                CEntry *en = arti.GetSubEntry(j);
                ICrpData *data = en->GetData();

                if (data->GetEntryCount() != -1)
                    en->SetCount(data->GetEntryCount());

                if (data->GetEntryLength() != -1)
                    en->SetLength(data->GetEntryLength());
            }
        }

        for (int i = 0; i < m_Misc.size(); i++)
        {
            CEntry *en = GetMisc(i);
            ICrpData *data = en->GetData();
            if (data->GetEntryCount() != -1)
                en->SetCount(data->GetEntryCount());
            if (data->GetEntryLength() != -1)
                en->SetLength(data->GetEntryLength());
        }

#ifdef SAVE_PROTECT
        m_Offs = (m_Articles.size() + 1) << 4;
#endif

        // write the header
#ifdef SAVE_PROTECT
        int newId = (int)m_Id ^ 0x20;
        file.Write(&newId, 4);
#else
        file.write((char *)&m_Id, 4);
#endif
        tmp = m_Flags | (m_Articles.size() << 5);
        file.write((char *)&tmp, 4);

        int miscCount = m_Misc.size();
        file.write((char *)&miscCount, 4);
        tmp = m_Offset >> 4;
        file.write((char *)&tmp, 4);

#ifdef SAVE_PROTECT
        for (int i = 0; i < m_Articles.size(); i++)
        {
            tmp = ID_ARTI;
            file.Write(&tmp, 4);
            tmp = 0x1A;
            file.Write(&tmp, 4);
            tmp = 3 * (i + 1) * (m_Misc.size() + m_Articles.size()) / 2 + 5;
            file.Write(&tmp, 4);
            tmp = ((m_Misc.size() * 3 + m_Articles.size() + 6) * 43212) ^ (0x85850923 * i);
            tmp &= (((i) << 5) - 1);
            file.Write(&tmp, 4);
        }
#endif

        file.seekg(m_Offset, std::ios::beg);

        count = m_Articles.size() + m_Misc.size();
        for (int i = 0; i < m_Articles.size(); i++)
            count += ((CEntry *)m_Articles[i])->GetCount();

        dataOffset = m_Offset + count * 0x10 + 0x10;
        file.seekg(dataOffset, std::ios::beg);
        file.write(cCRPGeneratedBy.data(), cCRPGeneratedBy.size());

        dataOffset = ALIGN_OFFS((int)file.tellg(), 0x100);

        entryOffset = m_Offset + (m_Articles.size() + m_Misc.size()) * 0x10;
        file.seekg(entryOffset, std::ios::beg);

        for (int i = 0; i < m_Articles.size(); i++)
        {
            CEntry *arti = (CEntry *)m_Articles[i];
            arti->SetTargetOffs(entryOffset);
            for (int j = 0; j < arti->GetCount(); j++)
            {
                CEntry *en = (CEntry *)(arti->GetSubEntry(j));

                en->SetTargetOffs(dataOffset);

                // write the data
                file.seekg(dataOffset, std::ios::beg);
                en->WriteData(file);
                dataOffset = ALIGN_OFFS((int)file.tellg(), 0x10);

                // write the entry
                file.seekg(entryOffset, std::ios::beg);
                en->WriteEntry(file);
                entryOffset += 0x10;
            }
            dataOffset = ALIGN_OFFS(dataOffset, 0x100);
        }

        file.seekg(m_Offset, std::ios::beg);

        for (int i = 0; i < m_Articles.size(); i++)
        {
            CEntry *arti = (CEntry *)m_Articles[i];
            arti->WriteEntry(file);
        }

        entryOffset = (int)file.tellg();
        for (int i = 0; i < m_Misc.size(); i++)
        {
            CEntry *en = (CEntry *)m_Misc[i];
            en->SetTargetOffs(dataOffset);

            file.seekg(dataOffset, std::ios::beg);
            en->WriteData(file);
            dataOffset = ALIGN_OFFS((int)file.tellg(), 0x10);

            file.seekg(entryOffset, std::ios::beg);
            en->WriteEntry(file);
            entryOffset += 0x10;
        }

        dataOffset = ALIGN_OFFS(dataOffset, 0x100);
        file.seekg(dataOffset - 1, std::ios::beg);
        file.write("\0", 1);

#ifdef SAVE_COMPRESS
        unsigned int fLength = (unsigned int)file.GetLength();
        unsigned char *fData = file.Detach();
        CompressSave(fData, fLength, (char *)(LPCTSTR)filename);
#endif
    }

    // -- accessors --

    int CCrpFile::GetArticleCount()
    {
        return m_Articles.size();
    }

    CEntry *CCrpFile::GetArticle(int index)
    {
        if (index < m_Articles.size())
            return dynamic_cast<CEntry *>(m_Articles[index]);
        else
            return nullptr;
    }

    int CCrpFile::GetMiscCount()
    {
        return m_Misc.size();
    }

    CEntry *CCrpFile::GetMisc(int index)
    {
        if (index < m_Misc.size())
            return dynamic_cast<CEntry *>(m_Misc[index]);
        else
            return nullptr;
    }

    CEntry *CCrpFile::GetMisc(eEntryID id, int index)
    {
        auto result = std::find_if(m_Misc.begin(), m_Misc.end(),
                                   [id, index](auto &it)
                                   {
                                       CEntry *en = dynamic_cast<CEntry *>(it);
                                       return en->GetId() == id && en->GetIndex() == index;
                                   });

        if (result == m_Misc.end())
            return nullptr;
        else
            return dynamic_cast<CEntry *>(*result.base());
    }

    int CCrpFile::FindMisc(eEntryID id, int index)
    {
        for (int i = 0; i < m_Misc.size(); i++)
        {
            CEntry *en = (CEntry *)m_Misc[i];
            if (en->GetId() == id && en->GetIndex() == index)
                return i;
        }
        return -1;
    }

    int CCrpFile::GetFlags()
    {
        return m_Flags;
    }

    void CCrpFile::SetFlags(int flags)
    {
        m_Flags = flags;
    }

    eHeaderID CCrpFile::GetId()
    {
        return m_Id;
    }

    void CCrpFile::SetId(eHeaderID id)
    {
        m_Id = id;
    }

    // new methods

    // firstly a helper:
    void CCrpFile::InsertMiscEntry(CEntry *newEn)
    {
        auto Id = static_cast<int>(newEn->GetId());
        if (Id < 0x10000)
            Id = (Id << 16) | (newEn->GetIndex());

        bool inserted = false;

        for (int i = 0; i < m_Misc.size(); i++)
        {
            CEntry *en = (CEntry *)m_Misc[i];
            auto enId = static_cast<int>(en->GetId());
            if (enId < 0x10000)
                enId = (enId << 16) | (en->GetIndex());

            if (enId > Id)
            {
                // insert before i
                inserted = true;
                m_Misc.insert(m_Misc.begin() + i, newEn);
                break;
            }
        }

        if (!inserted)
        {
            // last entry
            m_Misc.push_back(newEn);
        }
    }

    CEntry *CCrpFile::NewArticle(void)
    {
        CEntry *en = new CEntry(eEntryID::Article, 0);
        en->SetFlags(0x1A);
        m_Articles.push_back(en);

        return en;
    }

    CEntry *CCrpFile::NewMisc(eEntryID id, int index, bool Allocate)
    {
        CEntry *en = new CEntry(id, index);

        int flags = 0xFA;
        if ((int)id < 0x10000)
            flags |= 0x01;
        en->SetFlags(flags);

        if (Allocate)
        {
            en->SetData(AllocateDataEntry(id));
        }

        InsertMiscEntry(en);

        return en;
    }
} // namespace CrpLib
