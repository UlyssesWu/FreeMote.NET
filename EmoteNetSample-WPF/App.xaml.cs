using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Runtime.InteropServices;
using System.Windows;

namespace FreeMote.Tools.Viewer
{
    /// <summary>
    /// App.xaml 的交互逻辑
    /// </summary>
    public partial class App : Application
    {
        //[DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        //private static extern bool SetDllDirectory(string lpPathName);

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
        }
    }
}
