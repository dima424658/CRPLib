#pragma once

#include <vector>

#include "CRPLib/Interfaces.h"
#include "CRPLib/Common.h"

namespace CrpLib
{
    class CBPlanes : public ICrpData
    {
    private:
        int m_Unk1 = 0, m_Unk2 = 0;

        std::vector<tVector4> m_pVertices;

    public:
        virtual ~CBPlanes() = default;
        
        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        void SetCount(size_t value);
        size_t GetCount();

        tVector4 *GetVertices();
    };
} // namespace CrpLib