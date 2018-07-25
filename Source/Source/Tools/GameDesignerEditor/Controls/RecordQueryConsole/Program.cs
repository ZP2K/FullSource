using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace RecordQueryConsole
{
    class Program
    {       
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            if (args.Length == 0)//ͼ�񴰿�
            {

                SelectDataBaseForm dbForm = new SelectDataBaseForm();
                if (dbForm.ShowDialog() != DialogResult.OK)
                {
                    return;
                }
                string ChoosedDBName = dbForm.ChoosedDBName;
                Application.Run(new DataRecordsQureyForm(ChoosedDBName));
            }
            else if (args.Length != 4)//����̨
            {
                string usage = "Usage:\r\nRecordQueryConsole.exe SqlConnString StartTime EndTime SaveFilePath\r\n" +
                "\tSqlConnString:\t���ݿ������ַ���������Ҫ���ʵģ����ݿ����ơ��û������롢ip\r\n" +
                "\tStartTime:\t��ʼʱ�䣬��ʽΪ 2009-04-06 16:26:00\r\n" +
                "\tEndTime:\t��ֹʱ�䣬��ʽͬ��\r\n" +
                "\tSaveFilePath:\t�����ļ���·������Ҫ����չ������ѡ��Ϊ��xlsx��xml\r\n\r\n" +
                "\t��ѡ�����ݿ����£�\r\n" +
                "\t\ts3design_rtm:\t\tHead��\r\n" +
                "\t\ts3design-1-0-8-1380:\t1380���ݿ�\r\n" +
                "\t\ts3design-1-0-8-1380-new:1380��\r\n" +
                "\t\tss3design_skill_branch:\t���ܷ�֧��\r\n" +
                "Example:\r\nRecordQueryConsole.exe \"Server = 192.168.27.31; DataBase = s3design_rtm; Uid = s3design; Password = davidbowie;\" \"2009-04-05 16:26:00\" \"2009-04-06 16:26:00\" D:\\DataRecordQueryResult.xml\r\n";

                Console.Write(usage);
            }
            else
            {
                string sqlConnStr;
                string StartTime;
                string EndTime;
                string FilePath;

                sqlConnStr = args[0];
                StartTime = args[1];
                EndTime = args[2];
                FilePath = args[3];

                if (!Helper.IsLegalDataTimeString(StartTime))
                {
                    Console.WriteLine("StartTimeʱ���ʽ����");
                }
                else if (!Helper.IsLegalDataTimeString(EndTime))
                {
                    Console.WriteLine("EndTimeʱ���ʽ����");
                }
                else
                {
                    Console.Write("���ڲ�ѯ�͵��������Ե�....\r\n");
                    DataRecord.QueryDataRecord(sqlConnStr, StartTime, EndTime, FilePath);
                    Console.Write(DataRecord.TheMessage);
                }

            }
            
        }

       







    }
}
