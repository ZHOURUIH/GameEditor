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
			mInfo.Content = info;
		}
		public void setTitle(string title)
		{
			Title = title;
		}
		private void windowMove_Click(object sender, MouseEventArgs e)
		{
			if (e.LeftButton == MouseButtonState.Pressed)
			{
				this.DragMove();
			}
		}
	}
}
