//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2007-6-12
//      File_base        : ShellScriptManager
//      File_ext         : cs
//      Author           : ������
//      Description      : Shell�ű�������
//
//////////////////////////////////////////////////////////////////////
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace FSEye
{
	/// <summary>
	/// Shell�ű�������
	/// </summary>
	public class ShellScriptManager
	{		
		static ShellScriptManager g_theInstance = new ShellScriptManager();

		public static ShellScriptManager TheInstance
		{
			get { return g_theInstance; }
			set { g_theInstance = value; }
		}

		string _scriptRootPath = string.Empty;

		/// <summary>
		/// �ű���·��
		/// </summary>
		public string ScriptRootPath
		{
			get { return _scriptRootPath; }
			set { _scriptRootPath = value; }
		}

		/// <summary>
		/// ��ýű�����
		/// </summary>
		/// <param name="scriptName">�ű���</param>
		public string GetScript(string scriptName,string osType)
		{
            TextReader reader;
            string readString="";
            if (osType == null || osType.Length == 0)
            {
                reader = new StreamReader(_scriptRootPath + "windows" + GameServer.FilePathWinDelimiter + scriptName);
                readString = reader.ReadToEnd();
            }
            else
            {
                if (osType.ToLower().IndexOf("win") == -1)
                {
                    reader = new StreamReader(_scriptRootPath + "linux" + GameServer.FilePathWinDelimiter + scriptName);
                    readString = reader.ReadToEnd();
                    readString = readString.Replace("\r\n", "\n");
                }
                else
                {
                    reader = new StreamReader(_scriptRootPath + "windows" + GameServer.FilePathWinDelimiter + scriptName);
                    readString = reader.ReadToEnd();
                }

            }           
            reader.Dispose();
            return readString;
		}

		/// <summary>
		/// ��ýű�����
		/// </summary>
		/// <remarks>������</remarks>
		/// <param name="scriptName">�ű���</param>
		/// <param name="args">����</param>
		public string GetScript(string scriptName,string osType, params object[] args)
		{
            return FormatScriptParam(GetScript(scriptName, osType), args);
		}

		/// <summary>
		/// ��ʽ���ű�����
		/// </summary>
		/// <param name="scriptText">ԭʼ�ű�</param>
		/// <param name="args">����</param>
		/// <returns>��ʽ����Ľű�</returns>
		public static string FormatScriptParam(string scriptText, params object[] args)
		{
			if (scriptText == null || scriptText.Length == 0 || args == null || args.Length == 0)
				return scriptText;

			string returnText = scriptText;
			if (args != null && args.Length > 0)
			{
				for (int i = 0; i < args.Length; i++)
				{
					returnText = returnText.Replace(string.Format("C#P{0}", (i + 1)), args[i].ToString());
				}
			}

			return returnText;
		}
	}
}
