using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MUSECommonLibrary
{
    public static class GlobalVariables
    {
        public static string Scheme = "https";
        public static string Host = "";
        public static int Port = 8080;
        public static bool UsePortNumber = false; //only set to true if the port number is needed
        public static string AppPath = "";
    }
}
