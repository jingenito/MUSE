using System;
using System.Runtime.CompilerServices;

namespace TestAPP
{
    class Program
    {
        static void Main(string[] args)
        {
            double num = Math.Sqrt(2);

            Console.WriteLine("Approximating sqrt(2)");
            Console.WriteLine("Partial Quotients:");
            int[] result = CPPMathLibTest.ContinuedFraction(num, 10);
            for (int i = 0; i < result.Length; i++)
                Console.WriteLine(result[i]);
            Console.WriteLine("Finished");
            Console.ReadLine();
        }
    }
}
