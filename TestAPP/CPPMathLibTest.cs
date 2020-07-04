using System;
using System.Collections.Generic;
using System.Data;
using System.Runtime.InteropServices;
using System.Text;
using Newtonsoft.Json;

namespace TestAPP
{
    public static class CPPMathLibTest
    {
        private const string CPPDLLPath = @"c:/users/jinge/source/muse/cpplibrary/debug/cpplibrary.dll";

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_GetGCD", CallingConvention = CallingConvention.Cdecl)]
        private extern static int GreatestCommonDivisor(int n1, int n2);

        public static int GetGCD(int n1, int n2)
        {
            return GreatestCommonDivisor(n1, n2);
        }
    }
}
