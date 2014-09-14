using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace YamangTools
{
    public partial class Main : Form
    {
        bool renderStop = false;
        bool meshSizeChanged = true;
        bool mouseEventFlag = false;
        
        int actionFlag = 0;

        int preWidth = 0;
        int preHeight = 0;
        int preMapSpace = 0;

        int mouseXPosition = 0;
        int mouseYPosition = 0;

        float returnedXPos = 0.0f;
        float returnedZPos = 0.0f;

        int PickedTriPointA = 0;
        int PickedTriPointB = 0;
        int PickedTriPointC = 0;

        string heightMap = ".\\MapResource\\heightmap_128_128.bmp";
        string mapTexture = ".\\MapResource\\heightmap_128_128.bmp";
//         string mapTexture0 = ".\\MapResource\\heightmap_128_128.bmp";
//         string mapTexture1 = ".\\TextureResource\\grass.bmp";
//         string mapTexture2 = ".\\TextureResource\\leaf.bmp";
//         string mapTexture3 = ".\\TextureResource\\sky.bmp";
//         string mapTexture4 = ".\\TextureResource\\stone.bmp";


        ~Main()
        {
        }

        public Main()
        {
            InitializeComponent();
            YamangDll.InitD3D(this.RenderTarget.Handle);
            YamangDll.HeightMapTextureImport(this.RenderTarget.Handle, heightMap, mapTexture);
//             YamangDll.MapToolTextureImport(this.RenderTarget.Handle, mapTexture0);
//             YamangDll.MapToolTextureImport(this.RenderTarget.Handle, mapTexture1);
//             YamangDll.MapToolTextureImport(this.RenderTarget.Handle, mapTexture2);
//             YamangDll.MapToolTextureImport(this.RenderTarget.Handle, mapTexture3);
//             YamangDll.MapToolTextureImport(this.RenderTarget.Handle, mapTexture4);
            Render();
        }

        async private void Render()
        {
            
            while (!renderStop)
            {
                YamangDll.PreRendering();
                int curWidth = Convert.ToInt32(GetNumber(MapHeightVal));
                int curHeight = Convert.ToInt32(GetNumber(MapWidthVal));
                int curMapSpace = Convert.ToInt32(GetNumber(MapVertexSpacingVal));
                
                if((preWidth != curWidth) || (preHeight != curHeight) || (preMapSpace != curMapSpace))
                {
                    meshSizeChanged = true;
                }

                if(meshSizeChanged)
                {
                    meshSizeChanged = false;

                    YamangDll.InitGroundMesh( curWidth, curHeight );
                    YamangDll.CreateRawGround(curWidth, curHeight, curMapSpace);
                }

                YamangDll.ToolViewSetting(RenderTarget.Size.Width, RenderTarget.Size.Height);

                if(mouseEventFlag)
                {
                    SetAction();

                    YamangDll.CalcPickingRay(mouseXPosition, mouseYPosition);
                    //YamangDll.TransPickedTriangle(ref returnedXPos, ref returnedZPos);
                    YamangDll.TransPickedTriangle(ref returnedXPos, ref returnedZPos, ref PickedTriPointA, ref PickedTriPointB, ref PickedTriPointC);
                    
                    //이거 활용해서 해당 위치에 나무 같은거 뿌리면 될 듯
                    Console.WriteLine("A: " + PickedTriPointA + "\nB: " + PickedTriPointB + "\nC: " + PickedTriPointC);
                    Console.WriteLine("Xpos: " + returnedXPos + ", ZPos: " + returnedZPos);
                    
                    YamangDll.MapToolPickingEvent(actionFlag, PickedTriPointA, PickedTriPointB, PickedTriPointC);

                    mouseEventFlag = false;
                }
                YamangDll.RenderHeightMap();
                YamangDll.PostRendering();

                preWidth = curWidth;
                preHeight = curHeight;
                preMapSpace = curMapSpace;

                await Task.Delay(10);
            }
        }

        private void SetAction()
        {
            if (Higher.Checked)
            {
                actionFlag = 1;
            }
            else if (Lower.Checked)
            {
                actionFlag = 2;
            }
            else if (TextureTest.Checked)
            {
                actionFlag = 3;
            }
            else
            {
                actionFlag = 0;
            }
        }

        private float GetNumber(object target)
        {
            float result;

            try
            {
                result = Convert.ToSingle(((TextBox)target).Text);
                if (result <= 0)
                {
                    ((TextBox)target).Text = "1";
                    result = 1;
                }
            }
            catch
            {
                ((TextBox)target).Text = "1";
                result = 1;
            }
            return result;
        }

        private void Main_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (e.CloseReason == CloseReason.UserClosing)
            {
                renderStop = true;
                YamangDll.HeightMapCleanup();
                YamangDll.D3DCleanUp();
                MessageBox.Show(this, "맵툴을 종료합니다", "Closing...", MessageBoxButtons.OK, MessageBoxIcon.Question);
            }
        }

        private void RenderFrameClick(object sender, MouseEventArgs e)
        {
            mouseEventFlag = true;

            mouseXPosition = (e.X + ((Control)sender).Location.X) - 295;
            mouseYPosition = (e.Y + ((Control)sender).Location.Y) - 51;

            Console.WriteLine("x좌표 :" + mouseXPosition + ", y좌표 :" + mouseYPosition);
        }
    }
}
