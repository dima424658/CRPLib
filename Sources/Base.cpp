#include "CRPLib/Base.h"

namespace CrpLib
{
    void CBase::Read(std::istream &is, ICrpEntry *entry)
    {
        int levelCount;

        is.read(reinterpret_cast<char *>(&m_Flags), sizeof(m_Flags));
        is.read(reinterpret_cast<char *>(&m_Zero1), sizeof(m_Zero1));
        is.read(reinterpret_cast<char *>(&m_Zero2), sizeof(m_Zero2));
        is.read(reinterpret_cast<char *>(&levelCount), sizeof(levelCount));

        is.read(reinterpret_cast<char *>(&m_HasFloats), sizeof(m_HasFloats));
        is.read(reinterpret_cast<char *>(m_pFloats), sizeof(m_pFloats));

        is.read(reinterpret_cast<char *>(&m_BaseInfo), sizeof(m_BaseInfo));

        is.read(reinterpret_cast<char *>(&m_Unk), sizeof(m_Unk));

        m_pLevMasks.resize(levelCount);

        is.read(reinterpret_cast<char *>(m_pLevMasks.data()), sizeof(decltype(m_pLevMasks)::value_type) * m_pLevMasks.size());
    }

    void CBase::Write(std::ostream &os)
    {
        int levelCount = m_pLevMasks.size();

        os.write(reinterpret_cast<char *>(&m_Flags), sizeof(m_Flags));
        os.write(reinterpret_cast<char *>(&m_Zero1), sizeof(m_Zero1));
        os.write(reinterpret_cast<char *>(&m_Zero2), sizeof(m_Zero2));
        os.write(reinterpret_cast<char *>(&levelCount), sizeof(levelCount));

        os.write(reinterpret_cast<char *>(&m_HasFloats), sizeof(m_HasFloats));
        os.write(reinterpret_cast<char *>(m_pFloats), sizeof(m_pFloats));

        os.write(reinterpret_cast<char *>(&m_BaseInfo), sizeof(m_BaseInfo));

        os.write(reinterpret_cast<char *>(&m_Unk), sizeof(m_Unk));

        os.write(reinterpret_cast<char *>(m_pLevMasks.data()), sizeof(decltype(m_pLevMasks)::value_type) * m_pLevMasks.size());
    }

    int CBase::GetEntryLength()
    {
        return sizeof(m_Flags) + sizeof(m_Zero1) + sizeof(m_Zero2) + sizeof(int) /* level count */ + sizeof(m_HasFloats) + sizeof(m_pFloats) + sizeof(m_BaseInfo) + sizeof(m_Unk) +
               sizeof(decltype(m_pLevMasks)::value_type) * m_pLevMasks.size();
    }

    int CBase::GetEntryCount()
    {
        return 0x0;
    }

    void CBase::CreateLevels(int LevCount, int Start)
    {
        m_pLevMasks.resize(LevCount);

        for (int i = 0; i < LevCount; ++i)
            m_pLevMasks[i].SetLevel(i + Start);
    }

    int CBase::GetLevelCount()
    {
        return m_pLevMasks.size();
    }

    tLevelMask &CBase::GetLevel(size_t i)
    {
        return m_pLevMasks[i];
    }

    BaseFlags CBase::GetFlags()
    {
        return m_Flags;
    }

    void CBase::SetFlags(BaseFlags flags)
    {
        m_Flags = flags;
        m_HasFloats = (flags == BaseFlags::Effect) ? 1 : 0;
    }

    void CBase::SetHasFloats(bool val)
    {
        m_HasFloats = val ? 1 : 0;
    }

    float *CBase::GetFloats()
    {
        return m_HasFloats ? m_pFloats : nullptr;
    }

    tBaseInfo &CBase::GetBaseInfo()
    {
        return m_BaseInfo;
    }
} // namespace CrpLib