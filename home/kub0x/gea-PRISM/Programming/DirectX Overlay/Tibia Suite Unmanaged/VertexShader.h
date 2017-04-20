#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
// Parameters:
//
//   float4 g_diffuseColor;
//   float4 g_translationAndExtent;
//
//
// Registers:
//
//   Name                   Reg   Size
//   ---------------------- ----- ----
//   g_translationAndExtent c0       1
//   g_diffuseColor         c1       1
//

    vs_3_0
    def c2, -1, 1, 0, 0
    dcl_position v0
    dcl_texcoord v1
    dcl_color v2
    dcl_position o0
    dcl_texcoord o1.xy
    dcl_color o2
    rcp r0.x, c0.z
    add r0.yz, v0.xxyw, v0.xxyw
    mad o0.x, r0.y, r0.x, c2.x
    rcp r0.x, c0.w
    mad o0.y, r0.z, -r0.x, c2.y
    add r0.xy, c0.zwzw, c0.zwzw
    rcp r1.x, r0.x
    rcp r1.y, r0.y
    add o1.xy, r1, v1
    mul o2, c1, v2
    mov o0.zw, c2.xyzy

// approximately 11 instruction slots used
#endif

const BYTE g_vs30_VertexMain[] =
{
      0,   3, 254, 255, 254, 255, 
     46,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 130,   0, 
      0,   0,   0,   3, 254, 255, 
      2,   0,   0,   0,  28,   0, 
      0,   0,   0,   1,   0,   0, 
    123,   0,   0,   0,  68,   0, 
      0,   0,   2,   0,   1,   0, 
      1,   0,   0,   0,  84,   0, 
      0,   0,   0,   0,   0,   0, 
    100,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
     84,   0,   0,   0,   0,   0, 
      0,   0, 103,  95, 100, 105, 
    102, 102, 117, 115, 101,  67, 
    111, 108, 111, 114,   0, 171, 
      1,   0,   3,   0,   1,   0, 
      4,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 103,  95, 
    116, 114,  97, 110, 115, 108, 
     97, 116, 105, 111, 110,  65, 
    110, 100,  69, 120, 116, 101, 
    110, 116,   0, 118, 115,  95, 
     51,  95,  48,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  54,  46,  51, 
     46,  57,  54,  48,  48,  46, 
     49,  54,  51,  56,  52,   0, 
     81,   0,   0,   5,   2,   0, 
     15, 160,   0,   0, 128, 191, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   1,   0,  15, 144, 
     31,   0,   0,   2,  10,   0, 
      0, 128,   2,   0,  15, 144, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 224, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   1,   0,   3, 224, 
     31,   0,   0,   2,  10,   0, 
      0, 128,   2,   0,  15, 224, 
      6,   0,   0,   2,   0,   0, 
      1, 128,   0,   0, 170, 160, 
      2,   0,   0,   3,   0,   0, 
      6, 128,   0,   0, 208, 144, 
      0,   0, 208, 144,   4,   0, 
      0,   4,   0,   0,   1, 224, 
      0,   0,  85, 128,   0,   0, 
      0, 128,   2,   0,   0, 160, 
      6,   0,   0,   2,   0,   0, 
      1, 128,   0,   0, 255, 160, 
      4,   0,   0,   4,   0,   0, 
      2, 224,   0,   0, 170, 128, 
      0,   0,   0, 129,   2,   0, 
     85, 160,   2,   0,   0,   3, 
      0,   0,   3, 128,   0,   0, 
    238, 160,   0,   0, 238, 160, 
      6,   0,   0,   2,   1,   0, 
      1, 128,   0,   0,   0, 128, 
      6,   0,   0,   2,   1,   0, 
      2, 128,   0,   0,  85, 128, 
      2,   0,   0,   3,   1,   0, 
      3, 224,   1,   0, 228, 128, 
      1,   0, 228, 144,   5,   0, 
      0,   3,   2,   0,  15, 224, 
      1,   0, 228, 160,   2,   0, 
    228, 144,   1,   0,   0,   2, 
      0,   0,  12, 224,   2,   0, 
    100, 160, 255, 255,   0,   0
};
