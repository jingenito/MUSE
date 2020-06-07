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
            if (string.IsNullOrWhiteSpace(CONTINUEDFRACTIONEXEPATH) || !File.Exists(CONTINUEDFRACTIONEXEPATH)) { throw new NotImplementedException(); }
            if (rqst.Number == 0 || rqst.Count == 0) { return new ContinuedFractionResponse() { ServerResponse = "Missing or invalid inputs." }; }

            string filename = string.Format("{0}.json", Guid.NewGuid().ToString()); //cant use a static filename when the server is processing multiple requests
            var info = new ProcessStartInfo(CONTINUEDFRACTIONEXEPATH);
            info.CreateNoWindow = true;
            info.Arguments = string.Format("{0} {1} {2}", rqst.Number, rqst.Count, filename);
            var p = Process.Start(info);
            p.WaitForExit();

            ContinuedFractionResponse resp = null;
            if (File.Exists(filename))
            {
                //the c++ program wont output anything if there were problems
                using (var sr = new StreamReader(filename))
                {
                    var jsonString = await sr.ReadToEndAsync();
                    try
                    {
                        resp = JsonConvert.DeserializeObject<ContinuedFractionResponse>(jsonString);

                    }
                    catch (Exception ex)
                    {
                        resp = new ContinuedFractionResponse() { ServerResponse = ex.Message };
                    }
                }

                //delete the file now to save room when processing multiple requests
                try
                {
                    File.Delete(filename);
                }
                catch(Exception ex)
                {

                }
            }
            else
            {
                resp = new ContinuedFractionResponse() { ServerResponse = "There was an issue processing the request." };
            }

            return resp;
        }
    }
}
