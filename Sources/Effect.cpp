#include "CRPLib/Effect.h"

namespace CrpLib
{
    void CEffect::Read(std::istream &is, ICrpEntry *entry)
    {
        is.read(reinterpret_cast<char *>(&m_Unk1), sizeof(m_Unk1));
        is.read(reinterpret_cast<char *>(&m_Unk2), sizeof(m_Unk2));

        /*
        CMatrix trTmp;
        trTmp.Read(file, NULL);

        // convert from Effect matrix to Standard matrix
        int i,j;
        for (i=0;i<4;i++)
            m_Tr.SetItem(3,i,trTmp.GetItem(0,i));
        for (i=0;i<3;i++)
            for (j=0;j<4;j++)
                m_Tr.SetItem(j,i,trTmp.GetItem(j+1,i));

        */

        float fLine[4];
        is.read(reinterpret_cast<char *>(fLine), sizeof(fLine));

        for (int i = 0; i < 4; ++i)
            m_TransformMatrix.SetItem(3, i, fLine[i]);

        for (int i = 0; i < 3; ++i)
        {
            is.read(reinterpret_cast<char *>(fLine), sizeof(fLine));
            for (int j = 0; j < 4; ++j)
                m_TransformMatrix.SetItem(i, j, fLine[j]);
        }

        is.read(reinterpret_cast<char *>(&m_GlowColor), sizeof(m_GlowColor));
        is.read(reinterpret_cast<char *>(&m_SrcColor), sizeof(m_SrcColor));
        is.read(reinterpret_cast<char *>(&m_Mirror), sizeof(m_Mirror));

        is.read(reinterpret_cast<char *>(&m_GlareInfo.Type), sizeof(m_GlareInfo.Type));
        is.read(reinterpret_cast<char *>(&m_GlareInfo.Extra), sizeof(m_GlareInfo.Extra));
        is.read(reinterpret_cast<char *>(&m_GlareInfo.Info), sizeof(m_GlareInfo.Info));

        char Headlight;
        is.read(reinterpret_cast<char *>(&Headlight), sizeof(Headlight));

        m_GlareInfo.Headlight = (Headlight == 1);
    }

    void CEffect::Write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(&m_Unk1), sizeof(m_Unk1));
        os.write(reinterpret_cast<char *>(&m_Unk2), sizeof(m_Unk2));

        /*
        CMatrix trTmp;

        // convert from Standard matrix to Effect matrix
        int i,j;
        for (i=0;i<4;i++)
            trTmp.SetItem(0,i,m_Tr.GetItem(3,i));
        for (i=0;i<3;i++)
            for (j=0;j<4;j++)
                trTmp.SetItem(j+1,i,trTmp.GetItem(j,i));

        trTmp.Write(file);
        */

        float fLine[4];

        for (int i = 0; i < 4; ++i)
            fLine[i] = m_TransformMatrix.GetItem(3, i);

        os.write(reinterpret_cast<char *>(fLine), sizeof(fLine));

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 4; ++j)
                fLine[j] = m_TransformMatrix.GetItem(i, j);

            os.write(reinterpret_cast<char *>(fLine), sizeof(fLine));
        }

        os.write(reinterpret_cast<char *>(&m_GlowColor), sizeof(m_GlowColor));
        os.write(reinterpret_cast<char *>(&m_SrcColor), sizeof(m_SrcColor));
        os.write(reinterpret_cast<char *>(&m_Mirror), sizeof(m_Mirror));

        os.write(reinterpret_cast<char *>(&m_GlareInfo.Type), sizeof(m_GlareInfo.Type));
        os.write(reinterpret_cast<char *>(&m_GlareInfo.Extra), sizeof(m_GlareInfo.Extra));
        os.write(reinterpret_cast<char *>(&m_GlareInfo.Info), sizeof(m_GlareInfo.Info));

        char Headlight = (m_GlareInfo.Headlight ? 1 : 0);
        os.write(reinterpret_cast<char *>(&Headlight), sizeof(Headlight));
    }

    int CEffect::GetEntryLength()
    {
        return 0x58;
    }

    int CEffect::GetEntryCount()
    {
        return 0x1;
    }

    // Accessors and modifiers

    int CEffect::GetGlowColor()
    {
        return m_GlowColor;
    }

    void CEffect::SetGlowColor(int value)
    {
        m_GlowColor = value;
    }

    int CEffect::GetSourceColor()
    {
        return m_SrcColor;
    }

    void CEffect::SetSourceColor(int value)
    {
        m_SrcColor = value;
    }

    bool CEffect::GetMirrored()
    {
        return (m_Mirror == 8);
    }

    void CEffect::SetMirror(bool value)
    {
        m_Mirror = (value ? 8 : 0);
    }

    CMatrix *CEffect::GetTransform()
    {
        return &m_TransformMatrix;
    }

    void CEffect::SetTransform(CMatrix *value)
    {
        m_TransformMatrix = *value;
    }

    tGlareInfo *CEffect::GetGlareInfo()
    {
        return &m_GlareInfo;
    }
} // namespace CrpLib