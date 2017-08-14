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
            ((System.ComponentModel.ISupportInitialize)(this.tracker)).BeginInit();
            this.SuspendLayout();
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.Location = new System.Drawing.Point(4, 4);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(35, 14);
            this.lblName.TabIndex = 0;
            this.lblName.Text = "参数";
            // 
            // tracker
            // 
            this.tracker.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.tracker.Location = new System.Drawing.Point(0, 25);
            this.tracker.Name = "tracker";
            this.tracker.Size = new System.Drawing.Size(306, 50);
            this.tracker.TabIndex = 1;
            this.tracker.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            this.tracker.ValueChanged += new System.EventHandler(this.tracker_ValueChanged);
            // 
            // lblValue
            // 
            this.lblValue.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.lblValue.AutoEllipsis = true;
            this.lblValue.Location = new System.Drawing.Point(226, 4);
            this.lblValue.Name = "lblValue";
            this.lblValue.Size = new System.Drawing.Size(77, 23);
            this.lblValue.TabIndex = 2;
            this.lblValue.Text = "值";
            this.lblValue.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // ArgTrackBar
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.lblValue);
            this.Controls.Add(this.tracker);
            this.Controls.Add(this.lblName);
            this.Name = "ArgTrackBar";
            this.Size = new System.Drawing.Size(306, 75);
            ((System.ComponentModel.ISupportInitialize)(this.tracker)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.TrackBar tracker;
        private System.Windows.Forms.Label lblValue;
    }
}
