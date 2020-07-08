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
            int count = 10;
            int[] result = CPPMathLibTest.ContinuedFraction(num, ref count);
            CPPMathLibTest.PrintConvergents(result, count);
            Console.WriteLine("Finished");
            Console.ReadLine();
        }
    }
}
