//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2006-8
//      File_base        : IMessageHandler
//      File_ext         : cs
//      Author           : ������
//      Description      : ��Ϣ�����߽ӿ�
//
//////////////////////////////////////////////////////////////////////

namespace FSEye
{
	/// <summary>
	/// ��Ϣ�����߽ӿ�
	/// </summary>
	/// <author>������</author>
	public interface IMessageHandler
	{
		/// <summary>
		/// ������Ϣ
		/// </summary>
		/// <param name="message">��Ϣ</param>
		void ProcessMessage(GameServer server, IProtocol message);
	}
}
