#include <algorithm>
#include <limits>

#include "CRPLib/Part.h"

#include "CRPLib/Entry.h"

namespace CrpLib
{
    void CPart::Read(std::istream &is, ICrpEntry *entry)
    {
        int infoCount, indexCount;

        is.read(reinterpret_cast<char *>(&m_FillMode), sizeof(m_FillMode));
        is.read(reinterpret_cast<char *>(&m_TransInfo), sizeof(m_TransInfo));

        is.read(reinterpret_cast<char *>(&m_Mat), sizeof(m_Mat));
        is.read(reinterpret_cast<char *>(&m_Unk1), sizeof(m_Unk1));

        is.read(reinterpret_cast<char *>(&m_BSphere), sizeof(m_BSphere));
        is.read(reinterpret_cast<char *>(&m_Unk2), sizeof(m_Unk2));

        is.read(reinterpret_cast<char *>(&infoCount), sizeof(infoCount));
        is.read(reinterpret_cast<char *>(&indexCount), sizeof(indexCount));

        m_pInfo.resize(infoCount);
        m_pIndex.resize(indexCount);
        m_pIndices.resize(indexCount * dynamic_cast<CEntry *>(entry)->GetCount());

        is.read(reinterpret_cast<char *>(m_pInfo.data()), sizeof(decltype(m_pInfo)::value_type) * m_pInfo.size());
        is.read(reinterpret_cast<char *>(m_pIndex.data()), sizeof(decltype(m_pIndex)::value_type) * m_pIndex.size());
        is.read(reinterpret_cast<char *>(m_pIndices.data()), sizeof(decltype(m_pIndices)::value_type) * m_pIndices.size());
    }

    void CPart::Write(std::ostream &os)
    {
        int infoCount = m_pInfo.size(), indexCount = m_pIndex.size();

        os.write(reinterpret_cast<char *>(&m_FillMode), sizeof(m_FillMode));
        os.write(reinterpret_cast<char *>(&m_TransInfo), sizeof(m_TransInfo));

        os.write(reinterpret_cast<char *>(&m_Mat), sizeof(m_Mat));
        os.write(reinterpret_cast<char *>(&m_Unk1), sizeof(m_Unk1));

        os.write(reinterpret_cast<char *>(&m_BSphere), sizeof(m_BSphere));
        os.write(reinterpret_cast<char *>(&m_Unk2), sizeof(m_Unk2));

        os.write(reinterpret_cast<char *>(&infoCount), sizeof(infoCount));
        os.write(reinterpret_cast<char *>(&indexCount), sizeof(indexCount));

        os.write(reinterpret_cast<char *>(m_pInfo.data()), sizeof(decltype(m_pInfo)::value_type) * m_pInfo.size());
        os.write(reinterpret_cast<char *>(m_pIndex.data()), sizeof(decltype(m_pIndex)::value_type) * m_pIndex.size());
        os.write(reinterpret_cast<char *>(m_pIndices.data()), sizeof(decltype(m_pIndices)::value_type) * m_pIndices.size());
    }

    int CPart::GetEntryLength()
    {
        return sizeof(m_FillMode) + sizeof(m_TransInfo) + sizeof(m_Mat) + sizeof(m_Unk1) + sizeof(m_BSphere) + sizeof(m_Unk2) + sizeof(int) /* infoCount */ +
               sizeof(int) /* indexCount */ + sizeof(decltype(m_pInfo)::value_type) * m_pInfo.size() + sizeof(decltype(m_pIndex)::value_type) * m_pIndex.size() +
               sizeof(decltype(m_pIndices)::value_type) * m_pIndices.size();
    }

    int CPart::GetEntryCount()
    {
        return m_pIndices.size() / m_pIndex.size();
    }

    // inits an empty CPart with required Info and Index entries
    void CPart::InitStorage(ICrpEntry *entry, int indiceCount, eRMName rm)
    {
        dynamic_cast<CEntry*>(entry)->SetCount(indiceCount);

        m_pIndex.resize(2);

        m_pIndex[0].Id    = eIndexRowID::Vertex;
        m_pIndex[0].Index = 1;

        m_pIndex[1].Id    = eIndexRowID::UV;
        m_pIndex[1].Index = 1;

        m_pInfo.resize(4);

        m_pInfo[0].Id          = eInfoRowID::Cull;
        m_pInfo[0].IndexRowRef = -1;

        m_pInfo[1].Id          = eInfoRowID::Normal;
        m_pInfo[1].IndexRowRef = 0;

        m_pInfo[2].Id          = eInfoRowID::UV;
        m_pInfo[2].IndexRowRef = 1;

        m_pInfo[3].Id          = eInfoRowID::Vertex;
        m_pInfo[3].IndexRowRef = 0;

        switch (rm)
        {
        case eRMName::Ext:
            m_pInfo[0].RMOffs = 0x2;
            m_pInfo[1].RMOffs = 0x8;
            m_pInfo[2].RMOffs = 0x9;
            m_pInfo[3].RMOffs = 0xA;
            break;
        case eRMName::ExtEnv:
            m_pInfo[0].RMOffs = 0x4;
            m_pInfo[1].RMOffs = 0x13;
            m_pInfo[2].RMOffs = 0x14;
            m_pInfo[3].RMOffs = 0x15;
            break;
        case eRMName::Int:
            m_pInfo[0].RMOffs = 0x1;
            m_pInfo[1].RMOffs = 0x7;
            m_pInfo[2].RMOffs = 0x8;
            m_pInfo[3].RMOffs = 0x9;
            break;
        case eRMName::Wheel:
            m_pInfo[0].RMOffs = 0x1;
            m_pInfo[1].RMOffs = 0x7;
            m_pInfo[2].RMOffs = 0x8;
            m_pInfo[3].RMOffs = 0x9;
            break;
        case eRMName::Window:
            m_pInfo[0].RMOffs = 0x4;
            m_pInfo[1].RMOffs = 0xE;
            m_pInfo[2].RMOffs = 0xF;
            m_pInfo[3].RMOffs = 0x10;
            break;
        }

        m_pIndices.clear();
        m_pIndices.insert(m_pIndices.end(), indiceCount * m_pIndex.size(), 0);
    }

    short CPart::GetFillMode()
    {
        return m_FillMode;
    }

    void CPart::SetFillMode(short value)
    {
        m_FillMode = value;
    }

    ePartTrans CPart::GetTransInfo()
    {
        return m_TransInfo;
    }

    void CPart::SetTransInfo(ePartTrans value)
    {
        m_TransInfo = value;
    }

    short CPart::GetMaterial()
    {
        return m_Mat;
    }

    void CPart::SetMaterial(short value)
    {
        m_Mat = value;
    }

    tVector4 &CPart::GetBoundingSphere()
    {
        return m_BSphere;
    }

    size_t CPart::GetInfoCount()
    {
        return m_pInfo.size();
    }

    size_t CPart::GetIndexCount()
    {
        return m_pIndex.size();
    }

    tPartInfo &CPart::GetInfo(size_t index)
    {
        return m_pInfo[index];
    }

    tPartIndex &CPart::GetIndex(size_t index)
    {
        return m_pIndex[index];
    }

    char &CPart::GetIndices(size_t index)
    {
        return m_pIndices[m_pIndex[index].Offset];
    }

    size_t CPart::FindInfo(eInfoRowID ird)
    {
        auto result = std::find_if(m_pInfo.begin(), m_pInfo.end(), [ird](const auto &it) { return it.Id == ird; });

        if (result == m_pInfo.end())
            return std::numeric_limits<size_t>().max();
        else
            return std::distance(m_pInfo.begin(), result);
    }

    size_t CPart::FindIndex(eIndexRowID ird)
    {
        auto result = std::find_if(m_pIndex.begin(), m_pIndex.end(), [ird](const auto &it) { return it.Id == ird; });

        if (result == m_pIndex.end())
            return std::numeric_limits<size_t>().max();
        else
            return std::distance(m_pIndex.begin(), result);
    }

} // namespace CrpLib