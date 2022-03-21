#include "CRPLib/BPlanes.h"

#include "CRPLib/Entry.h"

namespace CrpLib
{
    void CBPlanes::Read(std::istream &is, ICrpEntry *entry)
    {
        is.read((char *) &m_Unk1, sizeof(m_Unk1));
        is.read((char *) &m_Unk2, sizeof(m_Unk2));

        m_pVertices.resize(dynamic_cast<CEntry *>(entry)->GetCount());

        is.read((char *) m_pVertices.data(), sizeof(decltype(m_pVertices)::value_type) * m_pVertices.size());
    }

    void CBPlanes::Write(std::ostream &os)
    {
        os.write((char *) &m_Unk1, sizeof(m_Unk1));
        os.write((char *) &m_Unk2, sizeof(m_Unk2));

        os.write((char *) m_pVertices.data(), sizeof(decltype(m_pVertices)::value_type) * m_pVertices.size());
    }

    int CBPlanes::GetEntryLength()
    {
        return sizeof(m_Unk1) + sizeof(m_Unk2) + sizeof(decltype(m_pVertices)::value_type) * m_pVertices.size();
    }

    int CBPlanes::GetEntryCount()
    {
        return m_pVertices.size();
    }

    size_t CBPlanes::GetCount()
    {
        return m_pVertices.size() / 4;
    }

    void CBPlanes::SetCount(size_t value)
    {
        m_pVertices.resize(value * 4);
    }

    tVector4 *CBPlanes::GetVertices()
    {
        return m_pVertices.data();
    }

} // namespace CrpLib