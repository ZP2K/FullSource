using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace GameLuaEditor
{    
    class NetworkManager
    {
        private static NetworkManager networkManager;
        private string hostAddress = Helper.ServerIP /*"127.0.0.1"*/;
        private int udpSendPort = 6001;
        private int udpReceivePort = 5432;
        private Socket udpSendSocket = null;
        private Socket udpReceiveSocket = null;
        private IPEndPoint serverEndPoint = null;
        private IPEndPoint clientEndPoint = null;
        private EndPoint senderRemote = new IPEndPoint(IPAddress.Any, 0);
        private int bufferSize = 4096;
        private Thread udpReceiveThread = null;        
        private List<string> receivedMessageList = new List<string>(); // �յ�����Ϣ���� 

        public delegate void ProcessServerUdpMessage(string message);
        private ProcessServerUdpMessage processServerUdpMessage;

        private NetworkManager()
        {

        }

        /// <summary>
        /// �Ƿ��Ѿ��յ���Ϣ
        /// </summary>
        public bool HasReceivedMessage
        {
            get
            {
                return receivedMessageList.Count > 0;
            }
        }

        /// <summary>
        /// �յ�����Ϣ����
        /// </summary>
        public List<string> ReceivedMessageList
        {
            get
            {
                List<string> list = new List<string>(receivedMessageList);
                return list;
            }
        }

        /// <summary>
        /// ������յ���UDP��Ϣ
        /// </summary>
        public ProcessServerUdpMessage CurrentProcessSerrverUdpMessage
        {
            set
            {
                processServerUdpMessage = value;
            }
        }

        /// <summary>
        /// ������ַ
        /// </summary>
        public string HostAddress
        {
            set
            {
                hostAddress = value;
            }

            get
            {
                return hostAddress;
            }
        }

        /// <summary>
        /// UDP���Ͷ˿�
        /// </summary>
        public int UdpSendPort
        {
            set
            {
                udpSendPort = value;
            }

            get
            {
                return udpSendPort;
            }
        }

        /// <summary>
        /// UDP���ն˿�
        /// </summary>
        public int UdpReceivePort
        {
            set
            {
                udpReceivePort = value;
            }

            get
            {
                return udpReceivePort;
            }
        }

        /// <summary>
        /// ��ȡ����������
        /// </summary>
        /// <returns>����������</returns>
        public static NetworkManager GetNetworkManager()
        {
            if (networkManager == null)
            {
                networkManager = new NetworkManager();
                networkManager.Init();
            }

            return networkManager;
        }        

        /// <summary>
        /// ��ʼ������
        /// </summary>
        private void Init()
        {
            InitUdpSocket();
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Uninit()
        {            
            StopReceiveUdpMessage();
            UnInitUdpSocket();
        }

        /// <summary>
        /// ��ս��յ�����Ϣ����
        /// </summary>
        public void ClearReceivedMessageList()
        {
            receivedMessageList.Clear();
        }

        /// <summary>
        /// ��ȡ��Ϣ������ָ������Ϣ
        /// </summary>
        /// <param name="message">��Ϣ����</param>
        /// <returns>ָ������Ϣ</returns>
        public string GetMessage(string message)
        {
            string returnMessage = null;
            int removeIndex = -1;

            for (int i = 0; i < receivedMessageList.Count; i++)
            {
                if (receivedMessageList[i] == message)
                {
                    removeIndex = i;
                    returnMessage = message;
                    break;
                }
            }

            if (removeIndex != -1)
            {
                receivedMessageList.RemoveAt(removeIndex);
            }

            return returnMessage;
        }

        /// <summary>
        /// ��ȡ��Ϣ��������ָ����Ϣͷ����Ϣ
        /// </summary>
        /// <param name="messageHead">��Ϣͷ</param>
        /// <returns>��Ϣ</returns>
        public string GetMessageForHead(string messageHead)
        {
            string message = null;
            int removeIndex = -1;

            for (int i = 0; i < receivedMessageList.Count; i++)
            {
                if (receivedMessageList[i].StartsWith(messageHead))
                {
                    removeIndex = i;
                    message = receivedMessageList[i];
                    break;
                }
            }

            if (removeIndex != -1)
            {
                receivedMessageList.RemoveAt(removeIndex);
            }                

            return message;
        }

        /// <summary>
        /// ��ʼ��UDP�׽���
        /// </summary>
        public void InitUdpSocket()
        {
            serverEndPoint = new IPEndPoint(IPAddress.Parse(hostAddress), udpSendPort);
            udpSendSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);

            clientEndPoint = new IPEndPoint(IPAddress.Parse(hostAddress), udpReceivePort);
            udpReceiveSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            udpReceiveSocket.Bind(clientEndPoint);
        }

        /// <summary>
        /// ����UDP�׽���
        /// </summary>
        public void UnInitUdpSocket()
        {
            if (udpSendSocket != null)
            {
                udpSendSocket.Close();
            }
            
            if (udpReceiveSocket != null)
            {
                udpReceiveSocket.Close();
            }            
        }

        /// <summary>
        /// ����UDP��Ϣ
        /// </summary>
        /// <param name="message">��Ϣ</param>
        public void SendUdpMessage(string message)
        {
            byte[] buffer = Encoding.GetEncoding("gb2312").GetBytes(message);
            udpSendSocket.SendTo(buffer, serverEndPoint);
        }

        /// <summary>
        /// ����UDP��Ϣ
        /// </summary>        
        public void BeginReceiveUdpMessage()
        {   
            if (udpReceiveThread == null)
            {
                udpReceiveThread = new Thread(new ThreadStart(ThreadReceiveUdpMessage));
                udpReceiveThread.Start();
            }
        }

        /// <summary>
        /// ֹͣ����UDP��Ϣ
        /// </summary>
        public void StopReceiveUdpMessage()
        {
            if (udpReceiveThread != null)
            {
                udpReceiveThread.Abort();
            }
        }

        /// <summary>
        /// �߳̽���UDP��Ϣ
        /// </summary>
        private void ThreadReceiveUdpMessage()
        {
            while (true)
            {
                byte[] buffer = new byte[bufferSize];
                int count = udpReceiveSocket.ReceiveFrom(buffer, ref senderRemote);
                string message = Encoding.GetEncoding("gb2312").GetString(buffer, 0, count);
                Console.WriteLine(message);
                receivedMessageList.Add(message);
            }
        }
    }
}
