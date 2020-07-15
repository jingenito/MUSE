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
            int n = 4;

            double[][] matrix = InitDoubleArr(n);
            matrix[0][0] = -2;
            matrix[0][1] = 7;
            matrix[0][2] = 7;
            matrix[0][3] = -5;
            matrix[1][0] = 3;
            matrix[1][1] = -2;
            matrix[1][2] = 6;
            matrix[1][3] = -1;
            matrix[2][0] = 2;
            matrix[2][1] = -8;
            matrix[2][2] = -9;
            matrix[2][3] = -7;
            matrix[3][0] = 8;
            matrix[3][1] = -9;
            matrix[3][2] = 6;
            matrix[3][3] = -4;

            Console.WriteLine("Initial Array:");
            PrintArrArr(matrix);

            double[][] result = CPPMathLibTest.ReduceBasisLLL(matrix, n, 0.75);
            Console.WriteLine("LLL Output:");
            PrintArrArr(result, true);

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
