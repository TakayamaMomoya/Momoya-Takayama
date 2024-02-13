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
 192;
 -0.35065;-0.29060;-0.70832;,
 -0.41210;-2.79278;-0.49779;,
 -0.46905;-2.79278;-0.42148;,
 -0.53891;-0.29060;-0.50604;,
 -0.41210;-2.79278;0.60759;,
 -0.28900;-0.29060;0.80151;,
 -0.59687;-0.29060;0.61144;,
 -0.52702;-2.79278;0.52049;,
 -0.67407;-2.79278;0.05045;,
 -0.41210;-2.79278;0.05045;,
 -0.41210;-2.79278;0.48748;,
 -0.55872;-2.79278;0.41916;,
 -0.77139;-0.29060;0.05365;,
 -0.67407;-2.79278;0.05045;,
 -0.55872;-2.79278;0.41916;,
 -0.63449;-0.29060;0.49119;,
 0.36043;-0.29060;0.49119;,
 0.27968;-2.79278;0.41916;,
 0.36838;-2.79278;0.05045;,
 0.46570;-0.29060;0.05365;,
 -0.02186;-2.79278;0.05045;,
 -0.06621;-2.79278;0.54484;,
 -0.07840;-2.79278;0.68070;,
 -0.06450;-0.29060;0.80157;,
 -0.28900;-0.29060;0.80151;,
 -0.41210;-2.79278;0.60759;,
 -0.28900;-0.29060;0.65898;,
 -0.05004;-0.29060;0.64033;,
 0.00260;-0.29060;0.05365;,
 -0.46050;-0.29060;0.05365;,
 -0.06053;-0.29060;-0.80157;,
 -0.07443;-2.79278;-0.68070;,
 -0.41210;-2.79278;-0.49779;,
 -0.35065;-0.29060;-0.70832;,
 -0.56061;0.47215;0.54897;,
 -0.28900;-0.29060;0.65898;,
 -0.46050;-0.29060;0.05365;,
 -0.64135;0.47215;0.05365;,
 -0.35623;0.47215;-0.61551;,
 -0.64135;0.47215;0.05365;,
 -0.56061;0.47215;0.54897;,
 -0.29610;0.47215;0.69815;,
 -0.05151;-0.29060;-0.58231;,
 -0.35065;-0.29060;-0.58240;,
 -0.35065;-0.29060;-0.58240;,
 -0.51326;0.47215;-0.41490;,
 -0.51326;0.47215;-0.41490;,
 -0.63065;-0.29060;-0.39620;,
 -0.55548;-2.79278;-0.32862;,
 -0.55548;-2.79278;-0.32862;,
 -0.41210;-2.79278;-0.39169;,
 -0.06746;-2.79278;-0.48545;,
 0.27719;-2.79278;-0.32862;,
 0.35747;-0.29060;-0.39620;,
 0.36838;-2.79278;0.05045;,
 0.27719;-2.79278;-0.32862;,
 0.27968;-2.79278;0.41916;,
 0.33150;-0.29060;0.61144;,
 0.25530;-2.79278;0.52049;,
 0.46570;-0.29060;0.05365;,
 0.36043;-0.29060;0.49119;,
 0.35747;-0.29060;-0.39620;,
 0.21124;-2.79278;-0.42276;,
 -0.07443;-2.79278;-0.68070;,
 -0.06053;-0.29060;-0.80157;,
 0.28744;-0.29060;-0.50732;,
 0.28744;-0.29060;-0.50732;,
 -0.06053;-0.29060;-0.80157;,
 -0.35065;-0.29060;-0.70832;,
 -0.35065;-0.29060;-0.70832;,
 -0.35623;0.47215;-0.61551;,
 -0.41210;-2.79278;-0.49779;,
 -0.46905;-2.79278;-0.42148;,
 -0.07443;-2.79278;-0.68070;,
 0.21124;-2.79278;-0.42276;,
 0.25530;-2.79278;0.52049;,
 -0.07840;-2.79278;0.68070;,
 -0.41210;-2.79278;0.60759;,
 -0.52702;-2.79278;0.52049;,
 -0.28900;-0.29060;0.80151;,
 -0.29610;0.47215;0.69815;,
 -0.06450;-0.29060;0.80157;,
 -0.28900;-0.29060;0.80151;,
 0.33150;-0.29060;0.61144;,
 0.25530;-2.79278;0.52049;,
 0.33150;-0.29060;0.61144;,
 -0.46863;0.32607;-0.37680;,
 -0.27335;0.52136;-0.37680;,
 -0.24691;0.45548;-0.48404;,
 -0.39265;0.29337;-0.48404;,
 -0.24691;0.45548;-0.48404;,
 -0.00658;0.51482;-0.48404;,
 -0.00658;0.00282;-0.48404;,
 0.23376;0.45548;-0.48404;,
 0.37950;0.29337;-0.48404;,
 0.45548;0.32607;-0.37680;,
 0.52696;0.05931;-0.37680;,
 0.43284;0.07191;-0.48404;,
 0.37950;0.29337;-0.48404;,
 0.45548;-0.20746;-0.37680;,
 0.37950;-0.14955;-0.48404;,
 0.30131;-0.39067;-0.37680;,
 0.22907;-0.33738;-0.48404;,
 0.30131;-0.39067;-0.37680;,
 -0.00658;-0.47423;-0.37680;,
 -0.00658;-0.37100;-0.48404;,
 0.22907;-0.33738;-0.48404;,
 -0.31447;-0.39067;-0.37680;,
 -0.24223;-0.33738;-0.48404;,
 -0.31447;-0.39067;-0.37680;,
 -0.46863;-0.20746;-0.37680;,
 -0.39265;-0.14955;-0.48404;,
 -0.24223;-0.33738;-0.48404;,
 -0.54011;0.05931;-0.37680;,
 -0.44600;0.07191;-0.48404;,
 -0.27335;0.52136;-0.37680;,
 -0.00658;0.59284;-0.37680;,
 -0.00658;0.51482;-0.48404;,
 -0.24691;0.45548;-0.48404;,
 0.26019;0.52136;-0.37680;,
 0.23376;0.45548;-0.48404;,
 -0.44600;0.07191;-0.48404;,
 -0.39265;0.29337;-0.48404;,
 -0.39265;-0.14955;-0.48404;,
 -0.24223;-0.33738;-0.48404;,
 -0.00658;-0.37100;-0.48404;,
 0.22907;-0.33738;-0.48404;,
 0.37950;-0.14955;-0.48404;,
 0.43284;0.07191;-0.48404;,
 0.26019;0.52136;-0.37680;,
 0.23376;0.45548;-0.48404;,
 -0.46863;0.32607;0.02471;,
 -0.27335;0.52136;0.02471;,
 -0.27335;0.52136;0.02471;,
 -0.00658;0.59284;0.02471;,
 0.26019;0.52136;0.02471;,
 0.26019;0.52136;0.02471;,
 0.45548;0.32607;0.02471;,
 0.52696;0.05931;0.02471;,
 0.45548;-0.20746;0.02471;,
 0.30131;-0.39067;0.02471;,
 0.30131;-0.39067;0.02471;,
 -0.00658;-0.47423;0.02471;,
 -0.31447;-0.39067;0.02471;,
 -0.31447;-0.39067;0.02471;,
 -0.46863;-0.20746;0.02471;,
 -0.54011;0.05931;0.02471;,
 -0.46863;0.32607;0.42621;,
 -0.39265;0.29337;0.53346;,
 -0.24691;0.45548;0.53346;,
 -0.27335;0.52136;0.42621;,
 -0.24691;0.45548;0.53346;,
 -0.00658;0.00282;0.53346;,
 -0.00658;0.51482;0.53346;,
 0.23376;0.45548;0.53346;,
 0.37950;0.29337;0.53346;,
 0.45548;0.32607;0.42621;,
 0.37950;0.29337;0.53346;,
 0.43284;0.07191;0.53346;,
 0.52696;0.05931;0.42621;,
 0.37950;-0.14955;0.53346;,
 0.45548;-0.20746;0.42621;,
 0.24576;-0.30784;0.53346;,
 0.30131;-0.39067;0.42621;,
 0.30131;-0.39067;0.42621;,
 0.24576;-0.30784;0.53346;,
 -0.00658;-0.37100;0.53346;,
 -0.00658;-0.47423;0.42621;,
 -0.25892;-0.30784;0.53346;,
 -0.31447;-0.39067;0.42621;,
 -0.31447;-0.39067;0.42621;,
 -0.25892;-0.30784;0.53346;,
 -0.39265;-0.14955;0.53346;,
 -0.46863;-0.20746;0.42621;,
 -0.44600;0.07191;0.53346;,
 -0.54011;0.05931;0.42621;,
 -0.27335;0.52136;0.42621;,
 -0.24691;0.45548;0.53346;,
 -0.00658;0.51482;0.53346;,
 -0.00658;0.59284;0.42621;,
 0.23376;0.45548;0.53346;,
 0.26019;0.52136;0.42621;,
 -0.44600;0.07191;0.53346;,
 -0.39265;0.29337;0.53346;,
 -0.39265;-0.14955;0.53346;,
 -0.25892;-0.30784;0.53346;,
 -0.00658;-0.37100;0.53346;,
 0.24576;-0.30784;0.53346;,
 0.37950;-0.14955;0.53346;,
 0.43284;0.07191;0.53346;,
 0.26019;0.52136;0.42621;,
 0.23376;0.45548;0.53346;;
 
 116;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,10,9;,
 4;22,23,24,25;,
 4;26,27,28,29;,
 4;30,31,32,33;,
 4;34,35,36,37;,
 3;0,3,38;,
 4;39,12,15,40;,
 3;6,5,41;,
 4;28,42,43,29;,
 4;37,36,44,45;,
 4;46,47,12,39;,
 4;47,48,13,12;,
 4;49,50,9,8;,
 4;50,51,20,9;,
 4;19,18,52,53;,
 4;54,20,51,55;,
 4;56,21,20,54;,
 4;57,23,22,58;,
 4;59,28,27,60;,
 4;61,42,28,59;,
 4;62,63,64,65;,
 4;66,67,42,61;,
 4;43,42,67,68;,
 4;45,44,69,70;,
 4;3,47,46,38;,
 4;2,48,47,3;,
 4;71,50,49,72;,
 4;73,51,50,71;,
 4;55,51,73,74;,
 4;53,52,62,65;,
 4;75,76,21,56;,
 4;10,21,76,77;,
 4;11,10,77,78;,
 4;15,14,7,6;,
 4;40,15,6,41;,
 4;79,35,34,80;,
 4;81,27,26,82;,
 4;60,27,81,83;,
 4;84,17,16,85;,
 4;86,87,88,89;,
 3;90,91,92;,
 3;91,93,92;,
 3;93,94,92;,
 4;95,96,97,98;,
 4;96,99,100,97;,
 4;99,101,102,100;,
 4;103,104,105,106;,
 4;104,107,108,105;,
 4;109,110,111,112;,
 4;110,113,114,111;,
 4;113,86,89,114;,
 4;115,116,117,118;,
 4;116,119,120,117;,
 3;121,122,92;,
 3;123,121,92;,
 3;124,123,92;,
 3;125,124,92;,
 3;126,125,92;,
 3;127,126,92;,
 3;128,127,92;,
 3;94,128,92;,
 4;129,95,98,130;,
 3;122,90,92;,
 4;131,132,87,86;,
 4;133,134,116,115;,
 4;134,135,119,116;,
 4;136,137,95,129;,
 4;137,138,96,95;,
 4;138,139,99,96;,
 4;139,140,101,99;,
 4;141,142,104,103;,
 4;142,143,107,104;,
 4;144,145,110,109;,
 4;145,146,113,110;,
 4;146,131,86,113;,
 4;147,148,149,150;,
 3;151,152,153;,
 3;153,152,154;,
 3;154,152,155;,
 4;156,157,158,159;,
 4;159,158,160,161;,
 4;161,160,162,163;,
 4;164,165,166,167;,
 4;167,166,168,169;,
 4;170,171,172,173;,
 4;173,172,174,175;,
 4;175,174,148,147;,
 4;176,177,178,179;,
 4;179,178,180,181;,
 3;182,152,183;,
 3;184,152,182;,
 3;185,152,184;,
 3;186,152,185;,
 3;187,152,186;,
 3;188,152,187;,
 3;189,152,188;,
 3;155,152,189;,
 4;190,191,157,156;,
 3;183,152,151;,
 4;131,147,150,132;,
 4;133,176,179,134;,
 4;134,179,181,135;,
 4;136,190,156,137;,
 4;137,156,159,138;,
 4;138,159,161,139;,
 4;139,161,163,140;,
 4;141,164,167,142;,
 4;142,167,169,143;,
 4;144,170,173,145;,
 4;145,173,175,146;,
 4;146,175,147,131;;
 
 MeshMaterialList {
  1;
  116;
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
    "data\\TEXTURE\\MATERIAL\\acTex002.png";
   }
  }
 }
 MeshNormals {
  96;
  0.751328;-0.043230;-0.658511;,
  -0.758360;-0.041359;-0.650523;,
  -0.756912;0.023898;-0.653079;,
  0.777008;-0.046482;0.627772;,
  -0.735010;0.030657;0.677363;,
  -0.803418;-0.042564;0.593892;,
  -0.596201;-0.048555;-0.801366;,
  -0.346624;-0.051529;0.936588;,
  -0.409331;0.003359;0.912380;,
  -0.645104;-0.005847;-0.764072;,
  -0.657701;0.100356;0.746565;,
  -0.752231;0.089090;-0.652849;,
  0.999245;-0.038862;-0.000003;,
  0.000000;1.000000;0.000000;,
  0.972332;0.233598;0.001797;,
  -0.998567;0.051909;-0.012986;,
  0.000000;-1.000000;-0.000000;,
  -0.392245;-0.053434;-0.918308;,
  0.168865;-0.053482;0.984187;,
  0.915810;-0.040743;-0.399539;,
  0.956392;0.109562;0.270759;,
  -0.877647;0.044310;-0.477254;,
  -0.870614;-0.040092;-0.490330;,
  0.971554;-0.038083;0.233736;,
  -0.928620;0.034474;0.369428;,
  0.919694;0.149454;-0.363080;,
  -0.823436;0.469581;-0.318508;,
  -0.479865;0.823710;-0.302046;,
  0.000000;0.953025;-0.302892;,
  0.479866;0.823709;-0.302046;,
  0.823436;0.469581;-0.318506;,
  0.938003;0.001554;-0.346622;,
  0.831529;-0.427688;-0.354459;,
  0.493388;-0.794144;-0.354828;,
  0.000000;-0.941010;-0.338379;,
  -0.493387;-0.794145;-0.354829;,
  -0.831529;-0.427688;-0.354461;,
  -0.938003;0.001554;-0.346624;,
  0.000000;0.000000;-1.000000;,
  -0.231075;0.392373;-0.890308;,
  0.000000;0.456510;-0.889718;,
  0.231076;0.392373;-0.890307;,
  -0.385393;0.216431;-0.897012;,
  -0.416887;0.001581;-0.908957;,
  -0.365524;-0.184598;-0.912314;,
  -0.208393;-0.347762;-0.914130;,
  0.000000;-0.426867;-0.904314;,
  0.208393;-0.347762;-0.914130;,
  0.365525;-0.184599;-0.912313;,
  0.416888;0.001581;-0.908957;,
  0.385395;0.216432;-0.897011;,
  -0.866025;0.500001;0.000000;,
  -0.499999;0.866026;0.000000;,
  0.000000;1.000000;0.000000;,
  0.500001;0.866025;0.000000;,
  0.866025;0.500001;0.000000;,
  1.000000;-0.000001;-0.000000;,
  0.886687;-0.462370;-0.000000;,
  0.538069;-0.842900;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.538068;-0.842901;0.000000;,
  -0.886687;-0.462371;0.000000;,
  -1.000000;-0.000001;0.000000;,
  -0.823436;0.469581;0.318508;,
  -0.479865;0.823710;0.302046;,
  0.000000;0.953025;0.302892;,
  0.479866;0.823709;0.302046;,
  0.823436;0.469581;0.318506;,
  0.938003;0.001554;0.346622;,
  0.829221;-0.428777;0.358528;,
  0.497842;-0.782929;0.373061;,
  0.000000;-0.928634;0.370996;,
  -0.497841;-0.782930;0.373062;,
  -0.829220;-0.428778;0.358529;,
  -0.938003;0.001554;0.346624;,
  0.000000;0.000000;1.000000;,
  -0.231075;0.392373;0.890308;,
  0.000000;0.456510;0.889718;,
  0.231076;0.392373;0.890307;,
  -0.385393;0.216431;0.897012;,
  -0.416887;0.001581;0.908957;,
  -0.360871;-0.184513;0.914181;,
  -0.208955;-0.330346;0.920440;,
  0.000000;-0.393988;0.919116;,
  0.208956;-0.330346;0.920440;,
  0.360873;-0.184513;0.914181;,
  0.416888;0.001581;0.908957;,
  0.385395;0.216432;0.897011;,
  -0.999245;-0.038862;-0.000004;,
  -0.953712;-0.037472;0.298379;,
  -0.989670;0.141045;-0.025689;,
  -0.892421;0.107864;0.438121;,
  -0.878241;0.128177;-0.460720;,
  0.657406;-0.042947;-0.752312;,
  0.945189;0.021061;0.325844;,
  0.863168;0.033899;-0.503778;;
  116;
  4;9,6,1,2;,
  4;7,8,4,5;,
  4;16,16,16,16;,
  4;15,88,89,24;,
  4;23,23,12,12;,
  4;16,16,16,16;,
  4;18,18,8,7;,
  4;13,13,13,13;,
  4;17,17,6,9;,
  4;25,25,14,14;,
  3;9,2,11;,
  4;90,15,24,91;,
  3;4,8,10;,
  4;13,13,13,13;,
  4;14,14,20,20;,
  4;92,21,15,90;,
  4;21,22,88,15;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;12,12,19,19;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;3,18,18,3;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;0,93,93,0;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;20,20,94,94;,
  4;2,21,92,11;,
  4;1,22,21,2;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;19,19,0,0;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;24,89,5,4;,
  4;91,24,4,10;,
  4;95,25,25,95;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;3,23,23,3;,
  4;26,27,39,42;,
  3;39,40,38;,
  3;40,41,38;,
  3;41,50,38;,
  4;30,31,49,50;,
  4;31,32,48,49;,
  4;32,33,47,48;,
  4;33,34,46,47;,
  4;34,35,45,46;,
  4;35,36,44,45;,
  4;36,37,43,44;,
  4;37,26,42,43;,
  4;27,28,40,39;,
  4;28,29,41,40;,
  3;43,42,38;,
  3;44,43,38;,
  3;45,44,38;,
  3;46,45,38;,
  3;47,46,38;,
  3;48,47,38;,
  3;49,48,38;,
  3;50,49,38;,
  4;29,30,50,41;,
  3;42,39,38;,
  4;51,52,27,26;,
  4;52,53,28,27;,
  4;53,54,29,28;,
  4;54,55,30,29;,
  4;55,56,31,30;,
  4;56,57,32,31;,
  4;57,58,33,32;,
  4;58,59,34,33;,
  4;59,60,35,34;,
  4;60,61,36,35;,
  4;61,62,37,36;,
  4;62,51,26,37;,
  4;63,79,76,64;,
  3;76,75,77;,
  3;77,75,78;,
  3;78,75,87;,
  4;67,87,86,68;,
  4;68,86,85,69;,
  4;69,85,84,70;,
  4;70,84,83,71;,
  4;71,83,82,72;,
  4;72,82,81,73;,
  4;73,81,80,74;,
  4;74,80,79,63;,
  4;64,76,77,65;,
  4;65,77,78,66;,
  3;80,75,79;,
  3;81,75,80;,
  3;82,75,81;,
  3;83,75,82;,
  3;84,75,83;,
  3;85,75,84;,
  3;86,75,85;,
  3;87,75,86;,
  4;66,78,87,67;,
  3;79,75,76;,
  4;51,63,64,52;,
  4;52,64,65,53;,
  4;53,65,66,54;,
  4;54,66,67,55;,
  4;55,67,68,56;,
  4;56,68,69,57;,
  4;57,69,70,58;,
  4;58,70,71,59;,
  4;59,71,72,60;,
  4;60,72,73,61;,
  4;61,73,74,62;,
  4;62,74,63,51;;
 }
 MeshTextureCoords {
  192;
  0.860780;0.105190;,
  0.877140;0.299620;,
  0.883070;0.299620;,
  0.876500;0.105190;,
  0.963030;0.299620;,
  0.978100;0.105190;,
  0.963330;0.105190;,
  0.956260;0.299620;,
  0.961380;0.358340;,
  0.944420;0.358340;,
  0.944420;0.386640;,
  0.953910;0.382210;,
  0.919990;0.105190;,
  0.919740;0.299620;,
  0.948390;0.299620;,
  0.953990;0.105190;,
  0.735590;0.106280;,
  0.741190;0.300710;,
  0.769840;0.300710;,
  0.769590;0.106280;,
  0.919140;0.358340;,
  0.922020;0.390350;,
  0.700210;0.106350;,
  0.699130;0.300780;,
  0.732860;0.301560;,
  0.726140;0.106350;,
  0.868950;0.317480;,
  0.856420;0.318450;,
  0.853660;0.349220;,
  0.877950;0.349220;,
  0.836440;0.106110;,
  0.837520;0.300550;,
  0.877200;0.299800;,
  0.858980;0.106110;,
  0.747500;0.050240;,
  0.740090;0.103770;,
  0.782540;0.103530;,
  0.782230;0.050050;,
  0.867990;0.045920;,
  0.919990;0.045920;,
  0.958480;0.045920;,
  0.970070;0.045920;,
  0.856500;0.382570;,
  0.872180;0.382580;,
  0.827140;0.103270;,
  0.815080;0.049860;,
  0.883580;0.045920;,
  0.885030;0.105190;,
  0.890280;0.299620;,
  0.953700;0.333790;,
  0.944420;0.329700;,
  0.922100;0.323630;,
  0.799300;0.300710;,
  0.804550;0.106280;,
  0.893870;0.358340;,
  0.899780;0.333790;,
  0.899620;0.382210;,
  0.668360;0.300780;,
  0.674280;0.106350;,
  0.829370;0.349220;,
  0.834890;0.326270;,
  0.835050;0.372810;,
  0.806610;0.300710;,
  0.837680;0.300710;,
  0.836050;0.106280;,
  0.813180;0.106280;,
  0.838720;0.378640;,
  0.856970;0.394070;,
  0.872180;0.389180;,
  0.835960;0.103220;,
  0.829150;0.049780;,
  0.944420;0.322830;,
  0.948100;0.327780;,
  0.922550;0.310990;,
  0.904050;0.327690;,
  0.901200;0.388770;,
  0.922810;0.399150;,
  0.944420;0.394410;,
  0.951860;0.388770;,
  0.730100;0.103830;,
  0.737040;0.050310;,
  0.857180;0.310000;,
  0.868950;0.310000;,
  0.836410;0.319970;,
  0.733320;0.300710;,
  0.726250;0.106280;,
  0.696160;0.049650;,
  0.705940;0.049650;,
  0.702640;0.044280;,
  0.694520;0.044280;,
  0.807340;0.316120;,
  0.788670;0.311510;,
  0.788670;0.351300;,
  0.769990;0.316120;,
  0.758670;0.328720;,
  0.695470;0.090580;,
  0.682100;0.090580;,
  0.682730;0.095960;,
  0.693830;0.095960;,
  0.668730;0.090580;,
  0.671630;0.095960;,
  0.659540;0.090580;,
  0.662210;0.095960;,
  0.666560;0.049640;,
  0.682000;0.049640;,
  0.682000;0.044260;,
  0.670180;0.044260;,
  0.697430;0.049640;,
  0.693810;0.044260;,
  0.660230;0.049650;,
  0.669410;0.049650;,
  0.672320;0.044280;,
  0.662900;0.044280;,
  0.682780;0.049650;,
  0.683420;0.044280;,
  0.695290;0.090230;,
  0.681920;0.090230;,
  0.681920;0.095600;,
  0.693970;0.095600;,
  0.668550;0.090230;,
  0.669880;0.095600;,
  0.822810;0.345930;,
  0.818670;0.328720;,
  0.818670;0.363130;,
  0.806980;0.377730;,
  0.788670;0.380340;,
  0.770360;0.377730;,
  0.758670;0.363130;,
  0.754520;0.345930;,
  0.705260;0.090580;,
  0.701950;0.095960;,
  0.696160;0.069780;,
  0.705940;0.069780;,
  0.695290;0.070100;,
  0.681920;0.070100;,
  0.668550;0.070100;,
  0.705260;0.070460;,
  0.695470;0.070460;,
  0.682100;0.070460;,
  0.668730;0.070460;,
  0.659540;0.070460;,
  0.666560;0.069760;,
  0.682000;0.069760;,
  0.697430;0.069760;,
  0.660230;0.069780;,
  0.669410;0.069780;,
  0.682780;0.069780;,
  0.696160;0.089900;,
  0.694520;0.095280;,
  0.702640;0.095280;,
  0.705940;0.089900;,
  0.807450;0.376830;,
  0.788780;0.341660;,
  0.788780;0.381440;,
  0.770100;0.376830;,
  0.758770;0.364240;,
  0.695470;0.050330;,
  0.693830;0.044960;,
  0.682730;0.044960;,
  0.682100;0.050330;,
  0.671630;0.044960;,
  0.668730;0.050330;,
  0.663690;0.044960;,
  0.659540;0.050330;,
  0.666560;0.089890;,
  0.669350;0.095260;,
  0.682000;0.095260;,
  0.682000;0.089890;,
  0.694640;0.095260;,
  0.697430;0.089890;,
  0.660230;0.089900;,
  0.664380;0.095280;,
  0.672320;0.095280;,
  0.669410;0.089900;,
  0.683420;0.095280;,
  0.682780;0.089900;,
  0.695290;0.049980;,
  0.693970;0.044600;,
  0.681920;0.044600;,
  0.681920;0.049980;,
  0.669880;0.044600;,
  0.668550;0.049980;,
  0.822920;0.347030;,
  0.818770;0.364240;,
  0.818770;0.329820;,
  0.808380;0.317520;,
  0.788780;0.312610;,
  0.769170;0.317520;,
  0.758770;0.329820;,
  0.754630;0.347030;,
  0.705260;0.050330;,
  0.701950;0.044960;;
 }
}
