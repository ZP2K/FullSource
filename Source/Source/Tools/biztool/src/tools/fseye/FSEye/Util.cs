//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2007
//
//      Created_datetime : 2007
//      File_base        : Util
//      File_ext         : cs
//      Author           : ������
//      Description      : FSEye������
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

namespace FSEye
{
	/// <summary>
	/// ������
	/// </summary>
	/// <author>������</author>
	public class Util
	{
		/// <summary>
		/// ��������־
		/// </summary>
		/// <param name="msg">��־��Ϣ</param>
		public static void DebugLog(string msg)
		{
			if (SystemConfig.Current.LogEnabled)
			{
				string logDir = SystemConfig.Current.LogDir;
				if (logDir != null)
				{
					using (StreamWriter writer = new StreamWriter(logDir + "/log.txt", true))
					{
						writer.WriteLine(DateTime.Now.ToString() + ": " + msg);
						writer.Flush();
                        //writer.Dispose();
					}
				}
			}
		}

		/// <summary>
		/// ��byte����ת��Ϊ16�����ַ���������ʽ
		/// </summary>
		public static string ConvertBytesToHexString(byte[] data)
		{
			if (data == null || data.Length == 0)
				return string.Empty;

			StringBuilder sb = new StringBuilder();
			sb.Append("0x");
			for (int i = 0; i < data.Length; i++)
			{
				sb.AppendFormat("{0:X2}", data[i]);
			}

			return sb.ToString();
		}

		/// <summary>
		/// ȥ���ַ�����β
		/// </summary>
		/// <remarks>ȥ���ַ��������0</remarks>
		public static void TrimStringEnd(ref byte[] buff)
		{
			if (buff == null || buff.Length == 0)
				return;

			int endIndex = 0;
			for (int i = 0; i < buff.Length; i++)
			{
				if (buff[i] == 0)
				{
					endIndex = i;
					break;
				}
			}
			Array.Resize<byte>(ref buff, endIndex);
		}

		/// <summary>
		/// �ѡ���-ֵ�����ַ���ת��Ϊ��ϣ��
		/// </summary>
		/// <remarks>�ַ�����ʽΪ��key1=value1\nkey2=value2\nkey3=value3��</remarks>
		public static Hashtable ConvertKeyValuePairToHashtable(string keyValuePairString)
		{
			if (keyValuePairString == null)
				throw new ArgumentNullException("keyValuePairString");

			Hashtable hashTable = new Hashtable();

			string[] pairs = keyValuePairString.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
			if (pairs != null || pairs.Length > 0)
			{
				foreach (string pair in pairs)
				{
					int splitPos = pair.IndexOf('=');
					string key = pair.Substring(0, splitPos);
					string value = pair.Remove(0, splitPos + 1);
					hashTable.Add(key, value);
				}
			}

			return hashTable;
		}

		/// <summary>
		/// ����X����Y
		/// </summary>
		/// <remarks>��֤�����㷨</remarks>
		public static int CaculateY(int x)
		{
			return 101 * x * x + 43 * x + 72;
		}
	}
}
