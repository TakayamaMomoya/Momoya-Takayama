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
 28;
 -18.59934;-1.04384;-23.74211;,
 18.33709;-1.04384;-23.74211;,
 18.33709;-271.76641;-23.74211;,
 -18.59934;-271.76641;-23.74211;,
 18.33709;13.36338;11.04852;,
 -18.59934;13.36338;11.04852;,
 -18.59934;-303.61363;11.04852;,
 18.33709;-303.61363;11.04852;,
 18.33709;13.36338;-4.77670;,
 18.33709;13.36338;11.04852;,
 18.33709;-303.61363;11.04852;,
 18.33709;-301.36026;-6.20500;,
 18.33709;-271.76641;-23.74211;,
 18.33709;-1.04384;-23.74211;,
 -18.59934;13.36338;-5.48045;,
 -18.59934;-1.04384;-23.74211;,
 -18.59934;-271.76641;-23.74211;,
 -18.59934;-300.71885;-6.90565;,
 -18.59934;-303.61363;11.04852;,
 -18.59934;13.36338;11.04852;,
 18.33709;13.36338;-4.77670;,
 18.33709;-1.04384;-23.74211;,
 -18.59934;-1.04384;-23.74211;,
 -18.59934;13.36338;-5.48045;,
 18.33709;-301.36026;-6.20500;,
 -18.59934;-300.71885;-6.90565;,
 -18.59934;-271.76641;-23.74211;,
 18.33709;-271.76641;-23.74211;;
 
 10;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,15,16,17;,
 4;14,17,18,19;,
 4;20,21,22,23;,
 4;20,23,19,9;,
 4;24,10,18,25;,
 4;24,25,26,27;;
 
 MeshMaterialList {
  3;
  10;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.116078;0.332549;0.385882;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.508235;0.702745;0.759216;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  9;
  0.003209;0.440351;-0.897820;,
  0.001934;-0.262312;-0.964981;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.003045;0.946232;-0.323473;,
  0.000000;1.000000;0.000000;,
  -0.001847;-0.829594;-0.558364;,
  -0.007062;-0.989501;-0.144352;;
  10;
  4;0,0,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;5,0,0,5;,
  4;5,5,6,6;,
  4;7,8,8,7;,
  4;7,7,1,1;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.545129;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.504075;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  0.475097;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.516065;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.454871;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.475097;,
  1.000000;0.504075;,
  0.000000;0.483936;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}
