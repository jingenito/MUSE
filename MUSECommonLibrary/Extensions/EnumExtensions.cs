using System;
using System.ComponentModel;
using System.Reflection;
using System.Diagnostics;

namespace MUSECommonLibrary.Extensions
{
    public static class EnumExtensions
    {
        [DebuggerStepThrough]
        public static string Description(this Enum e)
        {
            FieldInfo fi = e.GetType().GetField(e.ToString());

            DescriptionAttribute[] attributes = (DescriptionAttribute[])fi.GetCustomAttributes(typeof(DescriptionAttribute), false);
            if (attributes != null && attributes.Length > 0)
                return attributes[0].Description;
            else
                return e.ToString();
        }
    }
}
