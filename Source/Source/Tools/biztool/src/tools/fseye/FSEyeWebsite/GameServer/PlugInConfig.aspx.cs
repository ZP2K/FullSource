//////////////////////////////////////////////////////////////////////
//
//      Kingsoft Blaze Game Studio. Copyright (C) 2006
//
//      Created_datetime : 2007-4
//      File_base        : GameServer
//      File_ext         : cs
//      Author           : Ҧң
//      Description      : ��Ϸ������--��Ϸ�������������ҳ��
//
//////////////////////////////////////////////////////////////////////

using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using FSEye.PlugIn;
using FSEye;
using Resources;

public partial class GameServer_PlugInConfig : BasePage
{    
    public GameServer SelectedMinIndexGameServer
    {
        get
        {
            if (GameServerCheckBoxList.SelectedGameServers != null && GameServerCheckBoxList.SelectedGameServers.Length != 0)
            {
                return GameServerCheckBoxList.SelectedGameServers[0];
            }
            return null;
        }
    }

    protected void Page_Load(object sender, EventArgs e)
    {        
        ListPlugIns();        
    }

    private void ListPlugIns()
    {
        GameServer server = this.SelectedMinIndexGameServer;            
        foreach (IPlugIn plugIn in ServerPlugInFactory.TotalPlugIn)
        {
            TableRow row = new TableRow();
            TableCell cell1 = new TableCell();
            cell1.Width = 100;
            CheckBox check = new CheckBox();
            check.Text = plugIn.Name;
            //if (server != null)
            //{
            //    foreach (IPlugIn plugInTemp in server.InstalledPlugInList)
            //    {
            //        if (plugIn.Name.Equals(plugInTemp.Name))
            //        {
            //            check.Checked = true;
            //            break;
            //        }
            //    }
            //}
            cell1.Controls.Add(check);
            row.Cells.Add(cell1);
            for (int count = 0; count != (int)plugIn.Parameters[0]; ++count)
            {
                TableCell tempCellDes = new TableCell();
                tempCellDes.HorizontalAlign = HorizontalAlign.Right;
                Label des = new Label();
                des.Width = 120;
                des.Text = plugIn.ParaDescription[count]+":";
                tempCellDes.Controls.Add(des);
                row.Cells.Add(tempCellDes);

                TableCell tempCell = new TableCell();
                tempCell.HorizontalAlign = HorizontalAlign.Left;
                TextBox text = new TextBox();
                text.Width = 50;
                text.Text = plugIn.Parameters[count + 1].ToString();                
                tempCell.Controls.Add(text);
                row.Cells.Add(tempCell);
            }
            TablePlugIns.Rows.Add(row);
        }
    }

    protected void ButtonSelectAllPlugIns_Click(object sender, EventArgs e)
    {
        SetAllPlugIns(true);
    }

    protected void ButtonSelectNoPlugIns_Click(object sender, EventArgs e)
    {
        SetAllPlugIns(false);
    }

    private void SetAllPlugIns(bool select)
    {
        for (int rowIndex = 0; rowIndex != TablePlugIns.Rows.Count; ++rowIndex)
        {
            CheckBox tempCheck = TablePlugIns.Rows[rowIndex].Cells[0].Controls[0] as CheckBox;
            if (tempCheck != null) tempCheck.Checked = select;
        }
    }

    protected void UpdatePlugIn_Click(object sender, EventArgs e)
    {
        try
        {
            //�ж�MessageLog���,������ͬһ�ļ�Ϊ���������������־�ļ�
            if (GameServerCheckBoxList.SelectedGameServers.Length != 1)
            {
                CheckBox mesLogCheckBox = TablePlugIns.Rows[(int)ServerPlugInFactory.PlugInList.MessageLog].Cells[0].Controls[0] as CheckBox;
                if(mesLogCheckBox.Checked)throw new Exception(StringDef.FileReusedError);
            }
            foreach (GameServer server in GameServerCheckBoxList.SelectedGameServers)
            {
                ArrayList PlugInsToInsert = new ArrayList();
                ArrayList PlugInsToDelete = new ArrayList();
                ArrayList PlugInsToEdit = new ArrayList();

                //�ж�ҪEdit��,ҪDelete��,����ҪAdd�Ĳ��
                for (int index = 0; index != TablePlugIns.Rows.Count; ++index)
                {
                    CheckBox tempCheck = TablePlugIns.Rows[index].Cells[0].Controls[0] as CheckBox;
                    if (tempCheck == null) return;

                    bool isEdit = false;
                    bool isAdd = true;
                    foreach (IPlugIn oldPlug in server.InstalledPlugInList)
                    {
                        if (tempCheck.Text.Equals(oldPlug.Name))
                        {
                            //���ñ��λ
                            isAdd = false;
                            //Edit
                            if (tempCheck.Checked)
                            {
                                //IPlugIn newPlug = ServerPlugInFactory.GetDefaultPlugIn(index);
                                //object[] data = new object[(int)newPlug.Parameters[0] + 1];
                                //data[0] = newPlug.Parameters[0];
                                for (int j = 0; j != oldPlug.Parameters.Length - 1; ++j)
                                {
                                    TextBox temp = TablePlugIns.Rows[index].Cells[2 + j * 2].Controls[0] as TextBox;
                                    if (temp != null)
                                    {
                                        //�ж������޸�
                                        //data[j + 1] = temp.Text;
                                        if (temp.Text.Equals(oldPlug.Parameters[j + 1].ToString())) continue;
                                        //���²���
                                        object[] parameters = oldPlug.Parameters;
                                        parameters[j + 1] = temp.Text;
                                        oldPlug.Parameters = parameters;
                                        isEdit = true;
                                    }
                                }
                                if (!isEdit) break;
                                //����һ���޸�2007-4-11�޸ĵĻ�,�����������,�����ٽ���ж���Ͱ�װ����,ֻ����һ�����ݿ�Ϳ�����
                                //newPlug.Parameters = data;
                                //oldPlug.Parameters = data;
                                //ж���ɲ��
                                //server.UnInstallPlugIn(oldPlug);
                                PlugInsToEdit.Add(oldPlug);
                                break;
                            }
                            //Delete
                            else
                            {
                                PlugInsToDelete.Add(oldPlug);
                                break;
                            }
                        }
                    }
                    //Add
                    //��һ��������,������Ĭ�Ϲ��캯����������޸Ĳ�����
                    //���ֱ���ô��ι���,��������֪
                    if (isAdd && tempCheck.Checked)
                    {
                        IPlugIn newPlug = ServerPlugInFactory.GetDefaultPlugIn(index);
                        //object[] data = new object[(int)newPlug.Parameters[0] + 1];
                        //data[0] = newPlug.Parameters[0];
                        for (int k = 0; k != newPlug.Parameters.Length - 1; ++k)
                        {
                            TextBox temp = TablePlugIns.Rows[index].Cells[2 + 2 * k].Controls[0] as TextBox;
                            if (temp != null)
                            {
                                //�ж������޸�
                                //newPlug.Parameters[k + 1] = temp.Text;
                                object[] parameters = newPlug.Parameters;
                                parameters[k + 1] = temp.Text;
                                newPlug.Parameters = parameters;
                            }
                        }
                        PlugInsToInsert.Add(newPlug);
                    }
                }
                if (PlugInsToInsert.Count != 0)
                    AdminServer.TheInstance.GameServerManager.InsertPlugIn(server, PlugInsToInsert);
                if (PlugInsToDelete.Count != 0)
                    AdminServer.TheInstance.GameServerManager.DeletePlugIn(server, PlugInsToDelete);
                if (PlugInsToEdit.Count != 0)
                    AdminServer.TheInstance.GameServerManager.EditPlugIn(server, PlugInsToEdit);

				server.Disconnect();
            }

            ShowMessage(StringDef.OperationSucceed, MessageType.Success);
		}
        catch (Exception ex)
        {
            if (ex.Message.Equals(StringDef.FileReusedError))
            {
                serverText.Text = StringDef.OperationFail+StringDef.Colon+StringDef.FileReusedError;
                //ShowMessage(StringDef.OperationFail + StringDef.Colon + FileReusedError, MessageType.Failure);
                return;
            }
        }
        LoadState();        
    }

    private void LoadState()
    {
        GameServer server = this.SelectedMinIndexGameServer;        
        if (server != null)
        {
            serverText.Text = String.Format(StringDef.ServerPlugInInfo, server.Name);    
            for (int i = 0; i != TablePlugIns.Rows.Count; ++i)
            {
                CheckBox check = TablePlugIns.Rows[i].Cells[0].Controls[0] as CheckBox;
                check.Checked = false;
                IPlugIn defaultPlug = ServerPlugInFactory.TotalPlugIn[i] as IPlugIn;
                foreach (IPlugIn plugIn in server.InstalledPlugInList)
                {
                    if (check.Text.Equals(plugIn.Name))
                    {
                        check.Checked = true;
                        break;
                    }
                }
                for (int index = 0; index != defaultPlug.Parameters.Length - 1; ++index)
                {
                    TextBox text = TablePlugIns.Rows[i].Cells[2 + 2 * index].Controls[0] as TextBox;                    
                    text.Text = defaultPlug.Parameters[index + 1].ToString();
                    foreach (IPlugIn plugIn in server.InstalledPlugInList)
                    {
                        if (check.Text.Equals(plugIn.Name))
                        {
                            text.Text = plugIn.Parameters[index + 1].ToString();
                            break;
                        }
                    }
                }
            }
        }
        else serverText.Text = StringDef.SelectNoGameServer;
    }

    protected void Refresh(object sender, EventArgs e)
    {
        LoadState();
    }
}
