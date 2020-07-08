using System;
using System.Runtime.InteropServices;

namespace TestAPP
{
    public static class CPPMathLibTest
    {
        private const string CPPDLLPath = @"c:/users/jinge/source/muse/cpplibrary/debug/cpplibrary.dll";

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_GetGCD", CallingConvention = CallingConvention.Cdecl)]
        private extern static int _greatestCommonDivisor(int n1, int n2);

        public static int GreatestCommonDivisor(int n1, int n2)
        {
            int t1 = (int)Math.Abs(n1);
            int t2 = (int)Math.Abs(n2);
            return _greatestCommonDivisor(t1, t2);
        }

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ContinuedFraction", CallingConvention = CallingConvention.Cdecl)]
        private extern static IntPtr _continuedFraction(double gamma, ref int count);

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ReleaseMemory", CallingConvention = CallingConvention.Cdecl)]
        private extern static int ReleaseMemory(IntPtr arr);

        public static int[] ContinuedFraction(double gamma, ref int count)
        {
            IntPtr qs = _continuedFraction(gamma, ref count);
            int[] result = new int[count];
            Marshal.Copy(qs, result, 0, count);
            ReleaseMemory(qs);
            return result;
        }
    }
}
