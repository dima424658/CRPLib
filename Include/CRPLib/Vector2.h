#pragma once

#include <vector>

#include "CRPLib/Interfaces.h"
#include "CRPLib/Common.h"

namespace CrpLib
{
    class CVector2 : public ICrpData
    {
    private:
        std::vector<tVector2> m_pData;

    public:
        CVector2() = default;
        CVector2(tVector2 *pData, size_t count);
        virtual ~CVector2() = default;

        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        // Accessors and modifiers
        void SetItem(size_t index, tVector2 value);
        tVector2 &GetItem(size_t index);

        size_t GetCount();
        void SetCount(size_t count);
    };
} // namespace CrpLib
