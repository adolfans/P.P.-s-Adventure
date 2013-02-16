xof 0303txt 0032
template ColorRGBA {
 <35ff44e0-6c7c-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <d3e16e81-7835-11cf-8f52-0040333594a3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template Material {
 <3d82ab4d-62da-11cf-ab39-0020af71e433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}

template Frame {
 <3d82ab46-62da-11cf-ab39-0020af71e433>
 [...]
}

template Matrix4x4 {
 <f6f23f45-7686-11cf-8f52-0040333594a3>
 array FLOAT matrix[16];
}

template FrameTransformMatrix {
 <f6f23f41-7686-11cf-8f52-0040333594a3>
 Matrix4x4 frameMatrix;
}

template Vector {
 <3d82ab5e-62da-11cf-ab39-0020af71e433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template MeshFace {
 <3d82ab5f-62da-11cf-ab39-0020af71e433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template Mesh {
 <3d82ab44-62da-11cf-ab39-0020af71e433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

template MeshNormals {
 <f6f23f43-7686-11cf-8f52-0040333594a3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template Coords2d {
 <f6f23f44-7686-11cf-8f52-0040333594a3>
 FLOAT u;
 FLOAT v;
}

template MeshTextureCoords {
 <f6f23f40-7686-11cf-8f52-0040333594a3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}


Material _testMyloli2201___Default {
 1.000000;1.000000;1.000000;1.000000;;
 3.200000;
 0.000000;0.000000;0.000000;;
 0.000000;0.000000;0.000000;;

 TextureFilename {
  "testMyLoli.tga";
 }
}

Material blade_blade_mso__equalizerbladeSub0 {
 0.752900;0.760800;0.815700;1.000000;;
 32.000000;
 0.150000;0.150000;0.150000;;
 0.000000;0.000000;0.000000;;
}

Material blade_blade_mso__equalizerwhite_metalSub1 {
 0.545100;0.572500;0.658800;1.000000;;
 32.000000;
 0.150000;0.150000;0.150000;;
 0.000000;0.000000;0.000000;;
}

Material blade_blade_mso__equalizerblack_metalSub2 {
 0.192200;0.211800;0.270600;1.000000;;
 32.000000;
 0.150000;0.150000;0.150000;;
 0.000000;0.000000;0.000000;;
}

Material blade_blade_mso__equalizerhandleSub3 {
 0.988200;0.984300;0.984300;1.000000;;
 32.000000;
 0.150000;0.150000;0.150000;;
 0.000000;0.000000;0.000000;;
}

Frame Box001 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,273.356110,0.000000,-374.919067,1.000000;;
 }

 Mesh  {
  20;
  -758.31030;0.000000;-528.49677;,
  758.31030;0.000000;-528.49677;,
  -758.31030;0.000000;528.49677;,
  758.31030;0.000000;528.49677;,
  -758.31030;1563.85913;-528.49677;,
  758.31030;1563.85913;-528.49677;,
  -758.31030;1563.85913;528.49677;,
  758.31030;1563.85913;528.49677;,
  -758.31030;0.000000;-528.49677;,
  758.31030;1563.85913;-528.49677;,
  758.31030;0.000000;-528.49677;,
  -758.31030;1563.85913;-528.49677;,
  758.31030;0.000000;528.49677;,
  758.31030;1563.85913;-528.49677;,
  758.31030;0.000000;528.49677;,
  -758.31030;1563.85913;528.49677;,
  -758.31030;0.000000;528.49677;,
  758.31030;1563.85913;528.49677;,
  -758.31030;0.000000;528.49677;,
  -758.31030;1563.85913;-528.49677;;
  12;
  3;0,3,2;,
  3;3,0,1;,
  3;4,7,5;,
  3;7,4,6;,
  3;8,9,10;,
  3;9,8,11;,
  3;1,7,12;,
  3;7,1,13;,
  3;14,15,16;,
  3;15,14,17;,
  3;18,19,0;,
  3;19,18,6;;

  MeshNormals  {
   6;
   0.000000;-1.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   0.000000;0.000000;-1.000000;,
   1.000000;0.000000;0.000000;,
   0.000000;0.000000;1.000000;,
   -1.000000;0.000000;0.000000;;
   12;
   3;0,0,0;,
   3;0,0,0;,
   3;1,1,1;,
   3;1,1,1;,
   3;2,2,2;,
   3;2,2,2;,
   3;3,3,3;,
   3;3,3,3;,
   3;4,4,4;,
   3;4,4,4;,
   3;5,5,5;,
   3;5,5,5;;
  }

  MeshMaterialList  {
   1;
   12;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0;

   Material {
    0.333333;0.109804;0.694118;1.000000;;
    0.000000;
    0.333333;0.109804;0.694118;;
    0.000000;0.000000;0.000000;;
   }
  }

  MeshTextureCoords  {
   20;
   1.000000;1.000000;,
   0.000000;1.000000;,
   1.000000;0.000000;,
   0.000000;0.000000;,
   0.000000;1.000000;,
   1.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;0.000000;,
   0.000000;1.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;0.000000;,
   0.000000;1.000000;,
   1.000000;0.000000;,
   1.000000;1.000000;,
   0.000000;0.000000;,
   0.000000;1.000000;,
   1.000000;0.000000;;
  }
 }
}