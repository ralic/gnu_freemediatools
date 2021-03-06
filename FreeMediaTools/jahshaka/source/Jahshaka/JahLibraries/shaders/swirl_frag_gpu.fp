!!FP1.0
# NV_fragment_program generated by NVIDIA Cg compiler
# cgc version 1.2.0001, build date Feb 19 2004  12:16:25
# command line args: -profile fp30
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.xcenter : C0
#semantic main.ycenter : C1
#semantic main.swirl_radius : C2
#semantic main.twist_effect : C3
#semantic main.texture
#var float xcenter : C0 :  : 1 : 1
#var float ycenter : C1 :  : 2 : 1
#var float swirl_radius : C2 :  : 3 : 1
#var float twist_effect : C3 :  : 4 : 1
#var sampler2D texture :  : texunit 0 : 5 : 1
#var float2 uv : $vin.TEXCOORD0 : TEX0 : 0 : 1
#var half4 main : $vout.COLOR : COL : -1 : 1
DECLARE xcenter;
DECLARE ycenter;
DECLARE swirl_radius;
DECLARE twist_effect;
TEX R0, f[TEX0].xyxx, TEX0, 2D;
ADDR R1.x, f[TEX0].x, -xcenter.x;
ADDR R1.x, R1.x, {1e-07}.x;
ADDR R1.y, f[TEX0].y, -ycenter.x;
ADDR R1.y, R1.y, {1e-07}.x;
SLTR H0.x, R1.y, R1.x;
SLER H0.y, -R1.x, R1.y;
MULR R1.z, H0.y, H0.x;
MOVR R2.x, R1.z;
SLTR H0.x, R1.x, R1.y;
SLER H0.y, R1.y, -R1.x;
MULR R1.z, H0.y, H0.x;
MOVR R2.y, R1.z;
SLER H0.x, R1.x, R1.y;
SLTR H0.y, R1.x, -R1.y;
SLER H0.z, R1.y, R1.x;
MULR R1.z, H0.z, H0.y;
SGER H0.y, R1.y, {0}.x;
MADR R1.w, H0.y, {2}.x, {-1}.x;
RCPR R3.x, R1.x;
MULR R3.x, R1.y, R3.x;
MOVR R4.x, R3.x;
MOVR R4.y, R3.x;
ADDR R3.x, R1.x, R1.y;
SLTR H0.y, {0}.x, R3.x;
MULR R3.x, H0.x, H0.y;
MOVR R2.z, R3.x;
MOVR R2.w, R1.z;
RCPR R1.z, R1.y;
MULR R1.z, -R1.x, R1.z;
MOVR R4.z, R1.z;
MOVR R3.xyz, R4.xyzx;
MOVR R3.w, R4.z;
DP4R R1.z, R3, R2;
DP4R R2.x, {0, 3.1415927, 1.5707964, 1.5707964}, R2;
MULR R2.x, R1.w, R2.x;
MULR R1.w, {-0.038986512}.x, R1.z;
MADR R1.w, R1.w, R1.z, {0.14626446}.x;
MULR R1.w, R1.w, R1.z;
MADR R1.w, R1.w, R1.z, {-0.32117498}.x;
MULR R1.w, R1.w, R1.z;
MADR R1.w, R1.w, R1.z, {0.99921381}.x;
MADR R2.x, R1.w, R1.z, R2.x;
MULR R1.y, R1.y, R1.y;
MADR R1.y, R1.x, R1.x, R1.y;
RSQR R1.y, R1.y;
RCPR R1.y, R1.y;
MOVR R1.x, swirl_radius.x;
MADR R1.x, -{1}.x, R1.y, R1.x;
MADR R2.x, -R1.x, twist_effect.x, R2.x;
MULR R1.y, {1}.x, R1.y;
SLTR H0.x, R1.y, swirl_radius.x;
ADDX H0.x, -H0.x, {1}.x;
COSR R1.x, R2.x;
MADR R1.x, R1.y, R1.x, xcenter.x;
SINR R2.x, R2.x;
MADR R2.x, R1.y, R2.x, ycenter.x;
MOVR R1.y, R2.x;
TEX R1.xyz, R1.xyxx, TEX0, 2D;
MOVXC HC.x, H0.x;
MOVR R1.xyz(GT.x), R0;
MOVR o[COLH].xyz, R1.xyzx;
MOVH o[COLH].w, {0, 0, 0, 1}.w;
END
# 63 instructions, 5 R-regs, 1 H-regs.
# End of program
