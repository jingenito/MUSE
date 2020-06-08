using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MUSECommonLibrary
{
    public class ContinuedFractionResponse
    {
        public IEnumerable<int> qSequence { get; set; }
        public int Count { get; set; }
        public int OpCount { get; set; }
        public string ServerResponse { get; set; }
    }
}
