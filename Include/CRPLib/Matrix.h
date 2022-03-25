#pragma once

#include <array>

#include "CRPLib/Interfaces.h"

namespace CrpLib
{
    class CMatrix : public ICrpData
    {
    private:
        std::array<float, 16> m_Items
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f    
        };

    public:
        CMatrix() = default;
        CMatrix(float *values);

        void Read(std::istream &is, ICrpEntry *entry) override;
        void Write(std::ostream &os) override;

        int GetEntryLength() override;
        int GetEntryCount() override;

        // accessors and modifiers

        float GetItem(size_t row, size_t col);
        void SetItem(size_t row, size_t col, float value);

        float *GetValues();
        void SetValues(float *values);
    };
} // namespace CrpLib
