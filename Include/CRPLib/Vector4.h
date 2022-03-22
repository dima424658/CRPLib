#pragma once

#include <vector>

#include "CRPLib/Interfaces.h"
#include "CRPLib/Common.h"

namespace CrpLib
{
    class CVector4 : public ICrpData
    {
    private:
        std::vector<tVector4> m_pData;

    public:
        CVector4() = default;
        CVector4(tVector4 *pData, size_t count);
        virtual ~CVector4() = default;

        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        // Accessors and modifiers
        void SetItem(size_t index, tVector4 value);
        tVector4 &GetItem(size_t index);

        size_t GetCount();
        void SetCount(size_t count);
    };
} // namespace CrpLib
