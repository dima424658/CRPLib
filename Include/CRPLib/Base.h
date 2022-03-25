#pragma once

#include <vector>

#include "CRPLib/Interfaces.h"
#include "CRPLib/Common.h"

namespace CrpLib
{
    class CBase : public ICrpData
    {
    private:
        BaseFlags m_Flags = {};
        int m_Zero1 = 0, m_Zero2 = 0;

        int m_HasFloats = 0;
        float m_pFloats[12] = {};

        tBaseInfo m_BaseInfo{};

        tVector4 m_Unk;

        std::vector<tLevelMask> m_pLevMasks;

    public:
        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        void CreateLevels(int LevCount, int Start = 1);
        int GetLevelCount();
        tLevelMask &GetLevel(size_t i);

        BaseFlags GetFlags();
        void SetFlags(BaseFlags flags);

        void SetHasFloats(bool val);
        float *GetFloats();

        tBaseInfo &GetBaseInfo();
    };
} // namespace CrpLib
