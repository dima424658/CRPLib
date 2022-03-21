#include "CRPLib/Matrix.h"

namespace CrpLib
{
    CMatrix::CMatrix(float *values)
    {
        SetValues(values);
    }

    void CMatrix::Read(std::istream &is, ICrpEntry *entry)
    {
        is.read(reinterpret_cast<char *>(m_Items.data()), sizeof(decltype(m_Items)::value_type) * m_Items.size());
    }

    void CMatrix::Write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(m_Items.data()), sizeof(decltype(m_Items)::value_type) * m_Items.size());
    }

    int CMatrix::GetEntryLength()
    {
        return sizeof(decltype(m_Items)::value_type) * m_Items.size();
    }

    int CMatrix::GetEntryCount()
    {
        return 0x1;
    }

    // accessors and modifiers

    float CMatrix::GetItem(size_t row, size_t col)
    {
        return m_Items[row * 4 + col];
    }

    void CMatrix::SetItem(size_t row, size_t col, float value)
    {
        m_Items[row * 4 + col] = value;
    }

    float *CMatrix::GetValues()
    {
        return m_Items.data();
    }

    void CMatrix::SetValues(float *values)
    {
        std::copy(values, values + 16, m_Items.begin());
    }
} // namespace CrpLib