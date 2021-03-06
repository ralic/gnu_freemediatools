!!FP1.0
# cgc version 1.3.0001, build date Aug  4 2004 10:01:10
# command line args: -profile fp30
# source file: blur15_frag_gpu.cg
#vendor NVIDIA Corporation
#version 1.0.02
#profile fp30
#program main
#semantic main.horizontal : C0
#semantic main.vertical : C1
#semantic main.stretch : C2
#semantic main.texture : TEXUNIT0
#var float2 position : $vin.TEX0 : TEX0 : 0 : 1
#var float horizontal : C0 : c[0] : 1 : 1
#var float vertical : C1 : c[1] : 2 : 1
#var float stretch : C2 : c[2] : 3 : 1
#var sampler2D texture : TEXUNIT0 : texunit 0 : 4 : 1
#var half4 main : $vout.COL : COL : -1 : 1
DECLARE horizontal;
DECLARE vertical;
DECLARE stretch;
MOVR  R2.z, vertical.x;
MOVR  R2.y, horizontal.x;
SLTR  H0.x, {0}, R2.y;
SLTR  H2.x, {0}, R2.z;
ADDX_SAT H2.y, H0.x, H2.x;
MULX  H7.x, H2.y, H0;
MULX  H6.w, H2.y, H2.x;
MOVR  R2.x, f[TEX0];
MOVRC RC.x, H7;
ADDR  R2.x(GT), f[TEX0], stretch;
MOVRC RC.x, H7;
MOVR  R0.x, R2;
MOVR  R0.y, f[TEX0];
TEX   R1, R0, TEX0, 2D;
TEX   R0, f[TEX0], TEX0, 2D;
MOVH  H1, R0;
ADDH  H1(GT.x), R0, R1;
MOVRC RC.x, H7;
MOVR  R1.x, f[TEX0];
ADDR  R1.x(GT), f[TEX0], -stretch;
MOVRC RC.x, H7;
MOVH  H0, H1;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H6.w;
MOVR  R1.y, f[TEX0];
ADDR  R1.y(GT.x), f[TEX0], stretch.x;
MOVRC RC.x, H6.w;
MOVH  H1, H0;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H6.w;
MOVR  R1.z, f[TEX0].y;
ADDR  R1.z(GT.x), f[TEX0].y, -stretch.x;
MOVRC RC.x, H6.w;
MOVH  H0, H1;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVR  R1.w, R2.x;
SLTR  H1.x, {1}, R2.y;
SLTR  H2.x, {1}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H6.z, H2.y, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H6.z;
MULX  H6.y, H2, H2.x;
ADDR  R1.w(GT.x), R2.x, stretch.x;
MOVRC RC.x, H6.z;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H6.z;
ADDR  R2.x(GT), R1, -stretch;
MOVRC RC.x, H6.z;
MOVH  H0, H1;
MOVR  R0.x, R2;
MOVR  R0.y, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H6.y;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H6.y;
MOVH  H1, H0;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H6.y;
ADDR  R1.y(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H6.y;
MOVH  H0, H1;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {2}, R2.y;
SLTR  H2.x, {2}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H6.x, H2.y, H1;
MULX  H5.w, H2.y, H2.x;
MOVH  H1, H0;
MOVRC RC.x, H6;
ADDR  R1.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H6;
MOVR  R0.x, R1.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H6;
ADDR  R1.w(GT.x), R2.x, -stretch.x;
MOVRC RC.x, H6;
MOVH  H0, H1;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H5.w;
ADDR  R2.x(GT), R1, stretch;
MOVRC RC.x, H5.w;
MOVH  H1, H0;
MOVR  R0.y, R2.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H5.w;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H5.w;
MOVH  H0, H1;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {3}, R2.y;
SLTR  H2.x, {3}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H5.z, H2.y, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H5.z;
MULX  H5.y, H2, H2.x;
ADDR  R1.y(GT.x), R1.z, stretch.x;
MOVRC RC.x, H5.z;
MOVR  R0.x, R1.y;
MOVR  R0.y, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H5.z;
ADDR  R1.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H5.z;
MOVH  H0, H1;
MOVR  R0.x, R1.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H5.y;
ADDR  R1.w(GT.x), R2.x, stretch.x;
MOVRC RC.x, H5.y;
MOVH  H1, H0;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H5.y;
ADDR  R2.x(GT), R1, -stretch;
MOVRC RC.x, H5.y;
MOVH  H0, H1;
MOVR  R0.y, R2.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {4}, R2.y;
SLTR  H2.x, {4}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H5.x, H2.y, H1;
MULX  H4.w, H2.y, H2.x;
MOVH  H1, H0;
MOVRC RC.x, H5;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H5;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H5;
ADDR  R1.y(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H5;
MOVH  H0, H1;
MOVR  R0.x, R1.y;
MOVR  R0.y, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H4.w;
ADDR  R1.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H4.w;
MOVH  H1, H0;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H4.w;
ADDR  R1.w(GT.x), R2.x, -stretch.x;
MOVRC RC.x, H4.w;
MOVH  H0, H1;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {5}, R2.y;
SLTR  H2.x, {5}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H4.z, H2.y, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H4.z;
MULX  H4.y, H2, H2.x;
ADDR  R2.x(GT), R1, stretch;
MOVRC RC.x, H4.z;
MOVR  R0.x, R2;
MOVR  R0.y, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H4.z;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H4.z;
MOVH  H0, H1;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H4.y;
ADDR  R1.y(GT.x), R1.z, stretch.x;
MOVRC RC.x, H4.y;
MOVH  H1, H0;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H4.y;
ADDR  R1.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H4.y;
MOVH  H0, H1;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {6}, R2.y;
SLTR  H2.x, {6}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H4.x, H2.y, H1;
MULX  H3.w, H2.y, H2.x;
MOVH  H1, H0;
MOVRC RC.x, H4;
ADDR  R1.w(GT.x), R2.x, stretch.x;
MOVRC RC.x, H4;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H4;
ADDR  R2.x(GT), R1, -stretch;
MOVRC RC.x, H4;
MOVH  H0, H1;
MOVR  R0.x, R2;
MOVR  R0.y, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H3.w;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H3.w;
MOVH  H1, H0;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H3.w;
ADDR  R1.y(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H3.w;
MOVH  H0, H1;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {7}, R2.y;
SLTR  H2.x, {7}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H3.z, H2.y, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H3.z;
MULX  H3.y, H2, H2.x;
ADDR  R1.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H3.z;
MOVR  R0.x, R1.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H3.z;
ADDR  R1.w(GT.x), R2.x, -stretch.x;
MOVRC RC.x, H3.z;
MOVH  H0, H1;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H3.y;
ADDR  R2.x(GT), R1, stretch;
MOVRC RC.x, H3.y;
MOVH  H1, H0;
MOVR  R0.y, R2.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H3.y;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H3.y;
MOVH  H0, H1;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {8}, R2.y;
SLTR  H2.x, {8}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H3.x, H2.y, H1;
MULX  H2.w, H2.y, H2.x;
MOVH  H1, H0;
MOVRC RC.x, H3;
ADDR  R1.y(GT.x), R1.z, stretch.x;
MOVRC RC.x, H3;
MOVR  R0.x, R1.y;
MOVR  R0.y, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H3;
ADDR  R1.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H3;
MOVH  H0, H1;
MOVR  R0.x, R1.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H2.w;
ADDR  R1.w(GT.x), R2.x, stretch.x;
MOVRC RC.x, H2.w;
MOVH  H1, H0;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H2.w;
ADDR  R2.x(GT), R1, -stretch;
MOVRC RC.x, H2.w;
MOVH  H0, H1;
MOVR  R0.y, R2.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {9}, R2.y;
SLTR  H2.x, {9}, R2.z;
ADDX_SAT H2.y, H1.x, H2.x;
MULX  H2.z, H2.y, H1.x;
MOVH  H1, H0;
MOVRC RC.x, H2.z;
MULX  H2.y, H2, H2.x;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H2.z;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H2.z;
ADDR  R1.y(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H2.z;
MOVH  H0, H1;
MOVR  R0.x, R1.y;
MOVR  R0.y, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H2.y;
ADDR  R1.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H2.y;
MOVH  H1, H0;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H2.y;
ADDR  R1.w(GT.x), R2.x, -stretch.x;
MOVRC RC.x, H2.y;
MOVH  H0, H1;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {10}, R2.y;
SLTR  H7.y, {10}.x, R2.z;
ADDX_SAT H7.z, H1.x, H7.y;
MULX  H2.x, H7.z, H1;
MULX  H7.y, H7.z, H7;
MOVH  H1, H0;
MOVRC RC.x, H2;
ADDR  R2.x(GT), R1, stretch;
MOVRC RC.x, H2;
MOVR  R0.x, R2;
MOVR  R0.y, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H2;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H2;
MOVH  H0, H1;
MOVR  R0.x, R1;
MOVR  R0.y, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H7.y;
ADDR  R1.y(GT.x), R1.z, stretch.x;
MOVRC RC.x, H7.y;
MOVH  H1, H0;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H7.y;
ADDR  R1.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H7.y;
MOVH  H0, H1;
MOVR  R0.y, R1.z;
MOVR  R0.x, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {11}, R2.y;
SLTR  H7.w, {11}.x, R2.z;
ADDX_SAT H8.x, H1, H7.w;
MULX  H7.z, H8.x, H1.x;
MULX  H7.w, H8.x, H7;
MOVH  H1, H0;
MOVRC RC.x, H7.z;
ADDR  R1.w(GT.x), R2.x, stretch.x;
MOVRC RC.x, H7.z;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H7.z;
ADDR  R2.x(GT), R1, -stretch;
MOVRC RC.x, H7.z;
MOVH  H0, H1;
MOVR  R0.x, R2;
MOVR  R0.y, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H7.w;
ADDR  R1.x(GT), R1.y, stretch;
MOVRC RC.x, H7.w;
MOVH  H1, H0;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H7.w;
ADDR  R1.y(GT.x), R1.z, -stretch.x;
MOVRC RC.x, H7.w;
MOVH  H0, H1;
MOVR  R0.y, R1;
MOVR  R0.x, f[TEX0];
MOVR  R1.z, R1.w;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H1.x, {12}, R2.y;
SLTR  H8.y, {12}.x, R2.z;
ADDX_SAT H8.z, H1.x, H8.y;
MULX  H8.x, H8.z, H1;
MOVH  H1, H0;
MOVRC RC.x, H8;
MULX  H8.y, H8.z, H8;
ADDR  R1.z(GT.x), R1.w, stretch.x;
MOVRC RC.x, H8;
MOVR  R0.x, R1.z;
MOVR  R0.y, f[TEX0];
MOVR  R1.w, R2.x;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H8;
ADDR  R1.w(GT.x), R2.x, -stretch.x;
MOVRC RC.x, H8;
MOVH  H0, H1;
MOVR  R0.x, R1.w;
MOVR  R0.y, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H8.y;
ADDR  R2.x(GT), R1, stretch;
MOVRC RC.x, H8.y;
MOVH  H1, H0;
MOVR  R0.y, R2.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.x, R1.y;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H8.y;
ADDR  R1.x(GT), R1.y, -stretch;
MOVRC RC.x, H8.y;
MOVH  H0, H1;
MOVR  R0.y, R1.x;
MOVR  R0.x, f[TEX0];
MOVR  R1.y, R1.z;
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
SLTR  H8.w, {13}.x, R2.z;
SLTR  H1.x, {13}, R2.y;
ADDX_SAT H9.x, H1, H8.w;
MULX  H8.z, H9.x, H1.x;
MULX  H8.w, H9.x, H8;
MOVH  H1, H0;
MOVRC RC.x, H8.z;
ADDR  R1.y(GT.x), R1.z, stretch.x;
MOVRC RC.x, H8.z;
MOVR  R1.z, f[TEX0].y;
TEX   R0, R1.yzzw, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H8.z;
MOVR  R1.z, R1.w;
ADDR  R1.z(GT.x), R1.w, -stretch.x;
MOVRC RC.x, H8.z;
MOVR  R1.w, f[TEX0].y;
TEX   R0, R1.zwzw, TEX0, 2D;
MOVH  H0, H1;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H8.w;
MOVR  R1.w, R2.x;
ADDR  R1.w(GT.x), R2.x, stretch.x;
MOVRC RC.x, H8.w;
MOVH  H1, H0;
MOVR  R0.y, R1.w;
MOVR  R0.x, f[TEX0];
MOVR  R2.x, R1;
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H8.w;
ADDR  R2.x(GT), R1, -stretch;
MOVRC RC.x, H8.w;
MOVH  H0, H1;
MOVR  R0.y, R2.x;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVH  H1, H0;
ADDR  R0.x, R1.y, stretch;
MOVR  R0.y, f[TEX0];
SLTR  H9.x, {14}, R2.y;
SLTR  H9.y, {14}.x, R2.z;
ADDX_SAT H9.z, H9.x, H9.y;
MULX  H9.x, H9.z, H9;
TEX   R0, R0, TEX0, 2D;
MOVRC RC.x, H9;
MULX  H9.y, H9.z, H9;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H9;
MOVH  H0, H1;
ADDR  R0.x, R1.z, -stretch;
MOVR  R0.y, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H9.y;
MOVH  H1, H0;
ADDR  R0.y, R1.w, stretch.x;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H1(GT.x), H0, R0;
MOVRC RC.x, H9.y;
MOVH  H0, H1;
ADDR  R0.y, R2.x, -stretch.x;
MOVR  R0.x, f[TEX0];
TEX   R0, R0, TEX0, 2D;
ADDH  H0(GT.x), H1, R0;
MOVRC RC.x, H7;
MOVR  R0.y, {1}.x;
MOVR  R0.y(LE.x), {0}.x;
MOVRC RC.x, H6.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H6.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H6.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H6;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H5.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H5.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H5.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H5;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H4.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H4.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H4.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H4;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H3.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H3.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H3.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H3;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H2.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H2.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H2.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H2;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H7.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H7.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H7.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H8;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H8.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H8.z;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H8.w;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MOVRC RC.x, H9;
MOVR  R0.y, R0.x;
ADDR  R0.y(GT.x), R0.x, {1}.x;
MOVRC RC.x, H9.y;
MOVR  R0.x, R0.y;
ADDR  R0.x(GT), R0.y, {1};
MADR  R0.y, R0.x, {2, 1}.x, {2, 1};
MOVH  o[COLH], H0;
RCPR  R0.y, R0.y;
MOVRC RC.x, R0;
MULR  o[COLH](GT.x), H0, R0.y;
END
# 699 instructions, 3 R-regs, 10 H-regs
