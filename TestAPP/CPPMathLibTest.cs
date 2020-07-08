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
        private extern static IntPtr _continuedFraction(double gamma, int count);

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ReleaseMemory", CallingConvention = CallingConvention.Cdecl)]
        private extern static int ReleaseMemory(IntPtr arr);

        public static int[] ContinuedFraction(double gamma, int count)
        {
            IntPtr qs = _continuedFraction(gamma, count);
            int arrayLength = Marshal.ReadInt32(qs);
            // points to arr[1], which is first value
            IntPtr start = IntPtr.Add(qs, 4);
            int[] result = new int[arrayLength];
            Marshal.Copy(start, result, 0, arrayLength);
            ReleaseMemory(qs);
            return result;
        }
    }
}
