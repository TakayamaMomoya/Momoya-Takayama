xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 287;
 0.00000;200000.00298;0.05010;,
 0.00000;196157.04042;-39018.00683;,
 14931.56741;196157.04042;-36047.91304;,
 0.00000;200000.00298;0.05010;,
 27589.93791;196157.04042;-27589.87854;,
 0.00000;200000.00298;0.05010;,
 36047.99429;196157.04042;-14931.49866;,
 0.00000;200000.00298;0.05010;,
 39018.06621;196157.04042;0.05010;,
 0.00000;200000.00298;0.05010;,
 36047.98804;196157.04042;14931.63460;,
 0.00000;200000.00298;0.05010;,
 27589.93791;196157.04042;27590.01916;,
 0.00000;200000.00298;0.05010;,
 14931.57053;196157.04042;36048.05679;,
 0.00000;200000.00298;0.05010;,
 0.00589;196157.04042;39018.15058;,
 0.00000;200000.00298;0.05010;,
 -14931.55803;196157.04042;36048.05679;,
 0.00000;200000.00298;0.05010;,
 -27589.92854;196157.04042;27590.01916;,
 0.00000;200000.00298;0.05010;,
 -36047.98804;196157.04042;14931.63460;,
 0.00000;200000.00298;0.05010;,
 -39018.06621;196157.04042;0.05010;,
 0.00000;200000.00298;0.05010;,
 -36047.98804;196157.04042;-14931.52991;,
 0.00000;200000.00298;0.05010;,
 -27589.92854;196157.04042;-27589.87854;,
 0.00000;200000.00298;0.05010;,
 -14931.54866;196157.04042;-36047.91304;,
 0.00000;200000.00298;0.05010;,
 0.00000;196157.04042;-39018.00683;,
 0.00000;184775.90275;-76536.61989;,
 29289.32544;184775.90275;-70710.61355;,
 54119.61956;184775.90275;-54119.54456;,
 70710.68855;184775.90275;-29289.25044;,
 76536.68864;184775.90275;0.05010;,
 70710.68855;184775.90275;29289.39106;,
 54119.61331;184775.90275;54119.66956;,
 29289.32856;184775.90275;70710.73230;,
 0.01156;184775.90275;76536.78239;,
 -29289.30981;184775.90275;70710.73230;,
 -54119.60081;184775.90275;54119.72581;,
 -70710.67605;184775.90275;29289.39106;,
 -76536.68864;184775.90275;0.05010;,
 -70710.67605;184775.90275;-29289.28169;,
 -54119.59456;184775.90275;-54119.57581;,
 -29289.29106;184775.90275;-70710.63855;,
 0.00000;184775.90275;-76536.61989;,
 0.00000;166293.91498;-111113.98916;,
 42521.50688;166293.91498;-102655.93278;,
 78569.50742;166293.91498;-78569.43242;,
 102656.00153;166293.91498;-42521.42876;,
 111114.05166;166293.91498;0.05010;,
 102656.00153;166293.91498;42521.56938;,
 78569.49492;166293.91498;78569.55742;,
 42521.51313;166293.91498;102656.02653;,
 0.01678;166293.91498;111114.06416;,
 -42521.48501;166293.91498;102656.08903;,
 -78569.47617;166293.91498;78569.61992;,
 -102656.00153;166293.91498;42521.59751;,
 -111114.05166;166293.91498;0.05010;,
 -102655.98278;166293.91498;-42521.46001;,
 -78569.47617;166293.91498;-78569.45742;,
 -42521.45688;166293.91498;-102655.96403;,
 0.00000;166293.91498;-111113.98916;,
 0.00000;141421.35211;-141421.28961;,
 54119.61331;141421.35211;-130656.27695;,
 100000.00149;141421.35211;-99999.90774;,
 130656.30195;141421.35211;-54119.54456;,
 141421.35211;141421.35211;0.05010;,
 130656.30195;141421.35211;54119.66956;,
 100000.00149;141421.35211;100000.08274;,
 54119.61956;141421.35211;130656.31445;,
 0.02135;141421.35211;141421.36461;,
 -54119.58206;141421.35211;130656.31445;,
 -99999.97024;141421.35211;100000.08274;,
 -130656.30195;141421.35211;54119.72581;,
 -141421.35211;141421.35211;0.05010;,
 -130656.30195;141421.35211;-54119.57581;,
 -99999.96399;141421.35211;-99999.97024;,
 -54119.54456;141421.35211;-130656.30195;,
 0.00000;141421.35211;-141421.28961;,
 0.00000;111114.03916;-166293.87748;,
 63637.93845;111114.03916;-153635.48979;,
 117587.57675;111114.03916;-117587.50175;,
 153635.56479;111114.03916;-63637.85095;,
 166293.92748;111114.03916;0.05010;,
 153635.55229;111114.03916;63638.03845;,
 117587.57675;111114.03916;117587.57675;,
 63637.94470;111114.03916;153635.53979;,
 0.02511;111114.03916;166293.92748;,
 -63637.89470;111114.03916;153635.60229;,
 -117587.53925;111114.03916;117587.62675;,
 -153635.55229;111114.03916;63638.03845;,
 -166293.92748;111114.03916;0.05010;,
 -153635.55229;111114.03916;-63637.88220;,
 -117587.53925;111114.03916;-117587.53925;,
 -63637.85720;111114.03916;-153635.52729;,
 0.00000;111114.03916;-166293.87748;,
 0.00000;76536.68239;-184775.84025;,
 70710.67605;76536.68239;-170710.62754;,
 130656.30195;76536.68239;-130656.27695;,
 170710.67754;76536.68239;-70710.61355;,
 184775.90275;76536.68239;0.08062;,
 170710.67754;76536.68239;70710.73230;,
 130656.30195;76536.68239;130656.31445;,
 70710.68855;76536.68239;170710.67754;,
 0.02790;76536.68239;184775.91525;,
 -70710.63230;76536.68239;170710.72754;,
 -130656.25195;76536.68239;130656.37695;,
 -170710.66504;76536.68239;70710.79480;,
 -184775.90275;76536.68239;0.05010;,
 -170710.66504;76536.68239;-70710.63855;,
 -130656.25195;76536.68239;-130656.30195;,
 -70710.59480;76536.68239;-170710.65254;,
 0.00000;76536.68239;-184775.84025;,
 0.00000;39018.06933;-196157.00292;,
 75066.05737;39018.06933;-181225.42770;,
 138703.97707;39018.06933;-138703.91457;,
 181225.47770;39018.06933;-75065.98237;,
 196157.04042;39018.06933;0.08062;,
 181225.47770;39018.06933;75066.13862;,
 138703.97707;39018.06933;138704.01457;,
 75066.06362;39018.06933;181225.50270;,
 0.02962;39018.06933;196157.07792;,
 -75066.01362;39018.06933;181225.50270;,
 -138703.95207;39018.06933;138704.01457;,
 -181225.46520;39018.06933;75066.13862;,
 -196157.04042;39018.06933;0.05010;,
 -181225.46520;39018.06933;-75066.01362;,
 -138703.93957;39018.06933;-138703.97707;,
 -75065.96362;39018.06933;-181225.49020;,
 0.00000;39018.06933;-196157.00292;,
 0.00000;0.01510;-199999.96548;,
 76536.68864;0.01510;-184775.84025;,
 141421.35211;0.01510;-141421.28961;,
 184775.90275;0.01510;-76536.61989;,
 200000.00298;0.01510;0.08062;,
 184775.90275;0.01510;76536.78239;,
 141421.35211;0.01510;141421.36461;,
 76536.70114;0.01510;184775.91525;,
 0.03020;0.01510;200000.04048;,
 -76536.64489;0.01510;184775.91525;,
 -141421.32711;0.01510;141421.42711;,
 -184775.90275;0.01510;76536.78239;,
 -200000.00298;0.01510;0.05010;,
 -184775.90275;0.01510;-76536.65114;,
 -141421.31461;0.01510;-141421.35211;,
 -76536.60114;0.01510;-184775.90275;,
 0.00000;0.01510;-199999.96548;,
 0.00000;-39018.04121;-196157.00292;,
 75066.06362;-39018.04121;-181225.46520;,
 138704.00207;-39018.04121;-138703.93957;,
 181225.50270;-39018.04121;-75065.98237;,
 196157.06542;-39018.04121;0.08062;,
 181225.50270;-39018.04121;75066.13862;,
 138704.00207;-39018.04121;138704.01457;,
 75066.06987;-39018.04121;181225.50270;,
 0.02962;-39018.04121;196157.07792;,
 -75066.01987;-39018.04121;181225.50270;,
 -138703.95207;-39018.04121;138704.01457;,
 -181225.47770;-39018.04121;75066.13862;,
 -196157.06542;-39018.04121;0.05010;,
 -181225.47770;-39018.04121;-75066.01362;,
 -138703.95207;-39018.04121;-138703.97707;,
 -75065.96987;-39018.04121;-181225.49020;,
 0.00000;-39018.04121;-196157.00292;,
 0.00000;-76536.65114;-184775.87775;,
 70710.68855;-76536.65114;-170710.62754;,
 130656.30195;-76536.65114;-130656.27695;,
 170710.67754;-76536.65114;-70710.61355;,
 184775.91525;-76536.65114;0.08062;,
 170710.67754;-76536.65114;70710.79480;,
 130656.30195;-76536.65114;130656.31445;,
 70710.69480;-76536.65114;170710.67754;,
 0.02790;-76536.65114;184775.91525;,
 -70710.63855;-76536.65114;170710.72754;,
 -130656.26445;-76536.65114;130656.37695;,
 -170710.67754;-76536.65114;70710.79480;,
 -184775.91525;-76536.65114;0.05010;,
 -170710.67754;-76536.65114;-70710.63855;,
 -130656.25195;-76536.65114;-130656.33945;,
 -70710.60105;-76536.65114;-170710.69004;,
 0.00000;-76536.65114;-184775.87775;,
 0.00000;-111114.03916;-166293.87748;,
 63637.93845;-111114.03916;-153635.48979;,
 117587.57675;-111114.03916;-117587.50175;,
 153635.56479;-111114.03916;-63637.85095;,
 166293.92748;-111114.03916;0.05010;,
 153635.55229;-111114.03916;63638.03845;,
 117587.57675;-111114.03916;117587.57675;,
 63637.94470;-111114.03916;153635.53979;,
 0.02511;-111114.03916;166293.92748;,
 -63637.89470;-111114.03916;153635.60229;,
 -117587.53925;-111114.03916;117587.62675;,
 -153635.55229;-111114.03916;63638.03845;,
 -166293.92748;-111114.03916;0.05010;,
 -153635.55229;-111114.03916;-63637.88220;,
 -117587.53925;-111114.03916;-117587.53925;,
 -63637.85720;-111114.03916;-153635.52729;,
 0.00000;-111114.03916;-166293.87748;,
 0.00000;-141421.35211;-141421.28961;,
 54119.61331;-141421.35211;-130656.27695;,
 100000.00149;-141421.35211;-99999.90774;,
 130656.30195;-141421.35211;-54119.54456;,
 141421.35211;-141421.35211;0.05010;,
 130656.30195;-141421.35211;54119.66956;,
 100000.00149;-141421.35211;100000.08274;,
 54119.61956;-141421.35211;130656.31445;,
 0.02135;-141421.35211;141421.36461;,
 -54119.58206;-141421.35211;130656.31445;,
 -99999.97024;-141421.35211;100000.08274;,
 -130656.30195;-141421.35211;54119.72581;,
 -141421.35211;-141421.35211;0.05010;,
 -130656.30195;-141421.35211;-54119.57581;,
 -99999.96399;-141421.35211;-99999.97024;,
 -54119.54456;-141421.35211;-130656.30195;,
 0.00000;-141421.35211;-141421.28961;,
 0.00000;-166293.92748;-111113.97666;,
 42521.50376;-166293.92748;-102655.93278;,
 78569.48867;-166293.92748;-78569.43242;,
 102655.98278;-166293.92748;-42521.42876;,
 111114.03916;-166293.92748;0.05010;,
 102655.98278;-166293.92748;42521.56938;,
 78569.48867;-166293.92748;78569.55742;,
 42521.51313;-166293.92748;102656.02653;,
 0.01678;-166293.92748;111114.06416;,
 -42521.47876;-166293.92748;102656.02653;,
 -78569.46992;-166293.92748;78569.55742;,
 -102655.98278;-166293.92748;42521.56938;,
 -111114.03916;-166293.92748;0.05010;,
 -102655.98278;-166293.92748;-42521.46001;,
 -78569.46992;-166293.92748;-78569.43242;,
 -42521.45063;-166293.92748;-102655.94528;,
 0.00000;-166293.92748;-111113.97666;,
 0.00000;-184775.92775;-76536.59489;,
 29289.30981;-184775.92775;-70710.58230;,
 54119.58831;-184775.92775;-54119.51331;,
 70710.65105;-184775.92775;-29289.25044;,
 76536.65114;-184775.92775;0.05010;,
 70710.65105;-184775.92775;29289.39106;,
 54119.58831;-184775.92775;54119.66956;,
 29289.31606;-184775.92775;70710.73230;,
 0.01156;-184775.92775;76536.71989;,
 -29289.29419;-184775.92775;70710.73230;,
 -54119.56956;-184775.92775;54119.66956;,
 -70710.65105;-184775.92775;29289.39106;,
 -76536.65114;-184775.92775;0.05010;,
 -70710.65105;-184775.92775;-29289.25044;,
 -54119.56956;-184775.92775;-54119.54456;,
 -29289.27544;-184775.92775;-70710.61355;,
 0.00000;-184775.92775;-76536.59489;,
 0.00000;-196157.06542;-39017.94433;,
 14931.54866;-196157.06542;-36047.88179;,
 27589.90666;-196157.06542;-27589.84729;,
 36047.94741;-196157.06542;-14931.49866;,
 39018.01621;-196157.06542;0.05010;,
 36047.94741;-196157.06542;14931.60335;,
 27589.90666;-196157.06542;27589.98791;,
 14931.55178;-196157.06542;36048.02554;,
 0.00589;-196157.06542;39018.08808;,
 -14931.54085;-196157.06542;36048.02554;,
 -27589.89729;-196157.06542;27589.98791;,
 -36047.94429;-196157.06542;14931.60335;,
 -39018.01621;-196157.06542;0.05010;,
 -36047.94429;-196157.06542;-14931.49866;,
 -27589.89729;-196157.06542;-27589.84729;,
 -14931.53147;-196157.06542;-36047.88179;,
 0.00000;-196157.06542;-39017.94433;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;,
 0.00000;-200000.00298;0.05010;;
 
 256;
 3;0,1,2;,
 3;3,2,4;,
 3;5,4,6;,
 3;7,6,8;,
 3;9,8,10;,
 3;11,10,12;,
 3;13,12,14;,
 3;15,14,16;,
 3;17,16,18;,
 3;19,18,20;,
 3;21,20,22;,
 3;23,22,24;,
 3;25,24,26;,
 3;27,26,28;,
 3;29,28,30;,
 3;31,30,32;,
 4;1,33,34,2;,
 4;2,34,35,4;,
 4;4,35,36,6;,
 4;6,36,37,8;,
 4;8,37,38,10;,
 4;10,38,39,12;,
 4;12,39,40,14;,
 4;14,40,41,16;,
 4;16,41,42,18;,
 4;18,42,43,20;,
 4;20,43,44,22;,
 4;22,44,45,24;,
 4;24,45,46,26;,
 4;26,46,47,28;,
 4;28,47,48,30;,
 4;30,48,49,32;,
 4;33,50,51,34;,
 4;34,51,52,35;,
 4;35,52,53,36;,
 4;36,53,54,37;,
 4;37,54,55,38;,
 4;38,55,56,39;,
 4;39,56,57,40;,
 4;40,57,58,41;,
 4;41,58,59,42;,
 4;42,59,60,43;,
 4;43,60,61,44;,
 4;44,61,62,45;,
 4;45,62,63,46;,
 4;46,63,64,47;,
 4;47,64,65,48;,
 4;48,65,66,49;,
 4;50,67,68,51;,
 4;51,68,69,52;,
 4;52,69,70,53;,
 4;53,70,71,54;,
 4;54,71,72,55;,
 4;55,72,73,56;,
 4;56,73,74,57;,
 4;57,74,75,58;,
 4;58,75,76,59;,
 4;59,76,77,60;,
 4;60,77,78,61;,
 4;61,78,79,62;,
 4;62,79,80,63;,
 4;63,80,81,64;,
 4;64,81,82,65;,
 4;65,82,83,66;,
 4;67,84,85,68;,
 4;68,85,86,69;,
 4;69,86,87,70;,
 4;70,87,88,71;,
 4;71,88,89,72;,
 4;72,89,90,73;,
 4;73,90,91,74;,
 4;74,91,92,75;,
 4;75,92,93,76;,
 4;76,93,94,77;,
 4;77,94,95,78;,
 4;78,95,96,79;,
 4;79,96,97,80;,
 4;80,97,98,81;,
 4;81,98,99,82;,
 4;82,99,100,83;,
 4;84,101,102,85;,
 4;85,102,103,86;,
 4;86,103,104,87;,
 4;87,104,105,88;,
 4;88,105,106,89;,
 4;89,106,107,90;,
 4;90,107,108,91;,
 4;91,108,109,92;,
 4;92,109,110,93;,
 4;93,110,111,94;,
 4;94,111,112,95;,
 4;95,112,113,96;,
 4;96,113,114,97;,
 4;97,114,115,98;,
 4;98,115,116,99;,
 4;99,116,117,100;,
 4;101,118,119,102;,
 4;102,119,120,103;,
 4;103,120,121,104;,
 4;104,121,122,105;,
 4;105,122,123,106;,
 4;106,123,124,107;,
 4;107,124,125,108;,
 4;108,125,126,109;,
 4;109,126,127,110;,
 4;110,127,128,111;,
 4;111,128,129,112;,
 4;112,129,130,113;,
 4;113,130,131,114;,
 4;114,131,132,115;,
 4;115,132,133,116;,
 4;116,133,134,117;,
 4;118,135,136,119;,
 4;119,136,137,120;,
 4;120,137,138,121;,
 4;121,138,139,122;,
 4;122,139,140,123;,
 4;123,140,141,124;,
 4;124,141,142,125;,
 4;125,142,143,126;,
 4;126,143,144,127;,
 4;127,144,145,128;,
 4;128,145,146,129;,
 4;129,146,147,130;,
 4;130,147,148,131;,
 4;131,148,149,132;,
 4;132,149,150,133;,
 4;133,150,151,134;,
 4;135,152,153,136;,
 4;136,153,154,137;,
 4;137,154,155,138;,
 4;138,155,156,139;,
 4;139,156,157,140;,
 4;140,157,158,141;,
 4;141,158,159,142;,
 4;142,159,160,143;,
 4;143,160,161,144;,
 4;144,161,162,145;,
 4;145,162,163,146;,
 4;146,163,164,147;,
 4;147,164,165,148;,
 4;148,165,166,149;,
 4;149,166,167,150;,
 4;150,167,168,151;,
 4;152,169,170,153;,
 4;153,170,171,154;,
 4;154,171,172,155;,
 4;155,172,173,156;,
 4;156,173,174,157;,
 4;157,174,175,158;,
 4;158,175,176,159;,
 4;159,176,177,160;,
 4;160,177,178,161;,
 4;161,178,179,162;,
 4;162,179,180,163;,
 4;163,180,181,164;,
 4;164,181,182,165;,
 4;165,182,183,166;,
 4;166,183,184,167;,
 4;167,184,185,168;,
 4;169,186,187,170;,
 4;170,187,188,171;,
 4;171,188,189,172;,
 4;172,189,190,173;,
 4;173,190,191,174;,
 4;174,191,192,175;,
 4;175,192,193,176;,
 4;176,193,194,177;,
 4;177,194,195,178;,
 4;178,195,196,179;,
 4;179,196,197,180;,
 4;180,197,198,181;,
 4;181,198,199,182;,
 4;182,199,200,183;,
 4;183,200,201,184;,
 4;184,201,202,185;,
 4;186,203,204,187;,
 4;187,204,205,188;,
 4;188,205,206,189;,
 4;189,206,207,190;,
 4;190,207,208,191;,
 4;191,208,209,192;,
 4;192,209,210,193;,
 4;193,210,211,194;,
 4;194,211,212,195;,
 4;195,212,213,196;,
 4;196,213,214,197;,
 4;197,214,215,198;,
 4;198,215,216,199;,
 4;199,216,217,200;,
 4;200,217,218,201;,
 4;201,218,219,202;,
 4;203,220,221,204;,
 4;204,221,222,205;,
 4;205,222,223,206;,
 4;206,223,224,207;,
 4;207,224,225,208;,
 4;208,225,226,209;,
 4;209,226,227,210;,
 4;210,227,228,211;,
 4;211,228,229,212;,
 4;212,229,230,213;,
 4;213,230,231,214;,
 4;214,231,232,215;,
 4;215,232,233,216;,
 4;216,233,234,217;,
 4;217,234,235,218;,
 4;218,235,236,219;,
 4;220,237,238,221;,
 4;221,238,239,222;,
 4;222,239,240,223;,
 4;223,240,241,224;,
 4;224,241,242,225;,
 4;225,242,243,226;,
 4;226,243,244,227;,
 4;227,244,245,228;,
 4;228,245,246,229;,
 4;229,246,247,230;,
 4;230,247,248,231;,
 4;231,248,249,232;,
 4;232,249,250,233;,
 4;233,250,251,234;,
 4;234,251,252,235;,
 4;235,252,253,236;,
 4;237,254,255,238;,
 4;238,255,256,239;,
 4;239,256,257,240;,
 4;240,257,258,241;,
 4;241,258,259,242;,
 4;242,259,260,243;,
 4;243,260,261,244;,
 4;244,261,262,245;,
 4;245,262,263,246;,
 4;246,263,264,247;,
 4;247,264,265,248;,
 4;248,265,266,249;,
 4;249,266,267,250;,
 4;250,267,268,251;,
 4;251,268,269,252;,
 4;252,269,270,253;,
 3;254,271,255;,
 3;255,272,256;,
 3;256,273,257;,
 3;257,274,258;,
 3;258,275,259;,
 3;259,276,260;,
 3;260,277,261;,
 3;261,278,262;,
 3;262,279,263;,
 3;263,280,264;,
 3;264,281,265;,
 3;265,282,266;,
 3;266,283,267;,
 3;267,284,268;,
 3;268,285,269;,
 3;269,286,270;;
 
 MeshMaterialList {
  1;
  256;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\BG\\sky00.jpg";
   }
  }
 }
 MeshNormals {
  242;
  -0.000000;-1.000000;0.000000;,
  -0.000000;-0.980799;0.195019;,
  -0.074631;-0.980799;0.180174;,
  -0.137899;-0.980799;0.137899;,
  -0.180174;-0.980799;0.074631;,
  -0.195019;-0.980799;0.000000;,
  -0.180174;-0.980799;-0.074631;,
  -0.137899;-0.980799;-0.137899;,
  -0.074631;-0.980799;-0.180174;,
  -0.000000;-0.980799;-0.195019;,
  0.074631;-0.980799;-0.180174;,
  0.137899;-0.980799;-0.137899;,
  0.180174;-0.980799;-0.074631;,
  0.195019;-0.980799;0.000000;,
  0.180174;-0.980799;0.074631;,
  0.137899;-0.980799;0.137900;,
  0.074631;-0.980799;0.180174;,
  -0.000000;-0.923931;0.382560;,
  -0.146399;-0.923931;0.353439;,
  -0.270511;-0.923931;0.270511;,
  -0.353439;-0.923931;0.146399;,
  -0.382560;-0.923931;0.000000;,
  -0.353439;-0.923931;-0.146399;,
  -0.270511;-0.923931;-0.270511;,
  -0.146399;-0.923931;-0.353439;,
  -0.000000;-0.923931;-0.382560;,
  0.146399;-0.923931;-0.353439;,
  0.270510;-0.923931;-0.270511;,
  0.353439;-0.923931;-0.146399;,
  0.382560;-0.923931;0.000000;,
  0.353439;-0.923931;0.146399;,
  0.270511;-0.923931;0.270511;,
  0.146399;-0.923931;0.353439;,
  -0.000000;-0.831566;0.555426;,
  -0.212552;-0.831566;0.513147;,
  -0.392746;-0.831566;0.392745;,
  -0.513147;-0.831566;0.212552;,
  -0.555426;-0.831566;0.000000;,
  -0.513147;-0.831566;-0.212552;,
  -0.392745;-0.831566;-0.392746;,
  -0.212552;-0.831566;-0.513147;,
  -0.000000;-0.831566;-0.555426;,
  0.212552;-0.831566;-0.513147;,
  0.392745;-0.831566;-0.392746;,
  0.513147;-0.831566;-0.212552;,
  0.555426;-0.831566;0.000000;,
  0.513147;-0.831566;0.212552;,
  0.392745;-0.831566;0.392746;,
  0.212552;-0.831566;0.513147;,
  -0.000000;-0.707239;0.706975;,
  -0.270548;-0.707239;0.653160;,
  -0.499907;-0.707239;0.499907;,
  -0.653160;-0.707239;0.270548;,
  -0.706975;-0.707239;0.000000;,
  -0.653160;-0.707239;-0.270548;,
  -0.499907;-0.707238;-0.499907;,
  -0.270547;-0.707238;-0.653160;,
  -0.000000;-0.707238;-0.706975;,
  0.270547;-0.707238;-0.653160;,
  0.499907;-0.707238;-0.499907;,
  0.653160;-0.707238;-0.270548;,
  0.706975;-0.707239;0.000000;,
  0.653160;-0.707239;0.270548;,
  0.499907;-0.707239;0.499907;,
  0.270547;-0.707239;0.653160;,
  -0.000000;-0.555712;0.831375;,
  -0.318153;-0.555712;0.768090;,
  -0.587871;-0.555713;0.587870;,
  -0.768090;-0.555712;0.318153;,
  -0.831375;-0.555712;0.000000;,
  -0.768090;-0.555712;-0.318153;,
  -0.587870;-0.555712;-0.587871;,
  -0.318153;-0.555712;-0.768090;,
  -0.000000;-0.555712;-0.831375;,
  0.318153;-0.555712;-0.768090;,
  0.587870;-0.555712;-0.587871;,
  0.768090;-0.555712;-0.318154;,
  0.831375;-0.555712;0.000000;,
  0.768090;-0.555712;0.318153;,
  0.587870;-0.555712;0.587871;,
  0.318153;-0.555712;0.768090;,
  -0.000000;-0.382804;0.923830;,
  -0.353534;-0.382803;0.853507;,
  -0.653246;-0.382803;0.653246;,
  -0.853508;-0.382803;0.353534;,
  -0.923830;-0.382803;-0.000000;,
  -0.853507;-0.382803;-0.353535;,
  -0.653246;-0.382804;-0.653246;,
  -0.353534;-0.382804;-0.853507;,
  -0.000000;-0.382804;-0.923830;,
  0.353534;-0.382803;-0.853508;,
  0.653246;-0.382803;-0.653247;,
  0.853507;-0.382803;-0.353535;,
  0.923830;-0.382803;0.000000;,
  0.853507;-0.382803;0.353534;,
  0.653246;-0.382804;0.653246;,
  0.353534;-0.382804;0.853507;,
  -0.000000;-0.195159;0.980772;,
  -0.375325;-0.195159;0.906115;,
  -0.693510;-0.195159;0.693510;,
  -0.906115;-0.195159;0.375325;,
  -0.980772;-0.195159;-0.000000;,
  -0.906115;-0.195159;-0.375325;,
  -0.693510;-0.195159;-0.693510;,
  -0.375325;-0.195159;-0.906115;,
  -0.000000;-0.195159;-0.980772;,
  0.375325;-0.195159;-0.906115;,
  0.693510;-0.195159;-0.693511;,
  0.906115;-0.195159;-0.375325;,
  0.980772;-0.195159;0.000000;,
  0.906115;-0.195159;0.375325;,
  0.693510;-0.195159;0.693510;,
  0.375325;-0.195159;0.906115;,
  -0.000000;-0.000000;1.000000;,
  -0.382684;-0.000000;0.923880;,
  -0.707107;-0.000000;0.707107;,
  -0.923880;-0.000000;0.382683;,
  -1.000000;-0.000000;-0.000000;,
  -0.923879;-0.000000;-0.382684;,
  -0.707107;-0.000000;-0.707107;,
  -0.382684;-0.000000;-0.923880;,
  -0.000000;-0.000000;-1.000000;,
  0.382683;-0.000000;-0.923880;,
  0.707106;-0.000000;-0.707107;,
  0.923879;-0.000000;-0.382684;,
  1.000000;-0.000000;0.000000;,
  0.923880;-0.000000;0.382684;,
  0.707107;-0.000000;0.707107;,
  0.382683;-0.000000;0.923880;,
  -0.000000;0.195159;0.980772;,
  -0.375325;0.195159;0.906115;,
  -0.693510;0.195159;0.693510;,
  -0.906115;0.195159;0.375325;,
  -0.980772;0.195159;0.000000;,
  -0.906115;0.195159;-0.375325;,
  -0.693510;0.195159;-0.693511;,
  -0.375325;0.195159;-0.906115;,
  -0.000000;0.195159;-0.980772;,
  0.375325;0.195159;-0.906115;,
  0.693510;0.195159;-0.693511;,
  0.906115;0.195159;-0.375325;,
  0.980772;0.195159;0.000000;,
  0.906115;0.195159;0.375325;,
  0.693510;0.195159;0.693510;,
  0.375325;0.195159;0.906115;,
  -0.000000;0.382804;0.923830;,
  -0.353534;0.382804;0.853507;,
  -0.653246;0.382804;0.653246;,
  -0.853508;0.382803;0.353534;,
  -0.923830;0.382803;0.000000;,
  -0.853507;0.382804;-0.353535;,
  -0.653246;0.382804;-0.653247;,
  -0.353534;0.382804;-0.853507;,
  -0.000000;0.382803;-0.923830;,
  0.353534;0.382803;-0.853508;,
  0.653246;0.382803;-0.653247;,
  0.853507;0.382803;-0.353535;,
  0.923830;0.382803;0.000000;,
  0.853508;0.382803;0.353534;,
  0.653246;0.382803;0.653246;,
  0.353534;0.382803;0.853508;,
  -0.000000;0.555712;0.831375;,
  -0.318153;0.555712;0.768090;,
  -0.587871;0.555712;0.587871;,
  -0.768090;0.555712;0.318153;,
  -0.831375;0.555712;0.000000;,
  -0.768090;0.555712;-0.318153;,
  -0.587870;0.555712;-0.587871;,
  -0.318153;0.555712;-0.768090;,
  -0.000000;0.555712;-0.831375;,
  0.318153;0.555712;-0.768090;,
  0.587870;0.555712;-0.587871;,
  0.768090;0.555712;-0.318154;,
  0.831375;0.555712;0.000000;,
  0.768090;0.555712;0.318153;,
  0.587870;0.555712;0.587871;,
  0.318153;0.555712;0.768090;,
  -0.000000;0.707239;0.706975;,
  -0.270548;0.707239;0.653160;,
  -0.499907;0.707239;0.499907;,
  -0.653160;0.707239;0.270548;,
  -0.706975;0.707239;0.000000;,
  -0.653160;0.707239;-0.270548;,
  -0.499907;0.707238;-0.499907;,
  -0.270547;0.707238;-0.653160;,
  -0.000000;0.707238;-0.706975;,
  0.270547;0.707239;-0.653160;,
  0.499906;0.707239;-0.499907;,
  0.653160;0.707239;-0.270548;,
  0.706975;0.707239;0.000000;,
  0.653160;0.707239;0.270548;,
  0.499907;0.707239;0.499907;,
  0.270547;0.707239;0.653160;,
  -0.000000;0.831566;0.555426;,
  -0.212552;0.831566;0.513147;,
  -0.392746;0.831566;0.392745;,
  -0.513147;0.831566;0.212552;,
  -0.555426;0.831566;0.000000;,
  -0.513147;0.831566;-0.212552;,
  -0.392745;0.831566;-0.392746;,
  -0.212552;0.831566;-0.513147;,
  0.000000;0.831566;-0.555426;,
  0.212552;0.831566;-0.513147;,
  0.392745;0.831566;-0.392746;,
  0.513147;0.831566;-0.212552;,
  0.555426;0.831566;0.000000;,
  0.513147;0.831566;0.212552;,
  0.392745;0.831566;0.392746;,
  0.212552;0.831566;0.513147;,
  -0.000000;0.923931;0.382560;,
  -0.146399;0.923931;0.353439;,
  -0.270511;0.923931;0.270511;,
  -0.353439;0.923931;0.146399;,
  -0.382560;0.923931;0.000000;,
  -0.353439;0.923931;-0.146399;,
  -0.270511;0.923931;-0.270511;,
  -0.146399;0.923931;-0.353439;,
  0.000000;0.923931;-0.382560;,
  0.146399;0.923931;-0.353439;,
  0.270511;0.923931;-0.270511;,
  0.353439;0.923931;-0.146399;,
  0.382560;0.923931;0.000000;,
  0.353439;0.923931;0.146399;,
  0.270511;0.923931;0.270511;,
  0.146399;0.923931;0.353439;,
  -0.000000;0.980799;0.195019;,
  -0.074630;0.980799;0.180174;,
  -0.137899;0.980799;0.137899;,
  -0.180174;0.980799;0.074631;,
  -0.195019;0.980799;0.000000;,
  -0.180174;0.980799;-0.074630;,
  -0.137899;0.980799;-0.137899;,
  -0.074630;0.980799;-0.180174;,
  0.000000;0.980799;-0.195019;,
  0.074630;0.980799;-0.180174;,
  0.137899;0.980799;-0.137899;,
  0.180174;0.980799;-0.074630;,
  0.195019;0.980799;0.000000;,
  0.180174;0.980799;0.074631;,
  0.137899;0.980799;0.137899;,
  0.074630;0.980799;0.180174;,
  0.000000;1.000000;0.000000;;
  256;
  3;0,1,2;,
  3;0,2,3;,
  3;0,3,4;,
  3;0,4,5;,
  3;0,5,6;,
  3;0,6,7;,
  3;0,7,8;,
  3;0,8,9;,
  3;0,9,10;,
  3;0,10,11;,
  3;0,11,12;,
  3;0,12,13;,
  3;0,13,14;,
  3;0,14,15;,
  3;0,15,16;,
  3;0,16,1;,
  4;1,17,18,2;,
  4;2,18,19,3;,
  4;3,19,20,4;,
  4;4,20,21,5;,
  4;5,21,22,6;,
  4;6,22,23,7;,
  4;7,23,24,8;,
  4;8,24,25,9;,
  4;9,25,26,10;,
  4;10,26,27,11;,
  4;11,27,28,12;,
  4;12,28,29,13;,
  4;13,29,30,14;,
  4;14,30,31,15;,
  4;15,31,32,16;,
  4;16,32,17,1;,
  4;17,33,34,18;,
  4;18,34,35,19;,
  4;19,35,36,20;,
  4;20,36,37,21;,
  4;21,37,38,22;,
  4;22,38,39,23;,
  4;23,39,40,24;,
  4;24,40,41,25;,
  4;25,41,42,26;,
  4;26,42,43,27;,
  4;27,43,44,28;,
  4;28,44,45,29;,
  4;29,45,46,30;,
  4;30,46,47,31;,
  4;31,47,48,32;,
  4;32,48,33,17;,
  4;33,49,50,34;,
  4;34,50,51,35;,
  4;35,51,52,36;,
  4;36,52,53,37;,
  4;37,53,54,38;,
  4;38,54,55,39;,
  4;39,55,56,40;,
  4;40,56,57,41;,
  4;41,57,58,42;,
  4;42,58,59,43;,
  4;43,59,60,44;,
  4;44,60,61,45;,
  4;45,61,62,46;,
  4;46,62,63,47;,
  4;47,63,64,48;,
  4;48,64,49,33;,
  4;49,65,66,50;,
  4;50,66,67,51;,
  4;51,67,68,52;,
  4;52,68,69,53;,
  4;53,69,70,54;,
  4;54,70,71,55;,
  4;55,71,72,56;,
  4;56,72,73,57;,
  4;57,73,74,58;,
  4;58,74,75,59;,
  4;59,75,76,60;,
  4;60,76,77,61;,
  4;61,77,78,62;,
  4;62,78,79,63;,
  4;63,79,80,64;,
  4;64,80,65,49;,
  4;65,81,82,66;,
  4;66,82,83,67;,
  4;67,83,84,68;,
  4;68,84,85,69;,
  4;69,85,86,70;,
  4;70,86,87,71;,
  4;71,87,88,72;,
  4;72,88,89,73;,
  4;73,89,90,74;,
  4;74,90,91,75;,
  4;75,91,92,76;,
  4;76,92,93,77;,
  4;77,93,94,78;,
  4;78,94,95,79;,
  4;79,95,96,80;,
  4;80,96,81,65;,
  4;81,97,98,82;,
  4;82,98,99,83;,
  4;83,99,100,84;,
  4;84,100,101,85;,
  4;85,101,102,86;,
  4;86,102,103,87;,
  4;87,103,104,88;,
  4;88,104,105,89;,
  4;89,105,106,90;,
  4;90,106,107,91;,
  4;91,107,108,92;,
  4;92,108,109,93;,
  4;93,109,110,94;,
  4;94,110,111,95;,
  4;95,111,112,96;,
  4;96,112,97,81;,
  4;97,113,114,98;,
  4;98,114,115,99;,
  4;99,115,116,100;,
  4;100,116,117,101;,
  4;101,117,118,102;,
  4;102,118,119,103;,
  4;103,119,120,104;,
  4;104,120,121,105;,
  4;105,121,122,106;,
  4;106,122,123,107;,
  4;107,123,124,108;,
  4;108,124,125,109;,
  4;109,125,126,110;,
  4;110,126,127,111;,
  4;111,127,128,112;,
  4;112,128,113,97;,
  4;113,129,130,114;,
  4;114,130,131,115;,
  4;115,131,132,116;,
  4;116,132,133,117;,
  4;117,133,134,118;,
  4;118,134,135,119;,
  4;119,135,136,120;,
  4;120,136,137,121;,
  4;121,137,138,122;,
  4;122,138,139,123;,
  4;123,139,140,124;,
  4;124,140,141,125;,
  4;125,141,142,126;,
  4;126,142,143,127;,
  4;127,143,144,128;,
  4;128,144,129,113;,
  4;129,145,146,130;,
  4;130,146,147,131;,
  4;131,147,148,132;,
  4;132,148,149,133;,
  4;133,149,150,134;,
  4;134,150,151,135;,
  4;135,151,152,136;,
  4;136,152,153,137;,
  4;137,153,154,138;,
  4;138,154,155,139;,
  4;139,155,156,140;,
  4;140,156,157,141;,
  4;141,157,158,142;,
  4;142,158,159,143;,
  4;143,159,160,144;,
  4;144,160,145,129;,
  4;145,161,162,146;,
  4;146,162,163,147;,
  4;147,163,164,148;,
  4;148,164,165,149;,
  4;149,165,166,150;,
  4;150,166,167,151;,
  4;151,167,168,152;,
  4;152,168,169,153;,
  4;153,169,170,154;,
  4;154,170,171,155;,
  4;155,171,172,156;,
  4;156,172,173,157;,
  4;157,173,174,158;,
  4;158,174,175,159;,
  4;159,175,176,160;,
  4;160,176,161,145;,
  4;161,177,178,162;,
  4;162,178,179,163;,
  4;163,179,180,164;,
  4;164,180,181,165;,
  4;165,181,182,166;,
  4;166,182,183,167;,
  4;167,183,184,168;,
  4;168,184,185,169;,
  4;169,185,186,170;,
  4;170,186,187,171;,
  4;171,187,188,172;,
  4;172,188,189,173;,
  4;173,189,190,174;,
  4;174,190,191,175;,
  4;175,191,192,176;,
  4;176,192,177,161;,
  4;177,193,194,178;,
  4;178,194,195,179;,
  4;179,195,196,180;,
  4;180,196,197,181;,
  4;181,197,198,182;,
  4;182,198,199,183;,
  4;183,199,200,184;,
  4;184,200,201,185;,
  4;185,201,202,186;,
  4;186,202,203,187;,
  4;187,203,204,188;,
  4;188,204,205,189;,
  4;189,205,206,190;,
  4;190,206,207,191;,
  4;191,207,208,192;,
  4;192,208,193,177;,
  4;193,209,210,194;,
  4;194,210,211,195;,
  4;195,211,212,196;,
  4;196,212,213,197;,
  4;197,213,214,198;,
  4;198,214,215,199;,
  4;199,215,216,200;,
  4;200,216,217,201;,
  4;201,217,218,202;,
  4;202,218,219,203;,
  4;203,219,220,204;,
  4;204,220,221,205;,
  4;205,221,222,206;,
  4;206,222,223,207;,
  4;207,223,224,208;,
  4;208,224,209,193;,
  4;209,225,226,210;,
  4;210,226,227,211;,
  4;211,227,228,212;,
  4;212,228,229,213;,
  4;213,229,230,214;,
  4;214,230,231,215;,
  4;215,231,232,216;,
  4;216,232,233,217;,
  4;217,233,234,218;,
  4;218,234,235,219;,
  4;219,235,236,220;,
  4;220,236,237,221;,
  4;221,237,238,222;,
  4;222,238,239,223;,
  4;223,239,240,224;,
  4;224,240,225,209;,
  3;225,241,226;,
  3;226,241,227;,
  3;227,241,228;,
  3;228,241,229;,
  3;229,241,230;,
  3;230,241,231;,
  3;231,241,232;,
  3;232,241,233;,
  3;233,241,234;,
  3;234,241,235;,
  3;235,241,236;,
  3;236,241,237;,
  3;237,241,238;,
  3;238,241,239;,
  3;239,241,240;,
  3;240,241,225;;
 }
 MeshTextureCoords {
  287;
  0.031250;0.000000;,
  0.000000;0.062500;,
  0.062500;0.062500;,
  0.093750;0.000000;,
  0.125000;0.062500;,
  0.156250;0.000000;,
  0.187500;0.062500;,
  0.218750;0.000000;,
  0.250000;0.062500;,
  0.281250;0.000000;,
  0.312500;0.062500;,
  0.343750;0.000000;,
  0.375000;0.062500;,
  0.406250;0.000000;,
  0.437500;0.062500;,
  0.468750;0.000000;,
  0.500000;0.062500;,
  0.531250;0.000000;,
  0.562500;0.062500;,
  0.593750;0.000000;,
  0.625000;0.062500;,
  0.656250;0.000000;,
  0.687500;0.062500;,
  0.718750;0.000000;,
  0.750000;0.062500;,
  0.781250;0.000000;,
  0.812500;0.062500;,
  0.843750;0.000000;,
  0.875000;0.062500;,
  0.906250;0.000000;,
  0.937500;0.062500;,
  0.968750;0.000000;,
  1.000000;0.062500;,
  0.000000;0.125000;,
  0.062500;0.125000;,
  0.125000;0.125000;,
  0.187500;0.125000;,
  0.250000;0.125000;,
  0.312500;0.125000;,
  0.375000;0.125000;,
  0.437500;0.125000;,
  0.500000;0.125000;,
  0.562500;0.125000;,
  0.625000;0.125000;,
  0.687500;0.125000;,
  0.750000;0.125000;,
  0.812500;0.125000;,
  0.875000;0.125000;,
  0.937500;0.125000;,
  1.000000;0.125000;,
  0.000000;0.187500;,
  0.062500;0.187500;,
  0.125000;0.187500;,
  0.187500;0.187500;,
  0.250000;0.187500;,
  0.312500;0.187500;,
  0.375000;0.187500;,
  0.437500;0.187500;,
  0.500000;0.187500;,
  0.562500;0.187500;,
  0.625000;0.187500;,
  0.687500;0.187500;,
  0.750000;0.187500;,
  0.812500;0.187500;,
  0.875000;0.187500;,
  0.937500;0.187500;,
  1.000000;0.187500;,
  0.000000;0.250000;,
  0.062500;0.250000;,
  0.125000;0.250000;,
  0.187500;0.250000;,
  0.250000;0.250000;,
  0.312500;0.250000;,
  0.375000;0.250000;,
  0.437500;0.250000;,
  0.500000;0.250000;,
  0.562500;0.250000;,
  0.625000;0.250000;,
  0.687500;0.250000;,
  0.750000;0.250000;,
  0.812500;0.250000;,
  0.875000;0.250000;,
  0.937500;0.250000;,
  1.000000;0.250000;,
  0.000000;0.312500;,
  0.062500;0.312500;,
  0.125000;0.312500;,
  0.187500;0.312500;,
  0.250000;0.312500;,
  0.312500;0.312500;,
  0.375000;0.312500;,
  0.437500;0.312500;,
  0.500000;0.312500;,
  0.562500;0.312500;,
  0.625000;0.312500;,
  0.687500;0.312500;,
  0.750000;0.312500;,
  0.812500;0.312500;,
  0.875000;0.312500;,
  0.937500;0.312500;,
  1.000000;0.312500;,
  0.000000;0.375000;,
  0.062500;0.375000;,
  0.125000;0.375000;,
  0.187500;0.375000;,
  0.250000;0.375000;,
  0.312500;0.375000;,
  0.375000;0.375000;,
  0.437500;0.375000;,
  0.500000;0.375000;,
  0.562500;0.375000;,
  0.625000;0.375000;,
  0.687500;0.375000;,
  0.750000;0.375000;,
  0.812500;0.375000;,
  0.875000;0.375000;,
  0.937500;0.375000;,
  1.000000;0.375000;,
  0.000000;0.437500;,
  0.062500;0.437500;,
  0.125000;0.437500;,
  0.187500;0.437500;,
  0.250000;0.437500;,
  0.312500;0.437500;,
  0.375000;0.437500;,
  0.437500;0.437500;,
  0.500000;0.437500;,
  0.562500;0.437500;,
  0.625000;0.437500;,
  0.687500;0.437500;,
  0.750000;0.437500;,
  0.812500;0.437500;,
  0.875000;0.437500;,
  0.937500;0.437500;,
  1.000000;0.437500;,
  0.000000;0.500000;,
  0.062500;0.500000;,
  0.125000;0.500000;,
  0.187500;0.500000;,
  0.250000;0.500000;,
  0.312500;0.500000;,
  0.375000;0.500000;,
  0.437500;0.500000;,
  0.500000;0.500000;,
  0.562500;0.500000;,
  0.625000;0.500000;,
  0.687500;0.500000;,
  0.750000;0.500000;,
  0.812500;0.500000;,
  0.875000;0.500000;,
  0.937500;0.500000;,
  1.000000;0.500000;,
  0.000000;0.562500;,
  0.062500;0.562500;,
  0.125000;0.562500;,
  0.187500;0.562500;,
  0.250000;0.562500;,
  0.312500;0.562500;,
  0.375000;0.562500;,
  0.437500;0.562500;,
  0.500000;0.562500;,
  0.562500;0.562500;,
  0.625000;0.562500;,
  0.687500;0.562500;,
  0.750000;0.562500;,
  0.812500;0.562500;,
  0.875000;0.562500;,
  0.937500;0.562500;,
  1.000000;0.562500;,
  0.000000;0.625000;,
  0.062500;0.625000;,
  0.125000;0.625000;,
  0.187500;0.625000;,
  0.250000;0.625000;,
  0.312500;0.625000;,
  0.375000;0.625000;,
  0.437500;0.625000;,
  0.500000;0.625000;,
  0.562500;0.625000;,
  0.625000;0.625000;,
  0.687500;0.625000;,
  0.750000;0.625000;,
  0.812500;0.625000;,
  0.875000;0.625000;,
  0.937500;0.625000;,
  1.000000;0.625000;,
  0.000000;0.687500;,
  0.062500;0.687500;,
  0.125000;0.687500;,
  0.187500;0.687500;,
  0.250000;0.687500;,
  0.312500;0.687500;,
  0.375000;0.687500;,
  0.437500;0.687500;,
  0.500000;0.687500;,
  0.562500;0.687500;,
  0.625000;0.687500;,
  0.687500;0.687500;,
  0.750000;0.687500;,
  0.812500;0.687500;,
  0.875000;0.687500;,
  0.937500;0.687500;,
  1.000000;0.687500;,
  0.000000;0.750000;,
  0.062500;0.750000;,
  0.125000;0.750000;,
  0.187500;0.750000;,
  0.250000;0.750000;,
  0.312500;0.750000;,
  0.375000;0.750000;,
  0.437500;0.750000;,
  0.500000;0.750000;,
  0.562500;0.750000;,
  0.625000;0.750000;,
  0.687500;0.750000;,
  0.750000;0.750000;,
  0.812500;0.750000;,
  0.875000;0.750000;,
  0.937500;0.750000;,
  1.000000;0.750000;,
  0.000000;0.812500;,
  0.062500;0.812500;,
  0.125000;0.812500;,
  0.187500;0.812500;,
  0.250000;0.812500;,
  0.312500;0.812500;,
  0.375000;0.812500;,
  0.437500;0.812500;,
  0.500000;0.812500;,
  0.562500;0.812500;,
  0.625000;0.812500;,
  0.687500;0.812500;,
  0.750000;0.812500;,
  0.812500;0.812500;,
  0.875000;0.812500;,
  0.937500;0.812500;,
  1.000000;0.812500;,
  0.000000;0.875000;,
  0.062500;0.875000;,
  0.125000;0.875000;,
  0.187500;0.875000;,
  0.250000;0.875000;,
  0.312500;0.875000;,
  0.375000;0.875000;,
  0.437500;0.875000;,
  0.500000;0.875000;,
  0.562500;0.875000;,
  0.625000;0.875000;,
  0.687500;0.875000;,
  0.750000;0.875000;,
  0.812500;0.875000;,
  0.875000;0.875000;,
  0.937500;0.875000;,
  1.000000;0.875000;,
  0.000000;0.937500;,
  0.062500;0.937500;,
  0.125000;0.937500;,
  0.187500;0.937500;,
  0.250000;0.937500;,
  0.312500;0.937500;,
  0.375000;0.937500;,
  0.437500;0.937500;,
  0.500000;0.937500;,
  0.562500;0.937500;,
  0.625000;0.937500;,
  0.687500;0.937500;,
  0.750000;0.937500;,
  0.812500;0.937500;,
  0.875000;0.937500;,
  0.937500;0.937500;,
  1.000000;0.937500;,
  0.031250;1.000000;,
  0.093750;1.000000;,
  0.156250;1.000000;,
  0.218750;1.000000;,
  0.281250;1.000000;,
  0.343750;1.000000;,
  0.406250;1.000000;,
  0.468750;1.000000;,
  0.531250;1.000000;,
  0.593750;1.000000;,
  0.656250;1.000000;,
  0.718750;1.000000;,
  0.781250;1.000000;,
  0.843750;1.000000;,
  0.906250;1.000000;,
  0.968750;1.000000;;
 }
}
