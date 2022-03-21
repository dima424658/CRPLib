#pragma once

#include <vector>
#include <string>

#include "CRPLib/Interfaces.h"

namespace CrpLib
{
    class CRawData : public ICrpData
    {
    private:
        static constexpr uint32_t cRawDataMagic = 0x57415243;

        std::vector<char> m_pData;

    public:
        CRawData() = default;
        CRawData(char *pData, int length);

        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        void ReadFromStream(std::istream &is);
        void WriteToStream(std::ostream &os);

        void ParseFrom(ICrpEntry *entry);
        void ParseTo(ICrpEntry *entry);

        // Accessors and modifiers
        void SetData(const char *pData, size_t length);
        void SetData(const std::string &data);

        char *GetData();
        size_t GetLength();
    };
} // namespace CrpLib
