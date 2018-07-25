//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-9-14
//      File_base        : IPlugIn
//      File_ext         : cs
//      Author           : ������
//      Description      : ����ӿ�
//
//////////////////////////////////////////////////////////////////////

namespace FSEye
{
	/// <summary>
	/// ����ӿ�
	/// </summary>
	/// <author>������</author>
	public interface IPlugIn
	{
		/// <summary>
		/// �������
		/// </summary>
		string Name	{ get; }

		/// <summary>
		/// ����
		/// </summary>
		string Author { get; }

		/// <summary>
		/// GUID
		/// </summary>
		string Guid { get; }

        /// <summary>
        /// ����������
        /// </summary>
        string[] ParaDescription { get; }

        /// <summary>
        /// ����
        /// ע:��һ��object���ظ�PlugIn�Ĳ�������
        /// </summary>
        object[] Parameters { set; get; }

		/// <summary>
		/// ��ʼ��
		/// </summary>
		void Initialize(GameServer server);

		/// <summary>
		/// ����
		/// </summary>
		void Dispose();

		/// <summary>
		/// ִ�в����Ϊ
		/// </summary>
		/// <param name="actionName">��Ϊ��</param>
		/// <param name="args">����</param>
		bool DoAction(int userId,string actionKey, ref object outArg, params object[] args);
				
		/// <summary>
		/// �õ��������
		/// </summary>
		/// <param name="dataKey">���ݼ�</param>
		object GetData(int userId,string dataKey, params object[] args);
	}

	/// <summary>
	/// �������
	/// </summary>
	/// <author>������</author>
	public abstract class BasePlugIn : IPlugIn
	{
		protected GameServer _server;

		public abstract string Name { get; }

		public abstract string Author { get; }

		public abstract string Guid { get; }

		public abstract string[] ParaDescription { get; }

		public abstract object[] Parameters { get; set; }

		public virtual void Initialize(GameServer server)
		{
			_server = server;
		}

		public abstract void Dispose();

		public abstract bool DoAction(int userId,string actionKey, ref object outArg, params object[] args);

		public abstract object GetData(int userId,string dataKey, params object[] args);
	}
}
