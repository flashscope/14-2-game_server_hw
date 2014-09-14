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

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 InitD3D(IntPtr hWnd, Int32 width = 1280, Int32 height = 720);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void D3DCleanUp();

        [DllImport("YaMangDxDll.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 InitGeometry(IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] String fileName, ref IntPtr inputVal);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PreRendering();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Rendering(ref IntPtr inputVal);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void PostRendering();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderingTool(ref IntPtr inputVal);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MeshObjectCleanUp(ref IntPtr inputVal);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetAspectRatio(Int32 width, Int32 height);

        //////////////////////////////////////////////////////////////////////////
        //이하 height map 전용 함수
        //eight map은 일반 object와 g_D3D g_D3DDevice를 공유합니다.
        //////////////////////////////////////////////////////////////////////////

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 HeightMapTextureImport(IntPtr hWnd, [MarshalAs(UnmanagedType.LPWStr)] String heightMap, [MarshalAs(UnmanagedType.LPWStr)] String mapTexture);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void HeightMapCleanup();

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void HeightMapRender();

        //////////////////////////////////////////////////////////////////////////
        //Text Render를 위한 함수
        //////////////////////////////////////////////////////////////////////////

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenderText([MarshalAs(UnmanagedType.LPWStr)] String text, Int32 left, Int32 top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, Int32 right = 0, Int32 bottom = 0);

        //////////////////////////////////////////////////////////////////////////
        //Camera Set 함수
        //////////////////////////////////////////////////////////////////////////
        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void MoveCamera(float x, float y, float z);

        [DllImport("yaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ZoomCamera(float zoom);

        [DllImport("YaMangDxDll.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCameraView(float x, float y, float z);

        //DLL 내부 MESHOBJECT 정의
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct MESHOBJECT
        {
            IntPtr importedMesh;
            IntPtr MeshMarterials;
            IntPtr MeshTexture;

            UInt32 NumMaterials;
        };
    };
}
