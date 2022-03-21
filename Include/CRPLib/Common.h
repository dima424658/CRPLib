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
    constexpr short levelMin(T x)
    {
        return x << 12;
    }

    template <typename T>
    constexpr short levelMax(T x)
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
        unsigned char GeomIndex;
        unsigned char TypeIndex;
        unsigned char GeomUnk;
        unsigned char GeomPlace;

        eWheelType WheelType;
        eExtraType ExtraType;
        eTextureType TextureType;
        eGeomType GeomType;

        unsigned char AnimCount;
        eAnimType AnimType;
        eLevel LevelIndex; // see LEVELINDEX_n (n=LEVEL,INDEX,CREATE)
        unsigned char Zero1;

        eGlareType GlareType;
        unsigned char InteriorVisible; // 1 if true
        unsigned char Hand;            // 1 if true
        unsigned char Zero2;

        tBaseInfo()
        {
            GeomIndex = 0;
            TypeIndex = 0;
            GeomUnk = 0;
            GeomPlace = 0;

            WheelType = eWheelType::NonWheel;
            ExtraType = eExtraType::Type1;
            TextureType = eTextureType::Default;
            GeomType = eGeomType::Default;

            AnimCount = 1;
            AnimType = eAnimType::None;
            LevelIndex = eLevel::Body;
            Zero1 = 0;

            GlareType = eGlareType::None;
            InteriorVisible = 1;
            Hand = 0;
            Zero2 = 0;
        }
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

        tVector4()
        {
            this->x = this->y = this->z = this->w = 0.0f;
        }

        tVector4(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = 0.0f;
        }

        tVector4(float x, float y, float z, float w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
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

        tVector3()
        {
            this->x = this->y = this->z = 0.0f;
        }

        tVector3(float x, float y, float z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    struct tVector2
    {
        float u, v;

        tVector2()
        {
            this->u = this->v = 0.0f;
        }

        tVector2(float u, float v)
        {
            this->u = u;
            this->v = v;
        }
    };

    ICrpData *AllocateDataEntry(eEntryID id);
} // namespace CrpLib