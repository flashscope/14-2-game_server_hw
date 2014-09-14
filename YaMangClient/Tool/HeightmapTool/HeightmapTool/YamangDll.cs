using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace YamangTools
{
    class YamangDll
    {
        ////////////////////////////////////////////////////////////
        // C++ 타입을 common language로 C#과 호환되도록 하기 - 
        // http://msdn.microsoft.com/en-us/library/Aa712982
        /// ////////////////////////////////////////////////////////

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl )]
        public static extern Int32 InitD3D(IntPtr hWnd, long width = 1280, long height = 720);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void D3DCleanUp();

        [DllImport("YaMangDxDll.dll",  CharSet = CharSet.Unicode , CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 InitGeometry( IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] String fileName, ref IntPtr inputVal);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PreRendering();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Rendering(ref IntPtr inputVal);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PostRendering();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MeshObjectCleanUp( ref IntPtr inputVal );

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        //public static extern void SetMatrix( D3DXMATRIXA16* matrix, bool cameraSet = false );
        public static extern void SetMatrix(ref IntPtr matrix, bool cameraSet = false);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetAspectRatio(long width, long height);

        //////////////////////////////////////////////////////////////////////////
        //이하 height map 전용 함수
        //eight map은 일반 object와 g_D3D g_D3DDevice를 공유합니다.
        //////////////////////////////////////////////////////////////////////////

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 MapToolTextureImport(IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] String toolTexture );

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 HeightMapTextureImport( IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] String heightMap, [MarshalAs(UnmanagedType.LPWStr)] String mapTexture );

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void HeightMapCleanup();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ToolViewSetting(int width, int height);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderHeightMap();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InitGroundMesh( int row, int col );

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateRawGround( int row, int col, float pixelSize );


        /////////////////////////////////////////////////////////////////////////
        //peeking 관련 함수
        /////////////////////////////////////////////////////////////////////////

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CalcPickingRay(int mouseX, int mouseY);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 TransPickedTriangle(ref float pickedX, ref float pickedZ, ref int PickedTriPointA, ref int PickedTriPointB, ref int PickedTriPointC);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MapToolPickingEvent(int modeSelector, int PickedTriPointA, int PickedTriPointB, int PickedTriPointC);


        //////////////////////////////////////////////////////////////////////////
        //Text Render를 위한 함수
        //////////////////////////////////////////////////////////////////////////

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderText([MarshalAs(UnmanagedType.LPWStr)] String text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0);


        //DLL 내부 MESHOBJECT 정의
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct MESHOBJECT
        {
            IntPtr importedMesh;
            IntPtr MeshMarterials;
            IntPtr MeshTexture;

            UInt32 NumMaterials;
        };

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct ARGMATRIX
        {
            public float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44;
        };
    }
}
