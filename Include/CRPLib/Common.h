#pragma once

#include "CRPLib/Interfaces.h"

namespace CrpLib
{
    enum class eHeaderID : int
    {
        Car = 0x43617220,
        Track = 0x5472616B,
    };

    enum class eEntryID : int
    {
        // Parents
        Article = 0x41727469,

        // Normal use
        DamageZone = 0x647A,
        Vertex = 0x7674,
        UV = 0x7576,
        Transform = 0x7472,
        Part = 0x7072,
        Normal = 0x6E6D,
        Name = 0x4E616D65,
        Base = 0x42617365,
        Cull = 0x246E,
        Effect = 0x6566,

        // Misc
        FSH = 0x7366,
        Material = 0x6D74,
        RenderMethod = 0x524D7468,
        BPlanes = 0x426E6450,
    };

    enum class eRMName
    {
        Ext = 0,
        ExtEnv = 1,
        ExtEnvNS = 2,
        Int = 3,
        IntNS = 4,
        Wheel = 5,
        Window = 6,
    };

    enum class ePartTrans : short
    {
        Normal = 0,
        Window = 1,
        Texture = 0x200,

        WindowTexture = Window | Texture,
    };

    enum class eIndexRowID : short
    {
        Vertex = 0x4976,
        UV = 0x4975,
    };

    enum class eInfoRowID : short
    {
        Cull = 0x246E,
        Normal = 1,
        UV = 2,
        Vertex = 0,
    };

    struct tPartIndex
    {
        short Index;
        eIndexRowID Id; // INDEXROW_ID
        int Offset;
    };

    struct tPartInfo
    {
        int RMOffs, Offset;
        short Length;
        eInfoRowID Id;
        short Level, IndexRowRef;

        // TODO: These might *need* to be cast as INDEX_ROW
        int GetCount()
        {
            if (Id == eInfoRowID::UV)
                return (Length / 8);
            else
                return (Length / 16);
        }

        void SetCount(int value)
        {
            if (Id == eInfoRowID::UV)
                Length = (short)(value * 8);
            else
                Length = (short)(value * 16);
        }

        int GetOffsetIndex()
        {
            if (Id == eInfoRowID::UV)
                return (Offset / 8);
            else
                return (Offset / 16);
        }

        void SetOffsetIndex(int value)
        {
            if (Id == eInfoRowID::UV)
                Offset = value * 8;
            else
                Offset = value * 16;
        }
    };

    enum class eGlareType : char
    {
        Normal = 0,
        Brake = 1,
        Reverse = 2,
        Signal = 3,
        Sparkle = 4,
    };

    enum class eGlareExtra : char
    {
        Normal = 0,
        Mirror = 7,
        Transform = 4,
        Logo = 8,
    };

    enum class eGlareInfo : char
    {
        Light = 1,
        SignalFront = 5,
        SignalRear = 7,
        SignalBreak = 0xB,
        SignalBreak_1 = 2,
        SignalBreak_2 = 3,
        Reverse = 8,
        Sparkle = 9,
    };

    struct tGlareInfo
    {
        eGlareType Type;
        eGlareExtra Extra;
        eGlareInfo Info;
        bool Headlight;
    };

    template <typename T>
    constexpr T levelMin(T x)
    {
        return x << 12;
    }

    template <typename T>
    constexpr T levelMax(T x)
    {
        return levelMin(x) | 0xFFF;
    }

    struct tLevelMask
    {
        int LevType; // 0x1
        short Min;   // LEVEL_MIN()
        short Max;   // LEVEL_MAX()
        short Level;
        short Mask; // 0xFFFF

        tLevelMask()
        {
            LevType = 1;
            Min = levelMin(0);
            Max = levelMax(0);
            Level = 0;
            Mask = (short)0xFFFF;
        }

        void SetLevel(int level)
        {
            Min = levelMin(level);
            Max = levelMax(level);
            Level = level;
        }
    };

#define LEVELINDEX_LEVEL(x) ((x & 0xF0) >> 4)
#define LEVELINDEX_INDEX(x) (x & 0xF)
#define LEVELINDEX_CREATE(i, l) ((i & 0xF) << 4) | (l & 0xF)
    struct tBaseInfo
    {
        enum class eAnimType : unsigned char
        {
            None = 0,
            Cabrio = 1,
            Spoiler = 2,
            Wiper = 3,
            RightArm = 4,
            LeftArm = 5,
            DriverBody = 6,
            Antenna = 7,
        };

        enum class eLevel : unsigned char
        {
            Effect = 0,
            Body = 1,
            Wheel = 2,
            Steer = 4,
            Driver = 8
        };

        enum class eWheelType : unsigned char
        {
            Front1 = 0,
            Front2 = 1,
            Read1 = 2,
            Read2 = 3,
            NonWheel = 0xFF
        };

        enum class eExtraType : unsigned char
        {
            Type1 = 0,
            Type2 = 0x80,
            Antenna = 1
        };

        enum class eGlareType : unsigned char
        {
            None = 0,
            Shine = 9,
            Front = 4,
            Brake = 2,
            Reverse = 8,
            Rear = 6
        };

        enum class eTextureType : unsigned char
        {
            Default = 0,
            DoorIn = 1,
            Interior = 3,
            TrunkSpace = 5,
            Handle = 0xF6,
            Decal = 0xFD,
            Needle = 0xFE,
            Unknown = 0xFF,
        };

        enum class eGeomType : unsigned char
        {
            Default = 0,

            WheelFront1 = 1,
            WheelFront2 = 2,
            WheelRear1 = 3,
            WheelRear2 = 4,

            Steer = 5,
            DoorLeft = 6,
            DoorRight = 7,
            Hood = 8,
            Trunk = 9,
            Spoiler = 10,

            ThreadFront1 = 0x12,
            ThreadFront2 = 0x13,
            ThreadRear1 = 0x14,
            ThreadRear2 = 0x15,

            GlareFrontWheel = 1,
            GlareMirror = 6,
            GlareTrunkBrake = 9,
            GlareGasCap = 8,
        };

    public:
        unsigned char GeomIndex = 0;
        unsigned char TypeIndex = 0;
        unsigned char GeomUnk = 0;
        unsigned char GeomPlace = 0;

        eWheelType WheelType = eWheelType::NonWheel;
        eExtraType ExtraType = eExtraType::Type1;
        eTextureType TextureType = eTextureType::Default;
        eGeomType GeomType = eGeomType::Default;

        unsigned char AnimCount = 1;
        eAnimType AnimType = eAnimType::None;
        eLevel LevelIndex = eLevel::Body; // see LEVELINDEX_n (n=LEVEL,INDEX,CREATE)
        unsigned char Zero1 = 0;

        eGlareType GlareType = eGlareType::None;
        unsigned char InteriorVisible = 1; // 1 if true
        unsigned char Hand = 0;            // 1 if true
        unsigned char Zero2 = 0;
    };

    enum class BaseFlags : int
    {
        Normal = 0,
        Window = 1,
        Effect = 0x600,
    };

    struct tVector4
    {
        float x, y, z, w;

        tVector4(float x = 0, float y = 0, float z = 0, float w = 0) : x{x}, y{y}, z{z}, w{w}
        {
        }

        tVector4 operator+(const tVector4 &v)
        {
            return tVector4(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        tVector4 operator-(const tVector4 &v)
        {
            return tVector4(x - v.x, y - v.y, z - v.z, w - v.w);
        }
    };

    struct tVector3
    {
        float x, y, z;

        tVector3() = default;

        tVector3(float x = 0, float y = 0, float z = 0) : x{x}, y{y}, z{z}
        {
        }
    };

    struct tVector2
    {
        float u, v;

        tVector2(float u = 0, float v = 0) : u{u}, v{v}
        {
        }
    };

    ICrpData *AllocateDataEntry(eEntryID id);
} // namespace CrpLib