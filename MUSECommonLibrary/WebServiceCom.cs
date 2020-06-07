using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;

using MUSECommonLibrary.Extensions;
using Newtonsoft.Json;

namespace MUSECommonLibrary
{
    public class WebServiceCom
    {
        private static Uri APIMapper(APIType api)
        {
            var uBuilder = new UriBuilder();
            uBuilder.Scheme = GlobalVariables.Scheme;
            uBuilder.Host = GlobalVariables.Host;
            if (GlobalVariables.UsePortNumber)
            {
                uBuilder.Port = GlobalVariables.Port;
            }

            var sBuilder = new StringBuilder();
            if (!string.IsNullOrWhiteSpace(GlobalVariables.AppPath))
            {
                sBuilder.AppendFormat("{0}/", GlobalVariables.AppPath);
            }
            sBuilder.AppendFormat("api/{0}", api.Description());

            uBuilder.Path = sBuilder.ToString();
            return uBuilder.Uri;
        }

        private static async Task<string> SubmitRequestToWebService(APIType api,
                                                    string uri,
                                                    WebMethod method,
                                                    ContentType contentType,
                                                    string data = "")
        {
            return await SubmitRequestToWebService(api, uri, method, contentType, contentType, data);
        }

        private static async Task<string> SubmitRequestToWebService(APIType api,
                                              string uri,
                                              WebMethod method,
                                              ContentType contentType,
                                              ContentType acceptType,
                                              string data = "")
        {
            var uBuilder = new UriBuilder(APIMapper(api));
            uBuilder.Path += uri;

            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12; //3072
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(uBuilder.Uri);

            request.Method = method.Description();
            request.Accept = acceptType.Description();
            request.ContentType = contentType.Description();
            if (method == WebMethod.POST && !string.IsNullOrWhiteSpace(data))
            {
                var edata = Encoding.ASCII.GetBytes(data);
                request.ContentLength = edata.Length;
                using (var stm = await request.GetRequestStreamAsync())
                {
                    await stm.WriteAsync(edata, 0, edata.Length);
                }
            }

            var response = "";
            try
            {
                using(var rsp = await request.GetResponseAsync())
                {
                    using(var rspStream = rsp.GetResponseStream())
                    {
                        using(var sReader = new StreamReader(rspStream))
                        {
                            response = await sReader.ReadToEndAsync();
                        }
                    }
                }
            }
            catch(Exception ex)
            {

            }

            return response;
        }

        public async Task<ContinuedFractionResponse> POSTContinuedFractionRequest(double num, int count)
        {
            return await POSTContinuedFractionRequest(new ContinuedFractionRequest() { Number = num, Count = count });
        }
        public async Task<ContinuedFractionResponse> POSTContinuedFractionRequest(ContinuedFractionRequest rqst)
        {
            var jsonRqstString = JsonConvert.SerializeObject(rqst);
            var jsonRespString = await SubmitRequestToWebService(APIType.Algorithms, "", WebMethod.POST, ContentType.JSON, jsonRqstString);
            try
            {
                return JsonConvert.DeserializeObject<ContinuedFractionResponse>(jsonRespString);
            }catch(Exception ex)
            {
                return null;
            }
        }
    }
}
