#include "CRPLib/RawData.h"

#include "CRPLib/Entry.h"

namespace CrpLib
{
    CRawData::CRawData(char *pData, int length)
    {
        SetData(pData, length);
    }

    void CRawData::Read(std::istream &is, ICrpEntry *entry)
    {
        m_pData.resize(dynamic_cast<CEntry *>(entry)->GetLength());

        is.read(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    void CRawData::Write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    int CRawData::GetEntryLength()
    {
        return sizeof(decltype(m_pData)::value_type) * m_pData.size();
    }

    int CRawData::GetEntryCount()
    {
        return -1;
    }

    void CRawData::ReadFromStream(std::istream &is)
    {
        m_pData = std::vector<char>{std::istreambuf_iterator<char>{is}, std::istreambuf_iterator<char>{}};
    }

    void CRawData::WriteToStream(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(m_pData.data()), sizeof(decltype(m_pData)::value_type) * m_pData.size());
    }

    void CRawData::ParseFrom(ICrpEntry *entry)
    {
        std::fstream mfile{};
        // No fstream setLength mechanism like CMemFile so emulate by padding file to size of length
        uint32_t length = ((CEntry *)entry)->GetLength();
        std::vector<char> padBytes;
        padBytes.resize(length);
        mfile.write(padBytes.data(), sizeof(decltype(padBytes)::value_type) * padBytes.size());
        
        // TODO: Does CMemFile move the file pointer? If it doesn't, must rewind
        mfile.seekg(0, std::ios::beg);

        ICrpData *data = ((CEntry *)entry)->GetData();

        data->Write(mfile);

        mfile.seekg(0, std::ios::beg);

        this->Read(mfile, entry);

    }

    void CRawData::ParseTo(ICrpEntry *entry)
    {
        std::fstream mfile{};
        uint32_t length = ((CEntry *)entry)->GetLength();
        std::vector<char> padBytes;
        padBytes.resize(length);
        mfile.write(padBytes.data(), sizeof(decltype(padBytes)::value_type) * padBytes.size());
        mfile.seekg(0, std::ios::beg);

        ICrpData *data = ((CEntry *)entry)->GetData();

        Write(mfile);

        mfile.seekg(0, std::ios::beg);

        data->Read(mfile, entry);
    }

    // accessors and modifiers

    void CRawData::SetData(const char *pData, size_t length)
    {
        m_pData.clear();
        m_pData.insert(m_pData.end(), pData, pData + length);
    }

    void CRawData::SetData(const std::string &data)
    {
        SetData(data.c_str(), data.size() + 1);
    }

    char *CRawData::GetData()
    {
        return m_pData.data();
    }

    size_t CRawData::GetLength()
    {
        return sizeof(decltype(m_pData)::value_type) * m_pData.size();
    }
} // namespace CrpLib