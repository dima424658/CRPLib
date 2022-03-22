#pragma once

#include "CRPLib/Interfaces.h"
#include "CRPLib/Matrix.h"
#include "CRPLib/Common.h"

#include <fstream>

namespace CrpLib {

    class CEffect :
            public ICrpData {
    private:
        int m_Unk1 = 0x5; // 0x5 always
        int m_Unk2 = 0x0; // 0x0 always

        CMatrix m_TransformMatrix;

        int m_GlowColor, m_SrcColor;
        int m_Mirror;

        tGlareInfo m_GlareInfo;

    public:
        virtual ~CEffect() = default;

        virtual void Read(std::istream &is, ICrpEntry *entry);
        virtual void Write(std::ostream &os);

        virtual int GetEntryLength();
        virtual int GetEntryCount();

        // Accessors and modifiers
        int GetGlowColor();
        void SetGlowColor(int value);

        int GetSourceColor();
        void SetSourceColor(int value);

        bool GetMirrored();
        void SetMirror(bool value);

        CMatrix *GetTransform();
        void SetTransform(CMatrix *value);

        tGlareInfo *GetGlareInfo();
    };
}
