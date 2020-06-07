using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;

using MUSECommonLibrary;
using MUSEWebService.Services;

namespace MUSEWebService.Controllers
{
    [Route("api/Algorithms")]
    [ApiController]
    public class AlgorithmController : ControllerBase
    {
        private IAlgorithmService _algorithmService;

        public AlgorithmController(IAlgorithmService algorithmService)
        {
            _algorithmService = algorithmService;
        }

        [HttpPost("ContinuedFraction")]
        public async Task<IActionResult> GetContinuedFractionResponse([FromBody]ContinuedFractionRequest rqst)
        {
            var contFracResp = await _algorithmService.GetContinuedFractionExpansion(rqst);

            if(contFracResp == null)
                return BadRequest(new { message = "Bad Request." });

            return Ok(contFracResp);
        }
    }
}