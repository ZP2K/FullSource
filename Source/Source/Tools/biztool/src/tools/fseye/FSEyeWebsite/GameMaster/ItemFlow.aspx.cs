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
using System.Text;
using Resources;
using FSEye.FS2Lib;
using FSEye;
using FSEye.Security;

public partial class GameInfo_ItemFlow : BasePage
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (IsPostBack == false)
        {
            if ((Request.QueryString["itemGUID"] != null) && (Request.QueryString["serverID"] != null))
            {
                TextBoxInputItemGuid.Text = Request.QueryString["itemGUID"];
                serverGroupDropDownList.SelectedValue = Request.QueryString["serverID"];
            }

            RecentLoginTime_end.SelectedDate = DateTime.Now;
            RecentLoginTime_start.SelectedDate = RecentLoginTime_end.SelectedDate.AddHours(-24);
        }
    }

    protected void LinkButtonQuery_Click(object sender, EventArgs e)
    {
        ViewState["totalResults"] = 0;
        ViewState["resultsPerPage"] = 30;
        ViewState["currentPage"] = 0;
        
        QueryAndDisplay(true);
    }

    protected void ButtonNextPage_Click(object sender, EventArgs e)
    {
        QueryAndDisplay(false);
    }

    protected void ButtonPreviousPage_Click(object sender, EventArgs e)
    {
        int currentPage = (int)ViewState["currentPage"];
        currentPage -= 2;
        ViewState["currentPage"] = currentPage;

        QueryAndDisplay(false);
    }

    protected void ButtonFirstPage_Click(object sender, EventArgs e)
    {
        ViewState["currentPage"] = 0;

        QueryAndDisplay(false);
    }

    private void QueryAndDisplay(bool firstTimeQuery)
    {
        //����eventTypeCheckBoxList�Ĺ�ѡ�����ȷ���ļ���queryString�����뵽���յ�SQL�����
        if (eventTypeCheckBoxList.SelectedIndex == -1)
        {
            LabelSuccess.Visible = false;
            LabelOpMsg.Text = "δѡ���¼����� - ����Ҫѡ��һ���¼�";
            LabelOpMsg.Visible = true;
            resultDataGrid.Visible = false;
            resultPagingInfoPanel.Visible = false;
            return;
        }
        
        string itemGUID = TextBoxInputItemGuid.Text;
        GameServer gameCenter = AdminServer.TheInstance.GameServerManager.GetGameServer(int.Parse(serverGroupDropDownList.SelectedValue));
        ArrayList paramList = new ArrayList();
        
        string queryString1 = " select logtime as Time,"
                            + " (case Field3"
                            + " when 1 then '����:ϵͳ����'"
                            + " when 2 then '����:�ϳ�'"
                            + " when 3 then '����:NPC�̵깺��'"
                            + " when 4 then '����:�������'"
                            + " when 5 then '����:NPC����'"
                            + " when 6 then '����:�ֹ����'"
                            + " when 7 then '����:�������'"                            
                            + " when 8 then '����:�ű�(����)'"
                            + " else 'δ֪�¼�'"
                            + " end) as eventname,"
                            + " Field30 as GoodsName, Field2 as userGUID, (select cRoleName from trolebaseinfo where trolebaseinfo.cRoleId = log_4_1.Field2 limit 1) as userName, Field31 as ItemDetails"
                            + " from log_4_1"
                            + " where Field1 = '?'";

        string queryString2 = " select logtime as Time,"
                            + " (case Field3"
                            + " when 1 then '����:ϵͳ����'"
                            + " when 2 then '����:ϵͳ����(�綪���ڵ��ϵ�)'"
                            + " when 3 then '����:��Ч����'"
                            + " when 4 then '����:ʹ������'"
                            + " when 5 then '����:��Ʒ����'"
                            + " when 6 then '����:����NPC'"
                            + " when 7 then '����:�ű�ɾ��(����)'"                            
                            + " when 8 then '����:װ��ĥ��'"
                            + " else 'δ֪�¼�'"
                            + " end) as eventname,"
                            + " (select Field30 from log_4_1 where log_4_1.field1 = log_4_2.field1 limit 1) as GoodsName,"
                            + " Field2 as userGUID,"
                            + " (select cRoleName from trolebaseinfo where trolebaseinfo.cRoleId = log_4_2.Field2 limit 1) as userName,"
                            + " (select Field31 from log_4_1 where log_4_1.field1 = log_4_2.field1 limit 1) as ItemDetails"
                            + " from log_4_2"
                            + " where Field1 = '?'";

        string queryString3 = " select logtime as Time,"
                            + " (case Field3"
                            + " when 1 then CONCAT('����:�����ڵ���',' �ص�:',(select mapName from map where mapid = Field4 limit 1  ),'  ����: ','X:',CAST(Field5 as char(12)),' Y:',CAST(Field6 as char(12)))"
                            + " when 2 then CONCAT('�ʼ�',' �ص�:',(select mapName from map where mapid = Field4 limit 1  ),'  ����: ','X:',CAST(Field5 as char(12)),' Y:',CAST(Field6 as char(12)))"
                            + " else 'δ֪�¼�'"
                            + " end) as eventname,"
                            + " (select Field30 from log_4_1 where log_4_1.field1 = log_4_3.field1 limit 1) as GoodsName,"
                            + " Field2 as userGUID,"
                            + " (select cRoleName from trolebaseinfo where trolebaseinfo.cRoleId = log_4_3.Field2 limit 1) as userName,"
                            + " (select Field31 from log_4_1 where log_4_1.field1 = log_4_3.field1 limit 1) as ItemDetails"
                            + " from log_4_3"
                            + " where Field1 = '?'";

        string queryString4 = " select logtime as Time,"
                            + " (case Field3"
                            + " when 1 then CONCAT('����',' �ص�:',(select mapName from map where mapid = Field4 limit 1  ),'  ����: ','X:',CAST(Field5 as char(12)),' Y:',CAST(Field6 as char(12)))"
                            + " when 2 then CONCAT('��ȡ���ʼ�����',' �ص�:',(select mapName from map where mapid = Field4 limit 1  ),'  ����: ','X:',CAST(Field5 as char(12)),' Y:',CAST(Field6 as char(12)))"
                            + " else 'δ֪�¼�'"
                            + " end) as eventname,"
                            + " (select Field30 from log_4_1 where log_4_1.field1 = log_4_4.field1 limit 1) as GoodsName,"
                            + " Field2 as userGUID,"
                            + " (select cRoleName from trolebaseinfo where trolebaseinfo.cRoleId = log_4_4.Field2 limit 1) as userName,"
                            + " (select Field31 from log_4_1 where log_4_1.field1 = log_4_4.field1 limit 1) as ItemDetails"
                            + " from log_4_4"
                            + " where Field1 = '?'";

        string queryString5 = " select logtime as Time,"
                            + " (case Field4"
                            + " when 1 then CONCAT('�����߱��:��ҽ���',' ���<',(select cRoleName from trolebaseinfo where cRoleId =Field2 limit 1),'>����:',(select mapName from map where mapid = Field5 limit 1),' X:',CAST(Field6 as char(12)),' Y:',CAST(Field7 as char(12)),' ����<',(select cRoleName from trolebaseinfo where cRoleId =Field3 limit 1),'>����:',(select mapName from map where mapid = Field8 limit 1),' X:',CAST(Field9 as char(12)),' Y:',CAST(Field10 as char(12)))"
                            + " when 2 then CONCAT('�����߱��:�չ�',' ���<',(select cRoleName from trolebaseinfo where cRoleId =Field2 limit 1),'>����:',(select mapName from map where mapid = Field5 limit 1),' X:',CAST(Field6 as char(12)),' Y:',CAST(Field7 as char(12)),' ����<',(select cRoleName from trolebaseinfo where cRoleId =Field3 limit 1),'>����:',(select mapName from map where mapid = Field8 limit 1),' X:',CAST(Field9 as char(12)),' Y:',CAST(Field10 as char(12)))"
                            + " when 3 then CONCAT('�����߱��:����',' ���<',(select cRoleName from trolebaseinfo where cRoleId =Field2 limit 1),'>����:',(select mapName from map where mapid = Field5 limit 1),' X:',CAST(Field6 as char(12)),' Y:',CAST(Field7 as char(12)),' ����<',(select cRoleName from trolebaseinfo where cRoleId =Field3 limit 1),'>����:',(select mapName from map where mapid = Field8 limit 1),' X:',CAST(Field9 as char(12)),' Y:',CAST(Field10 as char(12)))"
                            + " else 'δ֪�¼�'"
                            + " end) as eventname,"
                            + " (select Field30 from log_4_1 where log_4_1.field1 = log_4_5.field1 limit 1) as GoodsName,"
                            + " Field2 as userGUID,"
                            + " (select cRoleName from trolebaseinfo where trolebaseinfo.cRoleId = log_4_5.Field2 limit 1) as userName,"
                            + " (select Field31 from log_4_1 where log_4_1.field1 = log_4_5.field1 limit 1) as ItemDetails"
                            + " from log_4_5"
                            + " where Field1 = '?'";

        string queryString6 = " select logtime as Time,"
                            + " (case field3"
                            + " when 1 then (case field15"
                            + " when 0 then '���Ըı䣺��������'"
                            + " else CONCAT('���Ըı䣺ǿ��װ���� +', field15)"
                            + " end)"
                            + " else 'δ֪�¼�'"
                            + " end) as eventname,"
                            + " Field30 as GoodsName, Field2 as userGUID, (select cRoleName from trolebaseinfo where trolebaseinfo.cRoleId = log_4_6.Field2 limit 1) as userName, Field31 as ItemDetails"
                            + " from log_4_6"
                            + " where field1 = '?'";

        ArrayList queryStringsArrayList = new ArrayList();

        if (eventTypeCheckBoxList.Items.FindByValue("log_4_1").Selected == true)
            queryStringsArrayList.Add(queryString1);
        
        if (eventTypeCheckBoxList.Items.FindByValue("log_4_2").Selected == true)
            queryStringsArrayList.Add(queryString2);

        if (eventTypeCheckBoxList.Items.FindByValue("log_4_3").Selected == true)
            queryStringsArrayList.Add(queryString3);
        
        if (eventTypeCheckBoxList.Items.FindByValue("log_4_4").Selected == true)
            queryStringsArrayList.Add(queryString4);
        
        if (eventTypeCheckBoxList.Items.FindByValue("log_4_5").Selected == true)
            queryStringsArrayList.Add(queryString5);

        if (eventTypeCheckBoxList.Items.FindByValue("log_4_6").Selected == true)
            queryStringsArrayList.Add(queryString6);
        

        //������ʱ���TextBox���Ƕ�Ϊ��
        //ע�⣬TextBoxΪ��ʱ��SelectedDate���Ե�ֵΪDateTime.MinValue
        if (!((RecentLoginTime_start.SelectedDate == DateTime.MinValue) && (RecentLoginTime_end.SelectedDate == DateTime.MinValue)))
        {
            DateTime time1;
            DateTime time2;

            if (RecentLoginTime_start.SelectedDate == DateTime.MinValue)
            {
                time1 = DateTime.Now;
            }
            else
            {
                time1 = RecentLoginTime_start.SelectedDate;
            }
            if (RecentLoginTime_end.SelectedDate == DateTime.MinValue)
            {
                time2 = DateTime.Now;
            }
            else
            {
                time2 = RecentLoginTime_end.SelectedDate;
            }

            for (int i = 0; i < queryStringsArrayList.Count; i++)
            {
                queryStringsArrayList[i] += " and logtime between '?' and '?'";

                paramList.Add(itemGUID);

                if (time1 <= time2)
                {
                    paramList.Add(time1.ToString("yyyy-MM-dd HH:mm:ss"));
                    paramList.Add(time2.ToString("yyyy-MM-dd HH:mm:ss"));
                }
                else
                {
                    paramList.Add(time2.ToString("yyyy-MM-dd HH:mm:ss"));
                    paramList.Add(time1.ToString("yyyy-MM-dd HH:mm:ss"));
                }
            }
        }
        else
        {
            for (int i = 0; i < queryStringsArrayList.Count; i++)
            {
                paramList.Add(itemGUID);
            }
        }

        string queryString = String.Join(" union all ", (string[])queryStringsArrayList.ToArray(typeof(String)));

        queryString += " order by Time DESC ";

        queryString = queryString.Insert(7, " SQL_CALC_FOUND_ROWS ");

        int currentPage = (int)ViewState["currentPage"];
        int resultsPerPage = (int)ViewState["resultsPerPage"];
        int offset = resultsPerPage * currentPage;
        queryString += " limit " + offset.ToString() + "," + resultsPerPage.ToString();
        currentPage++;
        ViewState["currentPage"] = currentPage;

        SqlCommand sqlcmd = new SqlCommand(queryString, paramList.ToArray());
        SqlResult queryResult = WebUtil.QueryGameServerDb(CurrentUser.Id, gameCenter, sqlcmd);

        if (queryResult != null && queryResult.Success)
        {
            object[] record = null;

            DataTable dt = new DataTable();
            dt.Columns.Add(new DataColumn("time", typeof(DateTime)));
            dt.Columns.Add(new DataColumn("event", typeof(String)));
            dt.Columns.Add(new DataColumn("itemName", typeof(String)));
            dt.Columns.Add(new DataColumn("ownerID", typeof(String)));
            dt.Columns.Add(new DataColumn("ownerRoleName", typeof(String)));
            dt.Columns.Add(new DataColumn("itemDetails", typeof(String)));
            dt.Columns.Add(new DataColumn("gameCenterID", typeof(String)));

            DataRow dr;
            
            queryResult.SetFieldType
                (
                    new SqlDataType[] 
                    {
                        SqlDataType.DateTime,
                        SqlDataType.String,
                        SqlDataType.String,
                        SqlDataType.String,
                        SqlDataType.String,
                        SqlDataType.String,
                    }
                );

            while ((record = queryResult.ReadRecord()) != null)
            {
                dr = dt.NewRow();
                dr[0] = (DateTime)record[0];
                dr[1] = (String)record[1];
                dr[2] = (String)record[2];
                dr[3] = (String)record[3];
                dr[4] = (String)record[4];
                dr[5] = (String)record[5];
                dr[6] = serverGroupDropDownList.SelectedValue;
                dt.Rows.Add(dr);
            }

            DataView dv = new DataView(dt);

            if (dt.Rows.Count == 0)
            {
                LabelSuccess.Visible = false;
                LabelOpMsg.Text = "û�����������ļ�¼";
                LabelOpMsg.Visible = true;
                resultDataGrid.Visible = false;
                resultPagingInfoPanel.Visible = false;

                return;
            }

            resultDataGrid.DataSource = dv;
            resultDataGrid.DataBind();
            resultDataGrid.Visible = true;

            LabelOpMsg.Visible = false;
            LabelSuccess.Text = "��ѯ�ɹ����";
            LabelSuccess.Visible = true;

            //ֻ�����״β�ѯʱ��Ҳ���ǵ����ѯ��ťʱ��ȥ������������otherwise�������Ѿ���ViewState["totalResults"]����
            if (firstTimeQuery == true)
            {
                SqlCommand sqlcmdRowCount = new SqlCommand("select found_rows()", paramList.ToArray());
                SqlResult queryResultRowCount = WebUtil.QueryGameServerDb(CurrentUser.Id, gameCenter, sqlcmdRowCount);

                queryResultRowCount.SetFieldType(new SqlDataType[] { SqlDataType.String });

                object[] rowCountResult = queryResultRowCount.ReadRecord();

                ViewState["totalResults"] = rowCountResult[0];
            }

            int rowCount = int.Parse((string)ViewState["totalResults"]);
            resultInfoLabel.Text = "����" + rowCount.ToString() + "����¼��ÿҳ" + resultsPerPage.ToString() + "����¼����ǰҳ����" + currentPage.ToString();

            //��ǰҳ������1ʱ������������һҳ����ť�͡���һҳ����ť
            if (currentPage >= 2)
            {
                ButtonFirstPage.Enabled = true;
                ButtonPreviousPage.Enabled = true;
            }
            else
            {
                ButtonFirstPage.Enabled = false;
                ButtonPreviousPage.Enabled = false;
            }

            //����һҳ�����м�¼��ʱ�������������һҳ����ť
            if ((resultsPerPage * currentPage) < rowCount)
            {
                ButtonNextPage.Enabled = true;
            }
            else
            {
                ButtonNextPage.Enabled = false;
            }

            resultPagingInfoPanel.Visible = true;

        }
        else
        {
            if (queryResult == null)
            {
                LabelSuccess.Visible = false;
                LabelOpMsg.Text = StringDef.NoConnectionAlert;
                LabelOpMsg.Visible = true;
                resultDataGrid.Visible = false;
                resultPagingInfoPanel.Visible = false;
            }
            else
            {
                LabelSuccess.Visible = false;
                LabelOpMsg.Text = StringDef.OperationFail;
                LabelOpMsg.Visible = true;
                resultDataGrid.Visible = false;
                resultPagingInfoPanel.Visible = false;
            }
        }


    }





}