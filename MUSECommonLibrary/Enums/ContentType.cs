using System.ComponentModel;

namespace MUSECommonLibrary
{
    public enum ContentType
    {
        [Description("application/xml")]
        XML = 0,
        [Description("application/json")]
        JSON = 1
    }
}
