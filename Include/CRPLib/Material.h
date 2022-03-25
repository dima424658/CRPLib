#pragma once

#include "CRPLib/Interfaces.h"

namespace CrpLib
{
    class CMaterial : public ICrpData
    {
    private:
        char m_pData1[0x10];
        char m_pRMthName[0x10];
        char m_pData2[0x8];
        int m_TpgIndex;
        char m_pData3[0x10C];

    public:
        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        int GetTpgIndex();

        void SetTpgIndex(int index);

        void SetCull(bool cull);

        const char *GetRenderMethodName();
    };

} // namespace CrpLib