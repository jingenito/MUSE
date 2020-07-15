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

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ReleaseMemoryIntArr", CallingConvention = CallingConvention.Cdecl)]
        private extern static int ReleaseMemoryIntPtr(IntPtr arr);

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ReleaseMemoryIntPtrPtr", CallingConvention = CallingConvention.Cdecl)]
        private extern unsafe static int ReleaseMemoryIntPtrPtr(int** arr, int n);

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ReleaseMemoryDoubleArr", CallingConvention = CallingConvention.Cdecl)]
        private extern unsafe static int ReleaseMemoryDoublePtrPtr(double** arr, int n);

        public static int[] ContinuedFraction(double gamma, ref int count)
        {
            IntPtr qs = _continuedFraction(gamma, ref count);
            int[] result = new int[count];
            Marshal.Copy(qs, result, 0, count);
            ReleaseMemoryIntPtr(qs);
            return result;
        }

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_PrintConvergents", CallingConvention = CallingConvention.Cdecl)]
        private extern static int _printConvergents(int[] qs, int count);

        public static void PrintConvergents(int[] qs, int count)
        {
            _printConvergents(qs, count);
        }

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_GramSchmidt", CallingConvention = CallingConvention.Cdecl)]
        private extern unsafe static double** _gramSchmidtOrthogonalization(double** matrix, int n);

        public static double[][] GramSchmidtOrthogonalization(double[][] matrix, int n)
        {
            double[][] m = new double[n][];

            unsafe
            {
                fixed(double* row = &matrix[0][0])
                {
                    double*[] arrayofptr = new double*[matrix.Length];
                    for (int i = 0; i < matrix.Length; i++)
                    {
                        fixed (double* ptr = &matrix[i][0])
                        {
                            arrayofptr[i] = ptr;
                        }
                    }

                    fixed (double** ptrptr = &arrayofptr[0])
                    {
                        double** result = _gramSchmidtOrthogonalization(ptrptr, n);

                        for(int i = 0; i < n; i++)
                        {
                            m[i] = new double[n];

                            for(int j = 0; j < n; j++)
                            {
                                m[i][j] = result[i][j];
                            }
                        }

                        ReleaseMemoryDoublePtrPtr(result, n);
                    }
                }
            }

            return m;
        }

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_ReduceBasis_LLL", CallingConvention = CallingConvention.Cdecl)]
        private extern unsafe static double** _reduceBasisLLL(double** matrix, int n, double alpha);

        public static double[][] ReduceBasisLLL(double[][] matrix, int n, double alpha)
        {
            double[][] m = new double[n][];

            unsafe
            {
                fixed (double* row = &matrix[0][0])
                {
                    double*[] arrayofptr = new double*[matrix.Length];
                    for (int i = 0; i < matrix.Length; i++)
                    {
                        fixed (double* ptr = &matrix[i][0])
                        {
                            arrayofptr[i] = ptr;
                        }
                    }

                    fixed (double** ptrptr = &arrayofptr[0])
                    {
                        double** result = _reduceBasisLLL(ptrptr, n, alpha);

                        for (int i = 0; i < n; i++)
                        {
                            m[i] = new double[n];

                            for (int j = 0; j < n; j++)
                            {
                                m[i][j] = result[i][j];
                            }
                        }

                        ReleaseMemoryDoublePtrPtr(result, n);
                    }
                }
            }

            return m;
        }

        [DllImport(CPPDLLPath, EntryPoint = "CPPMathLibrary_ManagedPort_Simultaneous_SameDivisor", CallingConvention = CallingConvention.Cdecl)]
        private extern unsafe static int** _simultaneous_SameDivisor(double* x, int n, double alpha, double epsilon);

        public static int[][] SimultaneousSameDivisor(double[] x, int n, double alpha, double epsilon)
        {
            int[][] result = new int[n + 1][];

            unsafe
            {
                fixed(double* arr = new double[n])
                {
                    for(int i = 0; i < n; i++)
                    {
                        arr[i] = x[i];
                    }

                    int n1 = n + 1;
                    int** sd = _simultaneous_SameDivisor(arr, n, alpha, epsilon);
                    for(int i = 0; i < n1; i++)
                    {
                        result[i] = new int[n1];
                        for(int j = 0; j < n1; j++)
                        {
                            result[i][j] = sd[i][j];
                        }
                    }

                    ReleaseMemoryIntPtrPtr(sd, n1);
                }
            }

            return result;
        }
    }
}
