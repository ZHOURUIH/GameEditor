using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;

namespace GameUpdate
{
	/// <summary>
	/// OKDialog.xaml 的交互逻辑
	/// </summary>
	public partial class OKDialog : Window
	{
		public OKDialog()
		{
			InitializeComponent();
		}
		private void mOKButton_Click(object sender, RoutedEventArgs e)
		{
			Hide();
		}
		public void setInfo(string info)
		{
			mInfoText.Text = info;
		}
		public void setTitle(string title)
		{
			Title = title;
		}
		private void windowMove_Click(object sender, MouseEventArgs e)
		{
			if (e.LeftButton == MouseButtonState.Pressed)
			{
				DragMove();
			}
		}
	}
}
