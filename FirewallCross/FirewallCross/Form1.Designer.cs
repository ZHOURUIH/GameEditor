namespace FirewallCross
{
	partial class Form1
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

		#region Windows 窗体设计器生成的代码

		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
			this.mProgramName = new System.Windows.Forms.Label();
			this.mProgramPath = new System.Windows.Forms.Label();
			this.mProgramPathText = new System.Windows.Forms.TextBox();
			this.mProgramListCombo = new System.Windows.Forms.ComboBox();
			this.button1 = new System.Windows.Forms.Button();
			this.mResultText = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// mProgramName
			// 
			this.mProgramName.AutoSize = true;
			this.mProgramName.Location = new System.Drawing.Point(94, 61);
			this.mProgramName.Name = "mProgramName";
			this.mProgramName.Size = new System.Drawing.Size(35, 12);
			this.mProgramName.TabIndex = 0;
			this.mProgramName.Text = "程序:";
			// 
			// mProgramPath
			// 
			this.mProgramPath.AutoSize = true;
			this.mProgramPath.Location = new System.Drawing.Point(94, 96);
			this.mProgramPath.Name = "mProgramPath";
			this.mProgramPath.Size = new System.Drawing.Size(35, 12);
			this.mProgramPath.TabIndex = 1;
			this.mProgramPath.Text = "路径:";
			// 
			// mProgramPathText
			// 
			this.mProgramPathText.Location = new System.Drawing.Point(135, 93);
			this.mProgramPathText.Name = "mProgramPathText";
			this.mProgramPathText.Size = new System.Drawing.Size(345, 21);
			this.mProgramPathText.TabIndex = 3;
			// 
			// mProgramListCombo
			// 
			this.mProgramListCombo.FormattingEnabled = true;
			this.mProgramListCombo.Location = new System.Drawing.Point(135, 58);
			this.mProgramListCombo.Name = "mProgramListCombo";
			this.mProgramListCombo.Size = new System.Drawing.Size(345, 20);
			this.mProgramListCombo.TabIndex = 4;
			this.mProgramListCombo.SelectedIndexChanged += new System.EventHandler(this.mProgramListCombo_SelectedIndexChanged);
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(216, 162);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(149, 23);
			this.button1.TabIndex = 5;
			this.button1.Text = "添加程序到防火墙例外";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.OnAddProgram);
			// 
			// mResultText
			// 
			this.mResultText.AutoSize = true;
			this.mResultText.Location = new System.Drawing.Point(371, 167);
			this.mResultText.Name = "mResultText";
			this.mResultText.Size = new System.Drawing.Size(71, 12);
			this.mResultText.TabIndex = 6;
			this.mResultText.Text = "           ";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(632, 261);
			this.Controls.Add(this.mResultText);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.mProgramListCombo);
			this.Controls.Add(this.mProgramPathText);
			this.Controls.Add(this.mProgramPath);
			this.Controls.Add(this.mProgramName);
			this.Name = "Form1";
			this.Text = "防火墙设置";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label mProgramName;
		private System.Windows.Forms.Label mProgramPath;
		private System.Windows.Forms.TextBox mProgramPathText;
		private System.Windows.Forms.ComboBox mProgramListCombo;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Label mResultText;
	}
}

