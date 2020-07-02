using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;
using Newtonsoft.Json;

namespace TestAPP
{
    public static class CPPMathLibTest
    {
        private const string CPPDLLPath = @"c:/users/jinge/source/muse/cpplibrary/debug/cpplibrary.dll";

        [DllImport(CPPDLLPath, CallingConvention = CallingConvention.Cdecl)]
        private extern static uint GreatestCommonDivisor(uint n1, uint n2);

        public static uint GetGCD(uint n1, uint n2)
        {
            return GreatestCommonDivisor(n1, n2);
        }
    }
}
