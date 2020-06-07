using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xamarin.Forms;

using MUSECommonLibrary;

namespace MUSEApp
{
    [DesignTimeVisible(false)]
    public partial class MainPage : ContentPage
    {
        List<string> constants = new List<string>(new string[] { "pi", "e" });

        const double euler = 2.7182818284590452353602874713527;
        const double pi = 3.1415926535897932384626433832794;

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

        private async void buttSubmit_Clicked(object sender, EventArgs e)
        {
            double num;
            var num_text = txtNumber.Text.Trim();
            if (num_text == "pi")
            {
                num = pi;
            }
            else if (num_text == "e")
            {
                num = euler;
            }
            else if (!double.TryParse(num_text, out num))
            {
                await DisplayAlert("Invalid", "There was an error parsing the number", "Ok");
                return;
            }

            int count;
            if (!int.TryParse(txtIterations.Text, out count))
            {
                await DisplayAlert("Invalid", "There was an error parsing the iterations", "Ok");
                return;
            }

            ContinuedFractionResponse resp = await WebServiceCom.POSTContinuedFractionRequest(num, count);
            if (resp == null)
            {
                await DisplayAlert("Server Error", "There was an issue communicating the server", "Ok");
                return;
            }
            else if (!string.IsNullOrWhiteSpace(resp.ServerResponse))
            {
                await DisplayAlert("Bad Request", resp.ServerResponse, "Ok");
                return;
            }
            else
            {
                txtOutput.Text = convertEnumerableToString(resp.qSequence);
            }
        }

        private string convertEnumerableToString(IEnumerable<int> qSequence)
        {
            var s = new StringBuilder();
            foreach (var q in qSequence)
            {
                if (q > 0)
                {
                    s.AppendFormat("{0}, ", q);
                }
            }

            if (s.Length > 2)
            {
                s.Length -= 2;
            }

            return s.ToString();
        }
    }
}
