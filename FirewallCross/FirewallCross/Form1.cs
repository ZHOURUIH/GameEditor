using NetFwTypeLib;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FirewallCross
{
	public partial class Form1 : Form
	{
		protected EditorCore mEditorCore;
		protected DateTime mLastTime;
		protected Timer mTimer;
		public Form1()
		{
			InitializeComponent();
			mEditorCore = new EditorCore();
			mEditorCore.init();
			mLastTime = DateTime.Now;
			mTimer = new Timer();
			mTimer.Interval = 20;
			mTimer.Tick += onTimer;
			mTimer.Start();
			Dictionary<string, string> pathList = mEditorCore.getProgramPathList();
			foreach (var item in pathList)
			{
				mProgramListCombo.Items.AddRange(new object[] { item.Key });
			}
			if(mProgramListCombo.Items.Count > 0)
			{
				mProgramListCombo.SelectedIndex = 0;
			}
		}
		private void onTimer(object sender, EventArgs e)
		{
			DateTime curTime = DateTime.Now;
			mLastTime = curTime;
			int deltaTime = (int)(curTime - mLastTime).TotalMilliseconds;
			mEditorCore.update(deltaTime / 1000.0f);
		}
		private void OnAddProgram(object sender, EventArgs e)
		{
			bool ret = false;
			if (mProgramListCombo.SelectedItem != null)
			{
				ret = EditorUtility.addFirewallExceptions(mProgramListCombo.SelectedItem.ToString(), mProgramPathText.Text);
			}
			mResultText.Text = (ret ? "添加成功!" : "添加失败!");
		}

		private void mProgramListCombo_SelectedIndexChanged(object sender, EventArgs e)
		{
			mProgramPathText.Text = mEditorCore.getProgramPath(mProgramListCombo.SelectedItem.ToString());
		}
	}
}
