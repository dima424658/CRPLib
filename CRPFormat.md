# CRP 3D Mesh and Textures (\*.CRP)

The files are compressed using a LZ77 algorithm style algorithm.  
The following format is based on the decompressed data.

## The .CRP file starts with following 16-byte header:

### Header

| Offset | Length |           Type            | Description                                               |
| :----: | :----: | :-----------------------: | --------------------------------------------------------- |
| 0x0000 | 0x0004 |          char[4]          | Identifier (cars: `raC` / `Car`; tracks: `karT` / `Trak`) |
| 0x0004 | 0x0004 | [HeaderInfo](#headerinfo) | Header info                                               |
| 0x0008 | 0x0004 |          LONGINT          | Number of MiscData following table of Articles            |
| 0x000C | 0x0004 |          LONGINT          | Offset Table of Articles (\* 16)                          |
|        | 0x0010 |                           | Size of header                                            |

### HeaderInfo

| Length (bits) | Description                             |
| ------------- | --------------------------------------- |
| 5 bits        | Unknown (always seems to be 0x1A)       |
| 27 bits       | Number of parts (Articles in this case) |
| 32 bits       | Size of HeaderInfo                      |

## Table of Articles

- **Offset**: Header Size
- **Length**: Number of Articles \* 16
- **Type**: Article[]

### Article

Each Article points to a PartTable.

| Offset | Length |           Type            | Description                                         |
| :----: | :----: | :-----------------------: | --------------------------------------------------- |
| 0x0000 | 0x0004 |          LONGINT          | Identifier (`itrA` / `Arti`)                        |
| 0x0004 | 0x0004 | [HeaderInfo](#headerinfo) | Header info                                         |
| 0x0008 | 0x0004 |          LONGINT          | Length of PartTable pointed to (\* 16)              |
| 0x000C | 0x0004 |          LONGINT          | Offset (Relative from current Article offset \* 16) |
|        | 0x0010 |                           | Size of Article                                     |

### Table of MiscPart

MiscPart can be MiscData (miscellaneous data) or it can be a MaterialPart or FSHPart.

- **Offset**: Header Size + Table of Articles Length
- **Length**: Number of MiscData \* Size of MiscData
- **Type**: MiscData[Number of MiscData]

### MiscPart

| Offset | Length |   Type   | Description                                    |
| :----: | :----: | :------: | ---------------------------------------------- |
| 0x0000 | 0x0004 | LONGINT  | Identifier                                     |
| 0x0004 | 0x0001 | SHORTINT | Unknown                                        |
| 0x0005 | 0x0003 | INTEGER  | Length                                         |
| 0x0008 | 0x0004 | LONGINT  | Unknown                                        |
| 0x000C | 0x0004 | LONGINT  | Offset (Relative from current MiscPart offset) |
|        | 0x0010 |          | Size of MiscPart                               |

### MaterialPart

| Offset | Length |   Type   | Description                                        |
| :----: | :----: | :------: | -------------------------------------------------- |
|  0x00  |  0x02  | SHORTINT | Index                                              |
|  0x02  |  0x02  | CHAR[2]  | Identifier (`tm` / `mt`)                           |
|  0x04  |  0x01  | SHORTINT | Unknown (seems always to be 0xFB)                  |
|  0x04  |  0x03  | INTEGER  | Length                                             |
|  0x08  |  0x04  | LONGINT  | Unknown (seems always to be 0x34)                  |
|  0x0C  |  0x04  | LONGINT  | Offset (Relative from current MaterialPart offset) |
|        | 0x0010 |          | Size of MaterialPart                               |

### FSHPart

Each FSHPart points to an FSH file.

| Offset | Length |   Type   | Description                                   |
| :----: | :----: | :------: | --------------------------------------------- |
|  0x00  |  0x02  | SHORTINT | Index                                         |
|  0x02  |  0x02  | CHAR[2]  | Identifier (`fs` / `sf`)                      |
|  0x04  |  0x01  | SHORTINT | Unknown (seems always to be 0xFB)             |
|  0x04  |  0x03  | INTEGER  | Length                                        |
|  0x08  |  0x04  | LONGINT  | Number of FSH files (seems always to be 0x01) |
|  0x0C  |  0x04  | LONGINT  | Offset (Relative from current FSHPart offset) |
|        | 0x0010 |          | Size of FSHPart                               |

## Table of PartTables

- **Offset**: Header Size + Table of Articles Length + Table of MiscData Length
- **Length**: Unknown
- **Type**: PartTable[]

### PartTable

All Parts are 0x10 long.

| Offset |      Length       |  Type   | Description       |
| :----: | :---------------: | :-----: | ----------------- |
|  0x00  |     N \* 0x10     | Part[N] | Table of Parts    |
|        | Length in Article |         | Size of PartTable |

## Possible Part structures:

### BasePart

| Offset | Length |   Type   | Description                                    |
| :----: | :----: | :------: | ---------------------------------------------- |
|  0x00  |  0x04  | CHAR[4]  | Identifier (`esaB` / `Base`)                   |
|  0x04  |  0x01  | SHORTINT | Unknown (seems always to be 0xEA)              |
|  0x05  |  0x03  | INTEGER  | Length                                         |
|  0x08  |  0x04  | LONGINT  | Unknown (seems always to be 0x00000000)        |
|  0x0C  |  0x04  | LONGINT  | Offset (Relative from current BasePart offset) |
|        | 0x0010 |          | Size of BasePart                               |

### NamePart

Each NamePart points to a zero-terminated string.

| Offset | Length |   Type   | Description                                    |
| :----: | :----: | :------: | ---------------------------------------------- |
|  0x00  |  0x04  | CHAR[4]  | Identifier (`emaN` / `Name`)                   |
|  0x04  |  0x01  | SHORTINT | Unknown (seems always to be 0xFA)              |
|  0x05  |  0x03  | INTEGER  | Length of text                                 |
|  0x08  |  0x04  | LONGINT  | Unknown (seems always to be 0x00000000)        |
|  0x0C  |  0x04  | LONGINT  | Offset (Relative from current NamePart offset) |
|        | 0x0010 |          | Size of NamePart                               |

### CullingPart

| Offset | Length |           Type            | Description                                       |
| :----: | :----: | :-----------------------: | ------------------------------------------------- |
|  0x00  |  0x02  | [Info1](#info1-structure) | Part info of type 1                               |
|  0x02  |  0x04  |          CHAR[2]          | Identifier (`n$` / `$n`)                          |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                           |
|  0x05  |  0x03  |          INTEGER          | Length                                            |
|  0x08  |  0x04  |          LONGINT          | Number of Culling                                 |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current CullingPart offset) |
|        | 0x0010 |                           | Size of CullingPart                               |

### TransformationPart

| Offset | Length |           Type            | Description                                              |
| :----: | :----: | :-----------------------: | -------------------------------------------------------- |
|  0x00  |  0x02  | [Info1](#info1-structure) | Part info of type 1                                      |
|  0x02  |  0x02  |          CHAR[2]          | Identifier (`rt` / `tr`)                                 |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                                  |
|  0x05  |  0x03  |          INTEGER          | Length                                                   |
|  0x08  |  0x04  |          LONGINT          | Number of Transformation Matrices (always 1)             |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current TransformationPart offset) |
|        | 0x0010 |                           | Size of TransformationPart                               |

### VertexPart

| Offset | Length |           Type            | Description                                      |
| :----: | :----: | :-----------------------: | ------------------------------------------------ |
|  0x00  |  0x02  | [Info1](#info1-structure) | Part info of type 1                              |
|  0x02  |  0x02  |          CHAR[2]          | Identifier (`tv` / `vt`)                         |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                          |
|  0x05  |  0x03  |          INTEGER          | Length                                           |
|  0x08  |  0x04  |          LONGINT          | Number of Vertices                               |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current VertexPart offset) |
|        | 0x0010 |                           | Size of VertexPart                               |

### NormalPart

| Offset | Length |           Type            | Description                                      |
| :----: | :----: | :-----------------------: | ------------------------------------------------ |
|  0x00  |  0x02  | [Info1](#info1-structure) | Part info of type 1                              |
|  0x02  |  0x02  |          CHAR[2]          | Identifier (`mn` / `nm`)                         |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                          |
|  0x05  |  0x03  |          INTEGER          | Length                                           |
|  0x08  |  0x04  |          LONGINT          | Number of Normals                                |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current NormalPart offset) |
|        | 0x0010 |                           | Size of NormalPart                               |

### UVPart

| Offset | Length |           Type            | Description                                  |
| :----: | :----: | :-----------------------: | -------------------------------------------- |
|  0x00  |  0x02  | [Info1](#info1-structure) | Part info of type 1                          |
|  0x02  |  0x02  |          CHAR[2]          | Identifier (`vu` / `uv`)                     |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                      |
|  0x05  |  0x03  |          INTEGER          | Length                                       |
|  0x08  |  0x04  |          LONGINT          | Number of UV                                 |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current UVPart offset) |
|        | 0x0010 |                           | Size of UVPart                               |

### TrianglePart

| Offset | Length |           Type            | Description                                        |
| :----: | :----: | :-----------------------: | -------------------------------------------------- |
|  0x00  |  0x02  | [Info2](#info2-structure) | Part info of type 2                                |
|  0x02  |  0x02  |          CHAR[2]          | Identifier (`rp` / `pr`)                           |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                            |
|  0x05  |  0x03  |          INTEGER          | Length                                             |
|  0x08  |  0x04  |          LONGINT          | Number of Indices                                  |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current TrianglePart offset) |
|        | 0x0010 |                           | Size of TrianglePart                               |

### EffectPart

| Offset | Length |           Type            | Description                                      |
| :----: | :----: | :-----------------------: | ------------------------------------------------ |
|  0x00  |  0x02  | [Info1](#info1-structure) | Part info of type 1                              |
|  0x02  |  0x02  |          CHAR[2]          | Identifier (`fe` / `ef`)                         |
|  0x04  |  0x01  |         SHORTINT          | Unknown                                          |
|  0x05  |  0x03  |          INTEGER          | Length                                           |
|  0x08  |  0x04  |          LONGINT          | Number of Effects (always 1)                     |
|  0x0C  |  0x04  |          LONGINT          | Offset (Relative from current EffectPart offset) |
|        | 0x0010 |                           | Size of EffectPart                               |

### Part info structures:

#### Info1 structure:

| Length (bits) | Description                       |
| ------------- | --------------------------------- |
| 4 bits        | Level of detail                   |
| 8 bits        | Animation index                   |
| 4 bits        | Damage switch (0x8 means damaged) |

#### Info2 structure:

| Length (bits) | Description     |
| ------------- | --------------- |
| 12 bits       | Part index      |
| 4 bits        | Level of detail |

## Data

The raw data which is addressed from the headers.

### Material

| Offset | Length |   Type   | Description                       |
| :----: | :----: | :------: | --------------------------------- |
|  0x00  |  0x10  |   ????   | Unknown                           |
|  0x10  |  0x10  | CHAR[16] | Description                       |
|  0x20  |  0x08  |   ????   | Unknown                           |
|  0x28  |  0x04  | LONGINT  | Texture page index (see TPG file) |
|  0x2C  | 0x010C |   ????   | Unknown                           |
|        | 0x0138 |          | Size of Material                  |

### Base

| Offset |    Length    |   Type   | Description                                |
| :----: | :----------: | :------: | ------------------------------------------ |
|  0x00  |     0x0C     |   ????   | Unknown                                    |
|  0x0C  |     0x04     | LONGINT  | Number of Levels                           |
|  0x10  |     0x34     |   ????   | Unknown                                    |
|  0x44  |     0x01     | SHORTINT | Geometry index                             |
|  0x45  |     0x01     | SHORTINT | Type index (0 = default; other = varation) |
|  0x46  |     0x02     |   ????   | Unknown                                    |
|  0x48  |     0x02     |   ????   | Unknown (seems to be unused)               |
|  0x4A  |     0x02     |   ????   | Unknown                                    |
|  0x4C  |     0x02     |   ????   | Unknown (seems to be 01 00)                |
|  0x4E  |     0x02     |  LEVIND  | Level index                                |
|  0x50  |     0x14     |   ????   | Unknown                                    |
|  0x64  |     LEN      |  LEVINF  | Level masks                                |
|        | 0x0064 + LEN |          | Size of Base                               |

- **LEVIND**: IL 00 (with I the index: 1 = body; 2 = wheel; 8 = driver)
- **LEN**: Number of Levels \* 0x0C
- **LEVINF**: 01 00 00 00 00 L0 FF LF 0L 00 FF FF (with L the Level)

#### Levels (LEVIND - L):

Levels may be left out.

| Value | Description               |
| :---: | ------------------------- |
|   1   | Model used in garage      |
|   2   | Model used in in-car view |
|  >3   | Exterior models           |

### Culling

The coordinates determine the normal of the corresponding triangle.  
The threshold affects the back-face culling algorithm.

| Offset | Length | Type | Description     |
| :----: | :----: | :--: | --------------- |
|  0x00  |  0x04  | REAL | X-coordinate    |
|  0x04  |  0x04  | REAL | Y-coordinate    |
|  0x08  |  0x04  | REAL | Z-coordinate    |
|  0x0C  |  0x04  | REAL | Threshold       |
|        | 0x0010 |      | Size of Culling |

#### Threshold:

- **Positive values**: sooner culling
- **Zero value** : normal culling
- **Negative values**: later culling

### TransformationMatrix

| Offset | Length |  Type  | Description                     |
| :----: | :----: | :----: | ------------------------------- |
|  0x00  |  0x40  | Matrix | Normal 4D Transformation Matrix |
|        | 0x0040 |        | Size of TransformationMatrix    |

### Vertex

Vertex seems to be a normal homogeneous 3D point.

| Offset | Length | Type | Description                             |
| :----: | :----: | :--: | --------------------------------------- |
|  0x00  |  0x04  | REAL | X-coordinate                            |
|  0x04  |  0x04  | REAL | Y-coordinate                            |
|  0x08  |  0x04  | REAL | Z-coordinate                            |
|  0x0C  |  0x04  | REAL | Unknown (seems always to be 0x3F800000) |
|        | 0x0010 |      | Size of Vertex                          |

### Normal

Normal seems to be a normal homogeneous 3D vector.

| Offset | Length | Type | Description                             |
| :----: | :----: | :--: | --------------------------------------- |
|  0x00  |  0x04  | REAL | X-coordinate                            |
|  0x04  |  0x04  | REAL | Y-coordinate                            |
|  0x08  |  0x04  | REAL | Z-coordinate                            |
|  0x0C  |  0x04  | REAL | Unknown (seems always to be 0x00000000) |
|        | 0x0010 |      | Size of Normal                          |

### UV

| Offset | Length | Type | Description  |
| :----: | :----: | :--: | ------------ |
|  0x00  |  0x04  | REAL | U-coordinate |
|  0x04  |  0x04  | REAL | V-coordinate |
|        | 0x0008 |      | Size of UV   |

### TriangleData

- **LENF** = NIF \* 0x10
- **OFFD** = LENF + 0x30
- **LEND** = NID \* 0x08

Info flags:

- **1st byte** - D3DFILLMODE:
  - D3DFILL_POINT = 1
  - D3DFILL_WIREFRAME = 2
  - D3DFILL_SOLID = 3

| Offset |       Length       |     Type      | Description                             |
| :----: | :----------------: | :-----------: | --------------------------------------- |
|  0x00  |        0x04        |     ????      | Info flags                              |
|  0x04  |        0x02        |    INTEGER    | Material index                          |
|  0x06  |        0x02        |     ????      | Unknown                                 |
|  0x08  |        0x10        |   FLOAT[4]    | Unknown                                 |
|  0x18  |        0x10        |     ????      | Unknown (always seems to be 0x00000000) |
|  0x28  |        0x04        |    LONGINT    | Number of InfoRows (NIF)                |
|  0x2C  |        0x04        |    LONGINT    | Number of IndexRows (NID)               |
|  0x30  |        LENF        | [InfoRow[NIF]](#possible-inforow-structures)  | Info rows                               |
|  OFFD  |        LEND        | [IndexRow[NID](#possible-indexrow-structures) | Index rows                              |
|        | 0x30 + LENF + LEND |               | Size of TriangleData header             |

## Possible InfoRow structures:

### CullingRow

| Offset | Length |  Type   | Description                         |
| :----: | :----: | :-----: | ----------------------------------- |
|  0x00  |  0x04  |  ????   | Unknown                             |
|  0x04  |  0x04  | LONGINT | Offset in culling data              |
|  0x08  |  0x02  | INTEGER | Length of culling data used         |
|  0x0A  |  0x02  | CHAR[2] | Identifier (`n$`)                   |
|  0x0C  |  0x02  | INTEGER | Level index                         |
|  0x0E  |  0x02  |  ????   | Unknown (always seems to be 0xFFFF) |
|        | 0x0010 |         | Size of CullingRow                  |

### VertexRow

| Offset | Length |  Type   | Description                         |
| :----: | :----: | :-----: | ----------------------------------- |
|  0x00  |  0x04  |  ????   | Unknown                             |
|  0x04  |  0x04  | LONGINT | Offset in vertex data               |
|  0x08  |  0x02  | INTEGER | Length of vertex data used          |
|  0x0A  |  0x04  | INTEGER | Unknown (?ordening?)                |
|  0x0C  |  0x02  | INTEGER | Level index                         |
|  0x0E  |  0x02  |  ????   | Unknown (always seems to be 0x0000) |
|        | 0x0010 |         | Size of VertexRow                   |

### NormalRow

| Offset | Length |  Type   | Description                         |
| :----: | :----: | :-----: | ----------------------------------- |
|  0x00  |  0x04  |  ????   | Unknown                             |
|  0x04  |  0x04  | LONGINT | Offset in normal data               |
|  0x08  |  0x02  | INTEGER | Length of normal data used          |
|  0x0A  |  0x04  | INTEGER | Unknown (?ordening?)                |
|  0x0C  |  0x02  | INTEGER | Level index                         |
|  0x0E  |  0x02  |  ????   | Unknown (always seems to be 0x0000) |
|        | 0x0010 |         | Size of NormalRow                   |

### UVRow

NIF always seems to be 3 or 4:

- **3**:
  - NormalRow
  - UVRow
  - VertexRow
- **4**:
  - CullingRow
  - NormalRow
  - UVRow
  - VertexRow

| Offset | Length |  Type   | Description                         |
| :----: | :----: | :-----: | ----------------------------------- |
|  0x00  |  0x04  |  ????   | Unknown                             |
|  0x04  |  0x04  | LONGINT | Offset in uv data                   |
|  0x08  |  0x02  | INTEGER | Length of uv data used              |
|  0x0A  |  0x04  | INTEGER | Unknown (?ordening?)                |
|  0x0C  |  0x02  | INTEGER | Level index                         |
|  0x0E  |  0x02  |  ????   | Unknown (always seems to be 0x0001) |
|        | 0x0010 |         | Size of UVRow                       |

## Possible IndexRow structures:

### VertexIndexRow

| Offset | Length |  Type   | Description              |
| :----: | :----: | :-----: | ------------------------ |
|  0x00  |  0x02  | INTEGER | Index of VertexIndex     |
|  0x02  |  0x02  | CHAR[2] | Identifier (`vI` / `Iv`) |
|  0x04  |  0x04  | LONGINT | Offset of Vertex indices |
|        | 0x0008 |         | Size of VertexIndexRow   |

### UVIndexRow

| Offset | Length |  Type   | Description              |
| :----: | :----: | :-----: | ------------------------ |
|  0x00  |  0x02  | INTEGER | Index of UVIndex         |
|  0x02  |  0x02  | CHAR[2] | Identifier (`uI` / `Iu`) |
|  0x04  |  0x04  | LONGINT | Offset of UV indices     |
|        | 0x0008 |         | Size of UVIndexRow       |

If NID is 2 there is a VertexIndexRow and UVIndexRow.

The header is followed by NID tables.  Each table contains Number of Indices (specified in TrianglePart) SHORTINTs, which are the actual indices.

### Effect

| Offset | Length |  Type   | Description                                    |
| :----: | :----: | :-----: | ---------------------------------------------- |
|  0x00  |  0x04  | LONGINT | Unknown (seems always to be 0x05000000)        |
|  0x04  |  0x04  | LONGINT | Unknown (seems always to be 0x00000000)        |
|  0x08  |  0x04  |  REAL   | X-coordinate                                   |
|  0x0C  |  0x04  |  REAL   | Y-coordinate                                   |
|  0x10  |  0x04  |  REAL   | Z-coordinate                                   |
|  0x14  |  0x04  |  REAL   | Unknown (seems always to be 0x3F800000)        |
|  0x18  |  0x04  |  REAL   | X-coordinate width (relative to X-coordinate)  |
|  0x1C  |  0x04  |  REAL   | Y-coordinate width (relative to Y-coordinate)  |
|  0x20  |  0x04  |  REAL   | Z-coordinate width (relative to Z-coordinate)  |
|  0x24  |  0x04  |  REAL   | Unknown (seems always to be 0x00000000)        |
|  0x28  |  0x04  |  REAL   | X-coordinate height (relative to X-coordinate) |
|  0x2C  |  0x04  |  REAL   | Y-coordinate height (relative to Y-coordinate) |
|  0x30  |  0x04  |  REAL   | Z-coordinate height (relative to Z-coordinate) |
|  0x34  |  0x04  |  REAL   | Unknown (seems always to be 0x00000000)        |
|  0x38  |  0x04  |  REAL   | X-coordinate depth (relative to X-coordinate)  |
|  0x3C  |  0x04  |  REAL   | Y-coordinate depth (relative to Y-coordinate)  |
|  0x40  |  0x04  |  REAL   | Z-coordinate depth (relative to Z-coordinate)  |
|  0x44  |  0x04  |  REAL   | Unknown (seems always to be 0x00000000)        |
|  0x48  |  0x04  |  Color  | Color of glow (BGRA format)                    |
|  0x4C  |  0x04  |  Color  | Color of source (BGRA format)                  |
|  0x50  |  0x04  | LONGINT | Mirror (0x08000000 means mirrored)             |
|  0x54  |  0x04  |  ????   | Information                                    |
|        | 0x0058 |         | Size of Effect                                 |

## POD Types

- LONGINT: long integer (32-bit)
- INTEGER: integer (16-bit)
- SHORTINT: short interger (8-bit)
- REAL: floating point number (32-bit)

## Copyright

The hacking was done by Addict and Jesper-Juul Mortensen.  
I'm not responsible for any damage this info does to your properties. I'm in no way affiliated with EA.
