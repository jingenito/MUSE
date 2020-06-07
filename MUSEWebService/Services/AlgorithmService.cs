using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Newtonsoft.Json;

using MUSECommonLibrary;

namespace MUSEWebService.Services
{
    public interface IAlgorithmService
    {
        Task<ContinuedFractionResponse> GetContinuedFractionExpansion(ContinuedFractionRequest rqst);
    }

    public class AlgorithmService : IAlgorithmService
    {
        private const string CONTINUEDFRACTIONEXEPATH = "";

        public async Task<ContinuedFractionResponse> GetContinuedFractionExpansion(ContinuedFractionRequest rqst)
        {
            if (string.IsNullOrWhiteSpace(CONTINUEDFRACTIONEXEPATH)) { throw new NotImplementedException(); }
            if(rqst.Number == 0 || rqst.Count == 0) { return null; }

            string filename = string.Format("{0}.json", Guid.NewGuid().ToString()); //cant use a static filename when the server is processing multiple requests
            var info = new ProcessStartInfo(CONTINUEDFRACTIONEXEPATH);
            info.CreateNoWindow = true;
            info.Arguments = string.Format("{0} {1} {2}", rqst.Number, rqst.Count, filename);
            var p = Process.Start(info);
            p.WaitForExit();
            
            using(var sr = new StreamReader(filename))
            {
                var jsonString = await sr.ReadToEndAsync();
                try
                {
                    return JsonConvert.DeserializeObject<ContinuedFractionResponse>(jsonString);

                }catch(Exception ex)
                {
                    return new ContinuedFractionResponse() { ServerResponse = ex.Message };
                }
            }
        }
    }
}
