#include "CRPLib/Material.h"

namespace CrpLib
{
    void CMaterial::Read(std::istream &is, ICrpEntry *entry)
    {
        is.read(reinterpret_cast<char *>(m_pData1), sizeof(m_pData1));
        is.read(reinterpret_cast<char *>(m_pRMthName), sizeof(m_pRMthName));
        is.read(reinterpret_cast<char *>(m_pData2), sizeof(m_pData2));
        is.read(reinterpret_cast<char *>(&m_TpgIndex), sizeof(m_TpgIndex));
        is.read(reinterpret_cast<char *>(m_pData3), sizeof(m_pData3));
    }

    void CMaterial::Write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(m_pData1), sizeof(m_pData1));
        os.write(reinterpret_cast<char *>(m_pRMthName), sizeof(m_pRMthName));
        os.write(reinterpret_cast<char *>(m_pData2), sizeof(m_pData2));
        os.write(reinterpret_cast<char *>(&m_TpgIndex), sizeof(m_TpgIndex));
        os.write(reinterpret_cast<char *>(m_pData3), sizeof(m_pData3));
    }

    int CMaterial::GetEntryLength()
    {
        return sizeof(m_pData1) + sizeof(m_pRMthName) + sizeof(m_pData2) + sizeof(m_TpgIndex) + sizeof(m_pData3);
    }

    int CMaterial::GetEntryCount()
    {
        return 0x34;
    }

    int CMaterial::GetTpgIndex()
    {
        return m_TpgIndex;
    }

    void CMaterial::SetTpgIndex(int index)
    {
        m_TpgIndex = index;
    }

    void CMaterial::SetCull(bool cull)
    {
        if (cull)
        {
            m_pData1[0xC]  = 0x20;
            m_pData3[0xE4] = 0x1;
        }
        else
        {
            m_pData1[0xC]  = 0x0;
            m_pData3[0xE4] = 0x0;
        }
    }

    const char *CMaterial::GetRenderMethodName()
    {
        return m_pRMthName;
    }

} // namespace CrpLib