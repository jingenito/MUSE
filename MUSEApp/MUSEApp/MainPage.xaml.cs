using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace MUSEApp
{
    [DesignTimeVisible(false)]
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        private async void buttShowConstants_Clicked(object sender, EventArgs e)
        {
            var s = new StringBuilder();
            s.AppendLine("pi");
            s.AppendLine("e");
            await DisplayAlert("All constants are case sensitive", s.ToString(), "Ok");
        }

        private void buttSubmit_Clicked(object sender, EventArgs e)
        {

        }
    }
}
