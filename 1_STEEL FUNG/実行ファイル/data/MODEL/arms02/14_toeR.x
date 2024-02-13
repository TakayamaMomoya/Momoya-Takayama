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
 32;
 -23.14890;-47.43542;-104.42312;,
 -23.14890;-1.42679;-29.07856;,
 23.12421;-1.42679;-29.07856;,
 23.12421;-47.43542;-104.42312;,
 -23.14890;-1.42679;-29.07856;,
 -23.14890;-1.42679;32.97583;,
 23.12421;-1.42679;32.97583;,
 23.12421;-1.42679;-29.07856;,
 -23.14890;-1.42679;32.97583;,
 -23.14890;-44.17482;32.97583;,
 23.12421;-44.17482;32.97583;,
 23.12421;-1.42679;32.97583;,
 -23.14890;-66.50728;32.97583;,
 -23.14890;-66.50728;-116.39527;,
 23.12421;-66.50728;-116.39527;,
 23.12421;-66.50728;32.97583;,
 23.12421;-47.43542;-104.42312;,
 23.12421;-1.42679;-29.07856;,
 23.12421;-1.42679;32.97583;,
 23.12421;-44.17482;32.97583;,
 -23.14890;-44.17482;32.97583;,
 -23.14890;-1.42679;32.97583;,
 -23.14890;-1.42679;-29.07856;,
 -23.14890;-47.43542;-104.42312;,
 -23.14890;-66.50728;-116.39527;,
 23.12421;-66.50728;-116.39527;,
 23.12421;-66.50728;-116.39527;,
 23.12421;-66.50728;32.97583;,
 -23.14890;-66.50728;32.97583;,
 23.12421;-66.50728;32.97583;,
 -23.14890;-66.50728;32.97583;,
 -23.14890;-66.50728;-116.39527;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,0,3,25;,
 4;26,16,19,27;,
 4;9,28,29,10;,
 4;30,20,23,31;;
 
 MeshMaterialList {
  1;
  10;
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
    "data\\TEXTURE\\MATERIAL\\arms_00.png";
   }
  }
 }
 MeshNormals {
  8;
  0.000000;-1.000000;-0.000000;,
  0.000000;0.962668;-0.270685;,
  0.000000;1.000000;0.000000;,
  0.000000;0.711462;-0.702724;,
  0.000000;-0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.531666;-0.846954;;
  10;
  4;3,1,1,3;,
  4;1,2,2,1;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;7,3,3,7;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;6,6,6,6;;
 }
 MeshTextureCoords {
  32;
  0.495160;0.812190;,
  0.535080;0.863920;,
  0.503310;0.888440;,
  0.463380;0.836710;,
  0.554230;0.957250;,
  0.554230;0.877390;,
  0.613780;0.877390;,
  0.613780;0.957250;,
  0.496870;0.965420;,
  0.496870;0.928340;,
  0.537010;0.928340;,
  0.537010;0.965420;,
  0.398040;0.964160;,
  0.398040;0.834600;,
  0.438170;0.834600;,
  0.438170;0.964160;,
  0.456170;0.844990;,
  0.496070;0.910330;,
  0.496070;0.964160;,
  0.459000;0.964160;,
  0.457780;0.964160;,
  0.494850;0.964160;,
  0.494850;0.910330;,
  0.454950;0.844990;,
  0.488810;0.803970;,
  0.457040;0.828490;,
  0.439630;0.834600;,
  0.439630;0.964160;,
  0.496870;0.908970;,
  0.537010;0.908970;,
  0.438410;0.964160;,
  0.438410;0.834600;;
 }
}
