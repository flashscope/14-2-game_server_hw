xof 0303txt 0032
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

template MeshMaterialList {
 <f6f23f42-7686-11cf-8f52-0040333594a3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3d82ab4d-62da-11cf-ab39-0020af71e433>]
}

template TextureFilename {
 <a42790e1-7810-11cf-8f52-0040333594a3>
 STRING filename;
}


Frame Box001 {
 

 FrameTransformMatrix {
  1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000,0.000000,0.000000,-1.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000;;
 }

 Mesh {
  48;
  5.000000;5.000000;0.000000;,
  -15.000000;-5.000000;0.000000;,
  15.000000;-5.000000;0.000000;,
  -5.000000;5.000000;0.000000;,
  5.000000;5.000000;-10.000000;,
  -15.000000;-5.000000;-10.000000;,
  -5.000000;5.000000;-10.000000;,
  15.000000;-5.000000;-10.000000;,
  15.000000;-5.000000;-10.000000;,
  -15.000000;-5.000000;0.000000;,
  -15.000000;-5.000000;-10.000000;,
  15.000000;-5.000000;0.000000;,
  15.000000;5.000000;-10.000000;,
  15.000000;-5.000000;0.000000;,
  15.000000;-5.000000;-10.000000;,
  15.000000;5.000000;0.000000;,
  -15.000000;-5.000000;-10.000000;,
  -15.000000;5.000000;0.000000;,
  -15.000000;5.000000;-10.000000;,
  -15.000000;-5.000000;0.000000;,
  15.000000;5.000000;0.000000;,
  15.000000;5.000000;-10.000000;,
  5.000000;5.000000;0.000000;,
  15.000000;5.000000;0.000000;,
  -15.000000;5.000000;0.000000;,
  -5.000000;5.000000;0.000000;,
  -15.000000;5.000000;-10.000000;,
  -15.000000;5.000000;0.000000;,
  -5.000000;5.000000;-10.000000;,
  -15.000000;5.000000;-10.000000;,
  5.000000;5.000000;-10.000000;,
  15.000000;5.000000;-10.000000;,
  5.000000;15.000000;0.000000;,
  -5.000000;15.000000;0.000000;,
  5.000000;15.000000;-10.000000;,
  -5.000000;15.000000;-10.000000;,
  5.000000;15.000000;-10.000000;,
  5.000000;5.000000;0.000000;,
  5.000000;5.000000;-10.000000;,
  5.000000;15.000000;0.000000;,
  -5.000000;15.000000;-10.000000;,
  5.000000;15.000000;0.000000;,
  5.000000;15.000000;-10.000000;,
  -5.000000;15.000000;0.000000;,
  -5.000000;5.000000;-10.000000;,
  -5.000000;15.000000;0.000000;,
  -5.000000;15.000000;-10.000000;,
  -5.000000;5.000000;0.000000;;
  28;
  3;0,1,2;,
  3;1,0,3;,
  3;4,5,6;,
  3;5,4,7;,
  3;8,9,10;,
  3;9,8,11;,
  3;12,13,14;,
  3;13,12,15;,
  3;16,17,18;,
  3;17,16,19;,
  3;20,21,22;,
  3;23,0,2;,
  3;24,25,26;,
  3;27,1,3;,
  3;26,25,28;,
  3;29,6,5;,
  3;21,30,22;,
  3;31,7,4;,
  3;32,3,0;,
  3;3,32,33;,
  3;34,6,35;,
  3;6,34,4;,
  3;36,37,38;,
  3;37,36,39;,
  3;40,41,42;,
  3;41,40,43;,
  3;44,45,46;,
  3;45,44,47;;

  MeshNormals {
   14;
   0.000000;0.000000;1.000000;,
   0.000000;0.000000;-1.000000;,
   0.000000;-1.000000;0.000000;,
   1.000000;0.000000;0.000000;,
   -1.000000;0.000000;0.000000;,
   0.000000;1.000000;0.000000;,
   1.000000;-0.000000;0.000000;,
   1.000000;0.000000;0.000000;,
   1.000000;-0.000000;0.000000;,
   1.000000;-0.000000;0.000000;,
   -1.000000;-0.000000;-0.000000;,
   -1.000000;-0.000000;0.000000;,
   -1.000000;-0.000000;-0.000000;,
   -1.000000;0.000000;-0.000000;;
   28;
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
   3;0,0,0;,
   3;5,5,5;,
   3;0,0,0;,
   3;5,5,5;,
   3;1,1,1;,
   3;5,5,5;,
   3;1,1,1;,
   3;0,0,0;,
   3;0,0,0;,
   3;1,1,1;,
   3;1,1,1;,
   3;6,8,7;,
   3;8,6,9;,
   3;5,5,5;,
   3;5,5,5;,
   3;10,12,11;,
   3;12,10,13;;
  }

  MeshTextureCoords {
   48;
   0.195556;0.407222;,
   0.566668;0.592778;,
   0.010000;0.592778;,
   0.381112;0.407222;,
   0.195556;0.804444;,
   0.566668;0.618888;,
   0.381112;0.804444;,
   0.010000;0.618888;,
   0.010000;0.195556;,
   0.566668;0.010000;,
   0.566668;0.195556;,
   0.010000;0.010000;,
   0.778334;0.610185;,
   0.592778;0.795741;,
   0.592778;0.610185;,
   0.778334;0.795741;,
   0.778334;0.407222;,
   0.592778;0.592778;,
   0.592778;0.407222;,
   0.778334;0.592778;,
   0.592778;0.990000;,
   0.592778;0.804444;,
   0.778334;0.990000;,
   0.010000;0.407222;,
   0.972593;0.592778;,
   0.787037;0.592778;,
   0.972593;0.407222;,
   0.566668;0.407222;,
   0.787037;0.407222;,
   0.566668;0.804444;,
   0.778334;0.804444;,
   0.010000;0.804444;,
   0.195556;0.221666;,
   0.381112;0.221666;,
   0.195556;0.990000;,
   0.381112;0.990000;,
   0.972593;0.804444;,
   0.787037;0.990000;,
   0.787037;0.804444;,
   0.972593;0.990000;,
   0.972593;0.610185;,
   0.787037;0.795741;,
   0.787037;0.610185;,
   0.972593;0.795741;,
   0.778334;0.212963;,
   0.592778;0.398519;,
   0.592778;0.212963;,
   0.778334;0.398519;;
  }

  MeshMaterialList {
   1;
   28;
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
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
    0.588000;0.588000;0.588000;1.000000;;
    0.100000;
    0.900000;0.900000;0.900000;;
    0.000000;0.000000;0.000000;;

    TextureFilename {
     "Corps_Rush_A.dds";
    }
   }
  }
 }
}