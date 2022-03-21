#include "CRPLib/Vector4.h"

#include "CRPLib/Entry.h"

namespace CrpLib
{
    CVector4::CVector4(tVector4 *pData, size_t count) : m_pData{pData, pData + count}
    {
    }

    void CVector4::Read(std::istream &is, ICrpEntry *entry)
    {
        m_pData.resize(dynamic_cast<CEntry *>(entry)->GetCount());

        is.read(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    void CVector4::Write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    int CVector4::GetEntryLength()
    {
        return sizeof(decltype(m_pData)::value_type) * m_pData.size();
    }

    int CVector4::GetEntryCount()
    {
        return m_pData.size();
    }

    // Accessors and modifiers
    void CVector4::SetItem(size_t index, tVector4 value)
    {
        m_pData[index] = std::move(value);
    }

    tVector4 &CVector4::GetItem(size_t index)
    {
        return m_pData[index];
    }

    size_t CVector4::GetCount()
    {
        return m_pData.size();
    }

    void CVector4::SetCount(size_t count)
    {
        m_pData.resize(count);
    }
} // namespace CrpLib