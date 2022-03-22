#include <algorithm>

#include "CRPLib/CrpFile.h"

template <typename T, typename U>
constexpr auto alignOffset(T x, U a)
{
    return (x % a == 0) ? x : (x + (a - x % a));
}

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
            article = std::make_unique<CEntry>();
            article->Read(is);
        }

        m_Misc.resize(miscCount);
        for (auto &misc : m_Misc)
        {
            misc = std::make_unique<CEntry>();
            misc->Read(is);
        }
    }

    void CCrpFile::Write(std::ostream &os)
    {
        // verify count/length for all!
        for (auto &it : m_Articles)
        {
            auto &article = dynamic_cast<CEntry &>(*it);

            for (int i = 0; i < article.GetCount(); ++i)
            {
                auto en = article.GetSubEntry(i);
                auto data = en->GetData();

                if (data->GetEntryCount() != -1)
                    en->SetCount(data->GetEntryCount());

                if (data->GetEntryLength() != -1)
                    en->SetLength(data->GetEntryLength());
            }
        }

        for (auto &it : m_Misc)
        {
            auto& entry = dynamic_cast<CEntry &>(*it);
            auto data = entry.GetData();

            if (data->GetEntryCount() != -1)
                entry.SetCount(data->GetEntryCount());

            if (data->GetEntryLength() != -1)
                entry.SetLength(data->GetEntryLength());
        }

#ifdef SAVE_PROTECT
        m_Offs = (m_Articles.size() + 1) << 4;
#endif

        // write the header
#ifdef SAVE_PROTECT
        int newId = (int)m_Id ^ 0x20;
        os.write(reinterpret_cast<char *>(&newId), sizeof(newId));
#else
        os.write(reinterpret_cast<char *>(&m_Id), sizeof(m_Id));
#endif
        int tmp = m_Flags | (m_Articles.size() << 5);
        os.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));

        int miscCount = m_Misc.size();
        os.write(reinterpret_cast<char *>(&miscCount), sizeof(miscCount));
        tmp = m_Offset >> 4;
        os.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));

#ifdef SAVE_PROTECT
        for (int i = 0; i < m_Articles.size(); i++)
        {
            tmp = ID_ARTI;
            os.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            tmp = 0x1A;
            os.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            tmp = 3 * (i + 1) * (m_Misc.size() + m_Articles.size()) / 2 + 5;
            os.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            tmp = ((m_Misc.size() * 3 + m_Articles.size() + 6) * 43212) ^ (0x85850923 * i);
            tmp &= (((i) << 5) - 1);
            os.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        }
#endif

        os.seekp(m_Offset, std::ios::beg);

        int count = m_Articles.size() + m_Misc.size();

        for (auto &it : m_Articles)
            count += dynamic_cast<CEntry &>(*it).GetCount();

        size_t dataOffset = m_Offset + count * 0x10 + 0x10;
        os.seekp(dataOffset, std::ios::beg);
        os.write(cCRPGeneratedBy.data(), cCRPGeneratedBy.size());

        dataOffset = alignOffset(os.tellp(), 0x100);

        size_t entryOffset = m_Offset + (m_Articles.size() + m_Misc.size()) * 0x10;
        os.seekp(entryOffset, std::ios::beg);

        for (auto &it : m_Articles)
        {
            auto &article = dynamic_cast<CEntry &>(*it);

            article.SetTargetOffset(entryOffset);
            for (int i = 0; i < article.GetCount(); ++i)
            {
                auto& entry = dynamic_cast<CEntry&>(*article.GetSubEntry(i));

                entry.SetTargetOffset(dataOffset);

                // Write the data
                os.seekp(dataOffset, std::ios::beg);
                entry.WriteData(os);
                dataOffset = alignOffset(os.tellp(), 0x10);

                // Write the entry
                os.seekp(entryOffset, std::ios::beg);
                entry.WriteEntry(os);
                entryOffset += 0x10;
            }
            dataOffset = alignOffset(dataOffset, 0x100);
        }

        os.seekp(m_Offset, std::ios::beg);

        for (auto &it : m_Articles)
        {
            auto &article = dynamic_cast<CEntry &>(*it);
            article.WriteEntry(os);
        }

        entryOffset = os.tellp();

        for (auto &it : m_Misc)
        {
            auto &entry = dynamic_cast<CEntry &>(*it);
            entry.SetTargetOffset(dataOffset);

            os.seekp(dataOffset, std::ios::beg);
            entry.WriteData(os);
            dataOffset = alignOffset(os.tellp(), 0x10);

            os.seekp(entryOffset, std::ios::beg);
            entry.WriteEntry(os);
            entryOffset += 0x10;
        }

        dataOffset = alignOffset(dataOffset, 0x100);
        os.seekp(dataOffset - 1, std::ios::beg);
        os.write("\0", 1);
    }

    // -- accessors --

    size_t CCrpFile::GetArticleCount()
    {
        return m_Articles.size();
    }

    CEntry *CCrpFile::GetArticle(size_t index)
    {
        if (index < m_Articles.size())
            return dynamic_cast<CEntry *>(m_Articles[index].get());
        else
            return nullptr;
    }

    size_t CCrpFile::GetMiscCount()
    {
        return m_Misc.size();
    }

    CEntry *CCrpFile::GetMisc(size_t index)
    {
        if (index < m_Misc.size())
            return dynamic_cast<CEntry *>(m_Misc[index].get());
        else
            return nullptr;
    }

    CEntry *CCrpFile::GetMisc(eEntryID id, size_t index)
    {
        auto result = std::find_if(m_Misc.begin(), m_Misc.end(),
                                   [id, index](auto &it)
                                   {
                                       auto &entry = dynamic_cast<CEntry&>(*it);
                                       return entry.GetId() == id && entry.GetIndex() == index;
                                   });

        if (result == m_Misc.end())
            return nullptr;
        else
            return dynamic_cast<CEntry *>(result->get());
    }

    int CCrpFile::FindMisc(eEntryID id, size_t index)
    {
        auto result = std::find_if(m_Misc.begin(), m_Misc.end(),
                                   [id, index](auto &it)
                                   {
                                       auto &entry = dynamic_cast<CEntry&>(*it);
                                       return entry.GetId() == id && entry.GetIndex() == index;
                                   });

        if (result == m_Misc.end())
            return -1;
        else
            return std::distance(m_Misc.begin(), result);
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

        auto it = std::find_if(m_Misc.begin(), m_Misc.end(),
                               [Id](const auto &it)
                               {
                                   auto &entry = dynamic_cast<CEntry &>(*it);
                                   auto enId = static_cast<int>(entry.GetId());
                                   if (enId < 0x10000)
                                       enId = (enId << 16) | (entry.GetIndex());

                                   return enId > Id;
                               });

        m_Misc.emplace(it, newEn);
    }

    CEntry *CCrpFile::NewArticle()
    {
        auto entry = new CEntry{eEntryID::Article, 0};
        entry->SetFlags(0x1A);
        m_Articles.emplace_back(entry);

        return entry;
    }

    CEntry *CCrpFile::NewMisc(eEntryID id, size_t index, bool Allocate)
    {
        auto entry = new CEntry{id, index};

        int flags = 0xFA;
        if (static_cast<int>(id) < 0x10000)
            flags |= 0x01;

        entry->SetFlags(flags);

        if (Allocate)
            entry->SetData(AllocateDataEntry(id));

        InsertMiscEntry(entry);

        return entry;
    }
} // namespace CrpLib