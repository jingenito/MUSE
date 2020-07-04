using System;
using System.Runtime.CompilerServices;

namespace TestAPP
{
    class Program
    {
        static void Main(string[] args)
        {
            while (true)
            {
                int n1, n2;

                Console.WriteLine("Enter a number: ");
                var t1 = Console.ReadLine();
                if (!int.TryParse(t1, out n1))
                {
                    Console.WriteLine("Invalid number");
                    Console.ReadLine();
                    return;
                }

                Console.WriteLine("Enter another number: ");
                var t2 = Console.ReadLine();
                if (!int.TryParse(t2, out n2))
                {
                    Console.WriteLine("Invalid number");
                    Console.ReadLine();
                    return;
                }

                var gcd = CPPMathLibTest.GetGCD(n1, n2);
                Console.WriteLine(string.Format("The GCD of {0} and {1} is {2}", n1, n2, gcd));
                Console.ReadLine();
            }
        }
    }
}
