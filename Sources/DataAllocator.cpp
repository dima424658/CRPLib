#include "CRPLib/Common.h"

#include "CRPLib/RawData.h"
#include "CRPLib/Vector2.h"
#include "CRPLib/Vector4.h"
#include "CRPLib/Effect.h"
#include "CRPLib/Matrix.h"
#include "CRPLib/Part.h"
#include "CRPLib/Base.h"
#include "CRPLib/BPlanes.h"
#include "CRPLib/Material.h"

namespace CrpLib
{
    ICrpData* AllocateDataEntry(eEntryID id)
    {
        switch (id)
        {
        case eEntryID::Name:
            return new CRawData();
        case eEntryID::Vertex:
        case eEntryID::Normal:
        case eEntryID::Cull:
            return new CVector4();
        case eEntryID::UV:
            return new CVector2();
        case eEntryID::Effect:
            return new CEffect();
        case eEntryID::Transform:
            return new CMatrix();
        case eEntryID::Part:
            return new CPart();
        case eEntryID::Base:
            return new CBase();
        case eEntryID::BPlanes:
            return new CBPlanes();
        case eEntryID::Material:
            return new CMaterial();
        default:
            return new CRawData();
        }
    }
} // namespace CrpLib