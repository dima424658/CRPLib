#include "CRPLib/Vector2.h"

#include "CRPLib/Entry.h"

namespace CrpLib
{
    CVector2::CVector2(tVector2 *pData, size_t count) : m_pData{pData, pData + count}
    {
    }

    void CVector2::Read(std::istream &is, ICrpEntry *entry)
    {
        m_pData.resize(dynamic_cast<CEntry *>(entry)->GetCount());

        is.read(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    void CVector2::Write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    int CVector2::GetEntryLength()
    {
        return sizeof(decltype(m_pData)::value_type) * m_pData.size();
    }

    int CVector2::GetEntryCount()
    {
        return m_pData.size();
    }

    // Accessors and modifiers
    void CVector2::SetItem(size_t index, tVector2 value)
    {
        m_pData[index] = std::move(value);
    }

    tVector2 &CVector2::GetItem(size_t index)
    {
        return m_pData[index];
    }

    size_t CVector2::GetCount()
    {
        return m_pData.size();
    }

    void CVector2::SetCount(size_t count)
    {
        m_pData.resize(count);
    }
} // namespace CrpLib