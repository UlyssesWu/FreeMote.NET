namespace NekoHacks
{
    partial class ArgTrackBar
    {
        /// <summary> 
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region 组件设计器生成的代码

        /// <summary> 
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.lblName = new System.Windows.Forms.Label();
            this.tracker = new System.Windows.Forms.TrackBar();
            this.lblValue = new System.Windows.Forms.Label();
            this.flowPanel = new System.Windows.Forms.FlowLayoutPanel();
            this.panel1 = new System.Windows.Forms.Panel();
            ((System.ComponentModel.ISupportInitialize)(this.tracker)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Location = new System.Drawing.Point(6, 8);
            this.lblName.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(66, 27);
            this.lblName.TabIndex = 0;
            this.lblName.Text = "参数";
            // 
            // tracker
            // 
            this.tracker.AutoSize = false;
            this.tracker.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tracker.Location = new System.Drawing.Point(0, 55);
            this.tracker.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.tracker.Name = "tracker";
            this.tracker.Size = new System.Drawing.Size(612, 66);
            this.tracker.TabIndex = 1;
            this.tracker.TickFrequency = 100;
            this.tracker.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.tracker.ValueChanged += new System.EventHandler(this.tracker_ValueChanged);
            // 
            // lblValue
            // 
            this.lblValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblValue.AutoEllipsis = true;
            this.lblValue.Location = new System.Drawing.Point(452, 8);
            this.lblValue.Margin = new System.Windows.Forms.Padding(6, 0, 6, 0);
            this.lblValue.Name = "lblValue";
            this.lblValue.Size = new System.Drawing.Size(154, 42);
            this.lblValue.TabIndex = 2;
            this.lblValue.Text = "值";
            this.lblValue.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // flowPanel
            // 
            this.flowPanel.AutoSize = true;
            this.flowPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.flowPanel.Font = new System.Drawing.Font("宋体", 8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.flowPanel.Location = new System.Drawing.Point(0, 121);
            this.flowPanel.Name = "flowPanel";
            this.flowPanel.Size = new System.Drawing.Size(612, 0);
            this.flowPanel.TabIndex = 3;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.lblValue);
            this.panel1.Controls.Add(this.lblName);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(612, 56);
            this.panel1.TabIndex = 4;
            // 
            // ArgTrackBar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(14F, 27F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.tracker);
            this.Controls.Add(this.flowPanel);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(6, 6, 6, 6);
            this.Name = "ArgTrackBar";
            this.Size = new System.Drawing.Size(612, 121);
            ((System.ComponentModel.ISupportInitialize)(this.tracker)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.TrackBar tracker;
        private System.Windows.Forms.Label lblValue;
        private System.Windows.Forms.FlowLayoutPanel flowPanel;
        private System.Windows.Forms.Panel panel1;
    }
}
