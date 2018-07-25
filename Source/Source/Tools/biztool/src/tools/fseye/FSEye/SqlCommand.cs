//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-9-13
//      File_base        : SqlCommand
//      File_ext         : cs
//      Author           : ������
//      Description      : SQL������
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace FSEye
{
	#region SqlCommand
	/// <summary>
	/// SQL����
	/// </summary>
	/// <author>������</author>
	public class SqlCommand : IDisposable
	{
		//����ռλ��
		public const string ParameterPlaceholder = "?";

		//�ַ�������
		private static Encoding StringEncoding = Encoding.Default;

		//SQL���� 
        //2008-03 huangbiao �����������������,����ĳ�GB2312
		//private static Encoding SqlEncoding = Encoding.ASCII;
        private static Encoding SqlEncoding = SystemConfig.Current.DefaultEncoding;

		//ԭʼ�����ַ���
		private string _cmdText;

		//�����б�
		private object[] _paramList;

		//SQL������
		private MemoryStream _sqlBufferStream;

		/// <summary>
		/// ����
		/// </summary>
		/// <param name="cmd"></param>
		/// <param name="paramList"></param>
		/// <remarks>����string��byte[]���������ʹ��?��ԭ������滻��Ȼ����뵽�������У����������Զ�escape�ַ�����byte��</remarks>
		public SqlCommand(string cmd, params object[] paramList)
		{
			_cmdText = cmd;
			_paramList = paramList;
			_sqlBufferStream = new MemoryStream();

			Prepare();
		}

        public SqlCommand(string cmd)
        {
            _cmdText = cmd;           
            _sqlBufferStream = new MemoryStream();

            byte[] remainStringBytes = SqlEncoding.GetBytes(_cmdText);
            _sqlBufferStream.Write(remainStringBytes, 0, remainStringBytes.Length);
            _sqlBufferStream.Close();
        }

		~SqlCommand()
		{
			Dispose();
		}

		public void Dispose()
		{
			if (_sqlBufferStream != null)
			{
				_sqlBufferStream.Close();
			}
		}

		/// <summary>
		/// ���
		/// </summary>
		public string CommandText
		{
			get { return _cmdText; }
		}

		/// <summary>
		/// �����б�
		/// </summary>
		public object[] Params
		{
			get { return _paramList; }
		}

		/// <summary>
		/// SQL����
		/// </summary>
		public byte[] SqlBuffer
		{
			get { return _sqlBufferStream.ToArray(); }
		}

		/// <summary>
		/// ����ƴװSQL����
		/// </summary>
		private void Prepare()
		{
			int placeholderLength = ParameterPlaceholder.Length;
			int paramIndex = 0;
			int pos = 0;
			int lastPos = -placeholderLength;
            byte[] remainStringBytes;
            if (_paramList.Length > 0)
            {
                while ((pos = _cmdText.IndexOf(ParameterPlaceholder, pos + placeholderLength)) != -1)
                {
                    byte[] subStringBytes = SqlEncoding.GetBytes(_cmdText.Substring(lastPos + placeholderLength, pos - lastPos - placeholderLength));
                    _sqlBufferStream.Write(subStringBytes, 0, subStringBytes.Length);
                    byte[] paramBytes = GetBytes(_paramList[paramIndex], StringEncoding);
                    _sqlBufferStream.Write(paramBytes, 0, paramBytes.Length);
                    lastPos = pos;
                    paramIndex++;
                }
                remainStringBytes = SqlEncoding.GetBytes(_cmdText.Substring(lastPos + placeholderLength));
            }
            else
            {
                remainStringBytes = SqlEncoding.GetBytes(_cmdText);
            }
			_sqlBufferStream.Write(remainStringBytes, 0, remainStringBytes.Length);
			_sqlBufferStream.Close();
		}

		/// <summary>
		/// ���������л�Ϊ�Ϸ���SQL�ֽ�����
		/// </summary>
		static byte[] GetBytes(object param, Encoding encoding)
		{
			Type type = param.GetType();
			if (type == typeof(string))
			{
				byte[] stringBytes = encoding.GetBytes(param as string);
				return EscapeByteArray(stringBytes, stringBytes.Length);
			}
			else if (type == typeof(byte[]))
			{
				byte[] paramValue = param as byte[];
				return EscapeByteArray(paramValue, paramValue.Length);
			}

			return SqlEncoding.GetBytes(param.ToString());
		}

		/// <summary>
		/// ת���ֽ�����
		/// </summary>
		static byte[] EscapeByteArray(byte[] bytes, int length)
		{
			byte[] buffer = new byte[bytes.Length * 2];

			int y = 0;
			for (int x = 0; x < length; x++)
			{
				byte b = bytes[x];
				if (b == '\0')
				{
					buffer[y++] = (byte)'\\';
					buffer[y++] = (byte)'0';
				}
				else if (b == '\\' || b == '\'' || b == '\"')
				{
					buffer[y++] = (byte)'\\';
					buffer[y++] = b;
				}
				else
					buffer[y++] = b;
			}

			Array.Resize<byte>(ref buffer, y);

			return buffer;
		}

		public override string ToString()
		{
			return SqlEncoding.GetString(SqlBuffer);
		}

		/// <summary>
		/// �Ѷ���ת��ΪSQL֧�ֵĲ����ַ���
		/// </summary>
		/// <param name="value">ת������</param>
		/// <param name="encoding">���룬���ҽ���������string��ʱ��������</param>
		public static string ValueToSqlString(object value, Encoding encoding)
		{
			if (value == null)
				return "''";

			Type valueType = value.GetType();

			if (valueType == typeof(string))
			{
				string stringValue = value as String;
				if (stringValue.Length == 0)
					return "''";
				else
					return Util.ConvertBytesToHexString(encoding.GetBytes(stringValue));
			}
			else if (valueType == typeof(byte[]))
				return Util.ConvertBytesToHexString(value as byte[]);
			else if (valueType == typeof(DateTime))
				return "'" + (DateTime)value + "'";
			else
				return value.ToString();
		}
	}
	#endregion

	#region SqlDataType
	public enum SqlDataType
	{
		Int64,
		Int32,
		Int16,
		SByte,
		UInt64,
		UInt32,
		UInt16,
		Byte,
		Float,
		Double,
		String,
		DateTime,
		Blob
	}

	#endregion

	#region SqlResult
	/// <summary>
	/// SQL���
	/// </summary>
	public class SqlResult
	{
		public enum State
		{
			Querying,
			Done
		}

		Encoding _encoding;

		uint _id;
		
		bool _success;

		byte[] _data;

		SqlDataType[] _types;

		BinaryReader _reader;

		State _resultState;

		public uint SessionId
		{
			get { return _id; }
		}

		public bool Success
		{
			get { return _success; }
		}

		public State ResultState
		{
			get { return _resultState; }
			set { _resultState = value; }
		}

		public SqlResult(uint id) : this(id, SystemConfig.Current.DefaultEncoding) { }


		public SqlResult(uint id, Encoding encoding)
		{
			_id = id;
			_resultState = State.Querying;
			_encoding = encoding;
		}

		public void AppendData(byte[] data)
		{
			if (_data != null)
			{
				byte[] originalData = _data;
				_data = new byte[_data.Length + data.Length];
				Array.Copy(originalData, 0, _data, 0, originalData.Length);
				Array.Copy(data, 0, _data, originalData.Length, data.Length);
			}
			else
			{
				_data = new byte[data.Length];
				Array.Copy(data, _data, data.Length);
			}
		}

		public void Done(bool success)
		{
			_reader = new BinaryReader(new MemoryStream(_data));
			_success = success;
			_resultState = State.Done;
		}

		/// <summary>
		/// ���ø����������
		/// </summary>
		/// <param name="types">�����б�</param>
		public void SetFieldType(SqlDataType[] types)
		{
			_types = types;
		}

		/// <summary>
		/// ��ȡ��һ����¼
		/// </summary>
		public object[] ReadRecord()
		{
			if (_resultState != State.Done)
				throw new Exception("Result not ready for reading.");

			if (_types != null && _types.Length > 0)
			{
				object[] record = new object[_types.Length];
				for (int i = 0; i < _types.Length; i++)
				{	
					byte[] data = ReadData();
					if (data != null)//&&data.Length!=0)
					{
						SqlDataType type = _types[i];
						object value = null;

                        try
                        {
                            switch (type)
                            {
                                case SqlDataType.Int32:
                                    value = Int32.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.UInt32:
                                    value = UInt32.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.Int16:
                                    value = Int16.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.UInt16:
                                    value = UInt16.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.SByte:
                                    value = SByte.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.Byte:
                                    value = Byte.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.Int64:
                                    value = Int64.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.UInt64:
                                    value = UInt64.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.Float:
                                    value = float.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.Double:
                                    value = Double.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.String:
                                    value = _encoding.GetString(data);
                                    break;
                                case SqlDataType.DateTime:
                                    value = DateTime.Parse(_encoding.GetString(data));
                                    break;
                                case SqlDataType.Blob:
                                    value = data;
                                    break;
                                default:
                                    throw new Exception("Unknown data type \"" + type + "\"");
                            }

                        }
                        catch (Exception)
                        {
                            value = null;
                        }
						record[i] = value;
					}
					else
					{
						return null;
					}
				}
				return record;
			}
			else
			{
				return null;
			}
		}

		/// <summary>
		/// ��ȡ��һ������
		/// </summary>
		byte[] ReadData()
		{	
			//if (_reader.PeekChar() > -1)
			if (_reader.BaseStream.Position < _reader.BaseStream.Length)
			{
				ushort length = _reader.ReadUInt16();
				return _reader.ReadBytes(length);
			}
			else
			{
				return null;
			}
		}
	}
	#endregion
}
