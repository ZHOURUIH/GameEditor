using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace GameUpdate
{
	/// <summary>
	/// OKDialog.xaml 的交互逻辑
	/// </summary>
	public partial class YesNoDialog : Window
	{
		public YesNoDialog()
		{
			InitializeComponent();
		}
		private void mButtonYes_Click(object sender, RoutedEventArgs e)
		{
			DialogResult = true;
		}

		private void mButtonNo_Click(object sender, RoutedEventArgs e)
		{
			DialogResult = false;
		}
		public void setInfo(string info)
		{
			mInfoBlock.Text = info;
		}
		public void setTitle(string title)
		{
			Title = title;
		}
	}
}
