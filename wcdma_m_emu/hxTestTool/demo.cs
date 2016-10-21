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
            public string rem_禁用测试单元 = "用逗号分割的序列,例如禁用A1和B2: A1,B2";
            public string 禁用测试单元 = "";
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