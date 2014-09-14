namespace YamangTools
{
    partial class Main
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.eunjarimInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.RenderTarget = new System.Windows.Forms.PictureBox();
            this.sideMenu = new System.Windows.Forms.TabControl();
            this.Terrain = new System.Windows.Forms.TabPage();
            this.Lower = new System.Windows.Forms.CheckBox();
            this.Higher = new System.Windows.Forms.CheckBox();
            this.Texture = new System.Windows.Forms.TabPage();
            this.Object = new System.Windows.Forms.TabPage();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.MapWidthVal = new System.Windows.Forms.TextBox();
            this.MapHeightVal = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.MapVertexSpacingVal = new System.Windows.Forms.TextBox();
            this.TextureTest = new System.Windows.Forms.CheckBox();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RenderTarget)).BeginInit();
            this.sideMenu.SuspendLayout();
            this.Terrain.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1051, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(93, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.eunjarimInfoToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // eunjarimInfoToolStripMenuItem
            // 
            this.eunjarimInfoToolStripMenuItem.Name = "eunjarimInfoToolStripMenuItem";
            this.eunjarimInfoToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.eunjarimInfoToolStripMenuItem.Text = "Eunjarim info";
            // 
            // RenderTarget
            // 
            this.RenderTarget.Location = new System.Drawing.Point(295, 51);
            this.RenderTarget.Name = "RenderTarget";
            this.RenderTarget.Size = new System.Drawing.Size(729, 588);
            this.RenderTarget.TabIndex = 2;
            this.RenderTarget.TabStop = false;
            this.RenderTarget.MouseDown += new System.Windows.Forms.MouseEventHandler(this.RenderFrameClick);
            // 
            // sideMenu
            // 
            this.sideMenu.Controls.Add(this.Terrain);
            this.sideMenu.Controls.Add(this.Texture);
            this.sideMenu.Controls.Add(this.Object);
            this.sideMenu.Location = new System.Drawing.Point(12, 29);
            this.sideMenu.Name = "sideMenu";
            this.sideMenu.SelectedIndex = 0;
            this.sideMenu.Size = new System.Drawing.Size(277, 614);
            this.sideMenu.TabIndex = 4;
            // 
            // Terrain
            // 
            this.Terrain.Controls.Add(this.TextureTest);
            this.Terrain.Controls.Add(this.Lower);
            this.Terrain.Controls.Add(this.Higher);
            this.Terrain.Location = new System.Drawing.Point(4, 22);
            this.Terrain.Name = "Terrain";
            this.Terrain.Padding = new System.Windows.Forms.Padding(3);
            this.Terrain.Size = new System.Drawing.Size(269, 588);
            this.Terrain.TabIndex = 0;
            this.Terrain.Text = "Terrain";
            this.Terrain.UseVisualStyleBackColor = true;
            // 
            // Lower
            // 
            this.Lower.AutoSize = true;
            this.Lower.Location = new System.Drawing.Point(6, 28);
            this.Lower.Name = "Lower";
            this.Lower.Size = new System.Drawing.Size(59, 16);
            this.Lower.TabIndex = 2;
            this.Lower.Text = "Lower";
            this.Lower.UseVisualStyleBackColor = true;
            // 
            // Higher
            // 
            this.Higher.AutoSize = true;
            this.Higher.Checked = true;
            this.Higher.CheckState = System.Windows.Forms.CheckState.Checked;
            this.Higher.Location = new System.Drawing.Point(6, 6);
            this.Higher.Name = "Higher";
            this.Higher.Size = new System.Drawing.Size(60, 16);
            this.Higher.TabIndex = 1;
            this.Higher.Text = "Higher";
            this.Higher.UseVisualStyleBackColor = true;
            // 
            // Texture
            // 
            this.Texture.Location = new System.Drawing.Point(4, 22);
            this.Texture.Name = "Texture";
            this.Texture.Padding = new System.Windows.Forms.Padding(3);
            this.Texture.Size = new System.Drawing.Size(269, 588);
            this.Texture.TabIndex = 1;
            this.Texture.Text = "Texture";
            this.Texture.UseVisualStyleBackColor = true;
            // 
            // Object
            // 
            this.Object.Location = new System.Drawing.Point(4, 22);
            this.Object.Name = "Object";
            this.Object.Padding = new System.Windows.Forms.Padding(3);
            this.Object.Size = new System.Drawing.Size(269, 588);
            this.Object.TabIndex = 2;
            this.Object.Text = "Object";
            this.Object.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(296, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "Map Size";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(361, 33);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "Width(X)";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(492, 33);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(58, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "Height(Z)";
            // 
            // MapWidthVal
            // 
            this.MapWidthVal.Location = new System.Drawing.Point(420, 27);
            this.MapWidthVal.Name = "MapWidthVal";
            this.MapWidthVal.Size = new System.Drawing.Size(48, 21);
            this.MapWidthVal.TabIndex = 6;
            this.MapWidthVal.Text = "64";
            this.MapWidthVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // MapHeightVal
            // 
            this.MapHeightVal.Location = new System.Drawing.Point(556, 27);
            this.MapHeightVal.Name = "MapHeightVal";
            this.MapHeightVal.Size = new System.Drawing.Size(48, 21);
            this.MapHeightVal.TabIndex = 7;
            this.MapHeightVal.Text = "64";
            this.MapHeightVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(625, 33);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(51, 12);
            this.label4.TabIndex = 7;
            this.label4.Text = "Spacing";
            // 
            // MapVertexSpacingVal
            // 
            this.MapVertexSpacingVal.Location = new System.Drawing.Point(689, 27);
            this.MapVertexSpacingVal.Name = "MapVertexSpacingVal";
            this.MapVertexSpacingVal.Size = new System.Drawing.Size(48, 21);
            this.MapVertexSpacingVal.TabIndex = 8;
            this.MapVertexSpacingVal.Text = "5.0";
            this.MapVertexSpacingVal.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // TextureTest
            // 
            this.TextureTest.AutoSize = true;
            this.TextureTest.Location = new System.Drawing.Point(6, 51);
            this.TextureTest.Name = "TextureTest";
            this.TextureTest.Size = new System.Drawing.Size(92, 16);
            this.TextureTest.TabIndex = 3;
            this.TextureTest.Text = "TextureTest";
            this.TextureTest.UseVisualStyleBackColor = true;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1051, 655);
            this.Controls.Add(this.MapVertexSpacingVal);
            this.Controls.Add(this.MapHeightVal);
            this.Controls.Add(this.MapWidthVal);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.sideMenu);
            this.Controls.Add(this.RenderTarget);
            this.Controls.Add(this.menuStrip1);
            this.Name = "Main";
            this.Text = "HeightMap Tool";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Main_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RenderTarget)).EndInit();
            this.sideMenu.ResumeLayout(false);
            this.Terrain.ResumeLayout(false);
            this.Terrain.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.PictureBox RenderTarget;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem eunjarimInfoToolStripMenuItem;

        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox MapVertexSpacingVal;

        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox MapHeightVal;

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox MapWidthVal;

        private System.Windows.Forms.TabControl sideMenu;
        private System.Windows.Forms.TabPage Terrain;
        private System.Windows.Forms.TabPage Texture;
        private System.Windows.Forms.TabPage Object;
        private System.Windows.Forms.CheckBox Lower;
        private System.Windows.Forms.CheckBox Higher;
        private System.Windows.Forms.CheckBox TextureTest;
    }
}

