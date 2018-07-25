using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Data.SqlClient;
using System.IO;
using System.Data;
using System.Text;
using System.Collections;

namespace SkillScriptExport
{
    public class Program
    {
        private string rootPath; // ��Ŀ¼
        private SqlConnection conn; // sql����
        private List<string> exportedFiles = new List<string>(); // Ҫ�������ļ�������
        private Hashtable fileTable = new Hashtable(); // �����ļ����������ݵ�hash��

        /// <summary>
        /// ���캯��
        /// </summary>
        /// <param name="conn">sql����</param>
        /// <param name="rootPath">��Ŀ¼</param>
        public Program(SqlConnection conn, string rootPath)
        {
            this.conn = conn;
            this.rootPath = rootPath;
        }

        /// <summary>
        /// �������ݱ�
        /// </summary>
        /// <returns></returns>
        public bool ExportScript()
        {
            bool success = true; // �Ƿ��Ѿ������ɹ�

            FolderBrowserDialog openFolderDialog = new FolderBrowserDialog();
            openFolderDialog.Description = "��ѡ�񵼳��ļ��ı���Ŀ¼";
            openFolderDialog.ShowNewFolderButton = true;
            if(rootPath == null)
            {
                openFolderDialog.RootFolder = Environment.SpecialFolder.MyComputer;
            }
            else
            {
                openFolderDialog.SelectedPath = rootPath;
            }

            DataTable dtExportTbl = GetDataTable("SELECT filename FROM sys_export_table_cfg WHERE tablename = 'tbl_skill_script'");
            string strRelaPath = dtExportTbl.Rows[0][0].ToString();
            rootPath += strRelaPath;
            //rootPath = openFolderDialog.SelectedPath;
            try
            {
                if (conn.State == ConnectionState.Closed) // ��sql����
                {
                    conn.Open();
                }

                string sqlString = string.Format("SELECT * FROM {0}", "tbl_skill_script");
                DataTable skillTable = GetDataTable(sqlString);
                sqlString = string.Format("SELECT * FROM {0}", "tbl_skill_AddAttribute");
                DataTable attributeTable = GetDataTable(sqlString);

                foreach (DataRow r in skillTable.Rows)
                {
                    string id = r["id"].ToString(); // ���ܵ�id
                    List<string> dataList = new List<string>(); // ��������������
                    string fileName = Path.Combine(r["foldername"].ToString(), r["filename"].ToString());
                    string fullPath = Path.Combine(rootPath, fileName);

                    // ���ͷ�ļ�
                    dataList.Add("Include(\"Scripts/Include/Skill.lh\")");
                    dataList.Add("Include(\"Scripts/Include/Player.lh\")");
                    dataList.Add("");

                    // ���tskilldata��
                    dataList.Add("tSkillData =");
                    dataList.Add("{");
                    dataList.Add("\t" + r["tskilldata"].ToString().Replace("\r\n", "\r\n\t"));
                    dataList.Add("};");
                    dataList.Add("");

                    // ���GetSkillLevelData����
                    dataList.Add("function GetSkillLevelData(skill)");
                    dataList.Add("    local bRetCode = false");
                    dataList.Add("    local dwSkillLevel = skill.dwLevel");
                    dataList.Add("");

                    // ���AddAttribute����
                    DataRow[] attributes = attributeTable.Select(string.Format("id = '{0}'", id));
                    foreach (DataRow row in attributes)
                    {
                        dataList.Add("    skill.AddAttribute(");
                        dataList.Add("        " + row["parm1"].ToString() + ",");
                        dataList.Add("        " + row["parm2"].ToString() + ",");
                        dataList.Add("        " + row["parm3"].ToString() + ",");
                        dataList.Add("        " + row["parm4"].ToString());
                        dataList.Add("    );");
                        dataList.Add("");
                    }

                    // ���CostMana
                    dataList.Add("    skill.nCostMana = " + r["nCostMana"].ToString());

                    // ���LevelUpExp
                    dataList.Add("    skill.dwLevelUpExp = " + r["dwLevelUpExp"].ToString());

                    // ���dwBaseKungfuExpAdd
                    dataList.Add("    skill.dwBaseKungfuExpAdd = " + r["dwBaseKungfuExpAdd"].ToString());

                    // ���nMinRadius
                    dataList.Add("    skill.nMinRadius = " + r["nMinRadius"].ToString() + " * LENGTH_BASE");

                    // ���nMaxRadius
                    dataList.Add("    skill.nMaxRadius = " + r["nMaxRadius"].ToString() + " * LENGTH_BASE");

                    // ���nAngleRange
                    dataList.Add("    skill.nAngleRange = " + r["nAngleRange"].ToString());

                    // ���nPrepareFrames
                    dataList.Add("    skill.nPrepareFrames = " + r["nPrepareFrames"].ToString());

                    // ���nBulletVelocity
                    dataList.Add("    skill.nBulletVelocity = " + r["nBulletVelocity"].ToString());

                    // ���nBreakRate
                    dataList.Add("    skill.nBreakRate = " + r["nBreakRate"].ToString());

                    // ���CoolDown
                    for (int i = 1; i <= 4; i++)
                    {
                        object o1 = r["SetCoolDown" + i + "_1"];
                        object o2 = r["SetCoolDown" + i + "_2"];
                        if ((o1 != null) && (o2 != null) && (o1.ToString() != "") && (o2.ToString() != ""))
                        {
                            dataList.Add(string.Format("    skill.SetCoolDown({0}, {1})", o1, o2));
                        }
                    }
                    dataList.Add("");

                    dataList.Add("    return true");
                    dataList.Add("end");
                    dataList.Add("");

                    // ���CanCast����
                    dataList.Add("function CanCast(player, nPreResult)");
                    dataList.Add("    return nPreResult");
                    dataList.Add("end");
                    dataList.Add("");

                    // ���CanLearnSkill����
                    dataList.Add("function CanLearnSkill(skill, player)");
                    dataList.Add("    return true");
                    dataList.Add("end");
                    dataList.Add("");

                    // ���OnSkillLevelUp����
                    dataList.Add("function OnSkillLevelUp(skill, player)");
                    dataList.Add("end");

                    fileTable[fullPath] = dataList;
                }
            }
            catch (SqlException ex)
            {
                MessageBox.Show("�ڶ�ȡ���ܱ�ű�����ʱ����sql�쳣: " + ex.ToString());
                success = false;
            }
            finally
            {
                if (conn.State == ConnectionState.Open) // �ر�sql����
                {
                    conn.Close();
                }
            }


            List<string> fileNameList = new List<string>();
            foreach (string s in fileTable.Keys)
            {
                fileNameList.Add(s);
            }

            // ѡ��Ҫ�������ļ�
            ChooseFileForm cForm = new ChooseFileForm(fileNameList);
            DialogResult result = cForm.ShowDialog();
            if (result != DialogResult.OK)
            {
                return false;
            }

            // �����ļ�
            foreach (string s in fileTable.Keys)
            {
                if (fileNameList.Contains(s))
                {
                    List<string> lines = fileTable[s] as List<string>;
                    SaveData(s, lines);
                }
            }

            // ��ʾ�����ļ��б�
            string message = "�����ļ�:\n\n";
            foreach (string s in fileNameList)
            {
                message = message + string.Format("{0}\n", s);
            }
            message = message + "\n\n";
            message = message + "�Ѿ��ɹ�����!";
            MessageBox.Show(message);

            return success;
        }

        /// <summary>
        /// ��ȡ���ݱ�
        /// </summary>
        /// <param name="sql">sql���</param>
        /// <returns>���ݱ�</returns>
        private DataTable GetDataTable(string sql)
        {
            SqlDataAdapter adp = new SqlDataAdapter(sql, conn);
            adp.MissingSchemaAction = MissingSchemaAction.AddWithKey;
            DataSet ds = new DataSet();
            adp.Fill(ds);
            System.Data.DataTable tbl = ds.Tables[0];
            return tbl;
        }

        /// <summary>
        /// ����Ĭ��������
        /// </summary>
        /// <param name="fileName">�ļ���</param>
        /// <param name="values">��������</param>
        private void SaveData(string fileName, List<string> values)
        {
            try
            {
                FileInfo fi = new FileInfo(fileName);
                StreamWriter sw;

                if (!fi.Exists) // �ļ�������
                {
                    MessageBox.Show(string.Format("�ļ�{0}������,���½����ļ�!", fileName));
                    string path = Path.GetDirectoryName(fileName); // ��ȡĿ¼
                    if (!Directory.Exists(path)) // Ŀ¼������
                    {
                        Directory.CreateDirectory(path); // �ȴ���Ŀ¼���ٴ����ļ�
                    }
                    FileStream fs = fi.Create();
                    fs.Close();
                    // sw = fi.CreateText(); // �½��ı�
                    sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                }
                else
                {
                    if (!exportedFiles.Contains(fileName)) // ��һ��д�ļ�
                    {
                        // sw = fi.CreateText();
                        sw = new StreamWriter(fileName, false, Encoding.GetEncoding("gb2312"));
                        exportedFiles.Add(fileName);
                    }
                    else // �������
                    {
                        // sw = fi.AppendText(); // ����ı�
                        sw = new StreamWriter(fileName, true, Encoding.GetEncoding("gb2312"));
                    }
                }

                foreach (string s in values)
                {
                    sw.WriteLine(s);
                }
                sw.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(string.Format("���ļ�{0}����IO����ʱ�����쳣: " + ex.ToString(), fileName));
            }
        }
    }
}