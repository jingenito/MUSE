using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;

namespace TestAPP
{
    class Program
    {
        static void Main(string[] args)
        {
            double[] x = new double[4];
            x[0] = 239.0 / 169;
            x[1] = 265.0 / 153;
            x[2] = 682.0 / 305;
            x[3] = 590.0 / 223;

            Console.WriteLine("Inital Vector:");
            PrintArr(x);

            int[][] result = CPPMathLibTest.SimultaneousSameDivisor(x, 4, 0.75, 1.0 / 10);
            Console.WriteLine("Same Divisor Matrix:");
            PrintArrArr(result);

            Console.ReadLine();
        }

        private static double[][] InitDoubleArr(int n, double value = 0)
        {
            double[][] arr = new double[n][];
            for (int i = 0; i < n; i++)
            {
                arr[i] = new double[n];
                for (int j = 0; j < n; j++)
                {
                    arr[i][j] = value;
                }
            }
            return arr;
        }

        private static void PrintArr<T>(T[] arr)
        {
            var s = new StringBuilder();
            for (int i = 0; i < arr.Length; i++)
            {
                s.AppendFormat("{0}\t", arr[i].ToString());
            }
            Console.WriteLine(s.ToString());
        }

        private static void PrintArrArr<T>(T[][] arr, bool isnumeric = false)
        {
            var s = new StringBuilder();
            for (int i = 0; i < arr.Length; i++)
            {
                for (int j = 0; j < arr[i].Length; j++)
                {
                    if (isnumeric)
                    {
                        try
                        {
                            var t = string.Format("{0}   ", Convert.ToDouble(arr[i][j]).ToString("F5"));
                            s.Append(t);
                        }
                        catch (Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                            return;
                        }
                    }
                    else
                    {
                        s.AppendFormat("{0}\t", arr[i][j].ToString());
                    }
                }
                s.AppendLine(string.Empty);
            }
            Console.WriteLine(s.ToString());
        }
    }
}
