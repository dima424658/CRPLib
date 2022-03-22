#pragma once

#include "CRPLib/Interfaces.h"
#include "CRPLib/Common.h"

#include <vector>

/*
    NOTE: CEntry->Count is number of indices NOT number of triangles
*/

namespace CrpLib
{
    class CPart : public ICrpData
    {
    private:
        short m_FillMode;       // D3DFILLMODE
        ePartTrans m_TransInfo; // PART_TRANS

        short m_Mat;
        short m_Unk1 = 0x8000;

        tVector4 m_BSphere;
        tVector4 m_Unk2;

        std::vector<tPartInfo> m_pInfo;
        std::vector<tPartIndex> m_pIndex;
        std::vector<char> m_pIndices;

    public:
        CPart() = default;

        virtual ~CPart() = default;

        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        // inits an empty CPart with required Info and Index entries
        void InitStorage(ICrpEntry *entry, int indiceCount, eRMName rm);

        short GetFillMode();
        void SetFillMode(short value);

        ePartTrans GetTransInfo();
        void SetTransInfo(ePartTrans value);

        short GetMaterial();
        void SetMaterial(short value);

        tVector4 &GetBoundingSphere();

        size_t GetInfoCount();
        size_t GetIndexCount();

        tPartInfo &GetInfo(size_t index);
        tPartIndex &GetIndex(size_t index);
        char &GetIndices(size_t index);

        size_t FindInfo(eInfoRowID ird);
        size_t FindIndex(eIndexRowID ird);
    };
} // namespace CrpLib
