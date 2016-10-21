using hxTestTool;

public class Config : INIConfig
{
    public string rem_配置文件版本
        = "说明: \n"
        + "1. 删除该配置文件后应用程序会自动重新生成一个新的,并设置每个配置项为默认参数\n"
        + "2. 配置文件按行读取,每一行一个配置项\n"
        + "3. 符号\"#\"之后直到一行末尾为注释,对应用程序无任何影响.\n";
    public string 配置文件版本 = "0.1";
    public class Section_应用参数
    {
        public string rem_日志存储路径 = "设置日志的存储路径,使用绝对路径";
        public string 日志存储路径 = "";
    }
    public class Section_测试参数
    {
        public string rem_禁用测试单元 = "用逗号分割的序列,例如禁用1和2: 1,2";
        public string 禁用测试单元 = "2,3,4,5,6,7,8";

        public string rem_单元重测次数 = "一般 1~5 为好";
        public int 单元重测次数 = 2;

        //public string rem_电极电压峰峰值最小值 = "单位为V";
        //public int 电极电压峰峰值最小值 = 25;
        //public string rem_电极电压峰峰值最大值 = "单位为V";
        //public int 电极电压峰峰值最大值 = 50;

        //电极电压(/H/M/L)
        public string rem_H档位电极电压峰峰值最小值 = "单位为V";
        public int H档位电极电压峰峰值最小值 = 42;
        public string rem_H档位电极电压峰峰值最大值 = "单位为V";
        public int H档位电极电压峰峰值最大值 = 58;

        public string rem_M档位电极电压峰峰值最小值 = "单位为V";
        public int M档位电极电压峰峰值最小值 = 40;
        public string rem_M档位电极电压峰峰值最大值 = "单位为V";
        public int M档位电极电压峰峰值最大值 = 46;

        public string rem_L档位电极电压峰峰值最小值 = "单位为V";
        public int L档位电极电压峰峰值最小值 = 37;
        public string rem_L档位电极电压峰峰值最大值 = "单位为V";
        public int L档位电极电压峰峰值最大值 = 44;

        //波形周期(间隔3s/连续3s)
        public string rem_间隔模式间隔时间最小值 = "单位为mS";
        public int 间隔模式间隔时间最小值 = 2800;
        public string rem_间隔模式间隔时间最大值 = "单位为mS";
        public int 间隔模式间隔时间最大值 = 3200;

        public string rem_连续模式间隔时间最小值 = "单位为mS";
        public int 连续模式间隔时间最小值 = 800;
        public string rem_连续模式间隔时间最大值 = "单位为mS";
        public int 连续模式间隔时间最大值 = 1200;

        //脉冲频率(55HZ)
        public string rem_脉冲频率最小值 = "单位为Hz";
        public int 脉冲频率最小值 = 52;
        public string rem_脉冲频率最大值 = "单位为Hz";
        public int 脉冲频率最大值 = 58;

        //------------------------------------------------------
        //arguments
        private string rem_电极电压矫正值B = "计算方式 \"(Va+Vb) / 2 * K + B\"";
        public double 电极电压矫正值B = 28f;
        private string rem_电极电压矫正值K = "计算方式 \"(Va+Vb) / 2 * K + B\"";
        public double 电极电压矫正值K = 0.5f;

        //---------------------------------------------------
        //power off voltage 
        public string rem_关机电极对地电压最小值 = "单位为V";
        public int 关机电极对地电压最小值 = 0;
        public string rem_关机电极对地电压最大值 = "单位为V";
        public int 关机电极对地电压最大值 = 3;
    }
    public string rem_应用参数
        = "======================================================\n"
        + "应用程序设定参数.";
    public Section_应用参数 应用参数 = new Section_应用参数();
    public string rem_测试参数
        = "======================================================\n"
        + "测试单元设定参数.";
    public Section_测试参数 测试参数 = new Section_测试参数();
}