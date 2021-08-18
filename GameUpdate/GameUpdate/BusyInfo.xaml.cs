using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;

namespace GameUpdate
{
	/// <summary>
	/// BusyInfo.xaml 的交互逻辑
	/// </summary>
	public partial class BusyInfo : Window
	{
		public BusyInfo()
		{
			InitializeComponent();
		}
		public void setInfo(string info)
		{
			mInfo.Content = info;
		}
		private void closeButton_Click(object sender, RoutedEventArgs e)
		{
			this.Close();
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
