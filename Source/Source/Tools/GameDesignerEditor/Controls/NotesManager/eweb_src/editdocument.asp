<%
response.buffer = false
%>
<!--#include file="conn.asp"-->
<%
id = request.querystring("id")
if id = 0 then
	'no ducument
	response.write "ID error~"
	response.end
end if
set rs = conn.execute("select * from sys_notes where id=" & id)
if rs.eof then
	'no ducument
	response.write "No ducument exist~"
	response.end
end if
'format_string = "<STYLE>p{margin:0} </STYLE>"
out_string = "" & rs("htmlcode")
'if instr(out_string, format_string) = 0 then
'	out_string = vbcrlf  & format_string & out_string
'end if
%>
<head>
	<script language="Javascript">
		function getstring()
		{
			return(eWebEditor1.getHTML());
		}
	</script>
</head>

<INPUT id="neirong" type="hidden" name="content1" value="<%=server.htmlencode(out_string)%>">
<div style="position:absolute;top:0;left:0;width:100%;height:100%;">
<IFRAME ID="eWebEditor1" src="ewebeditor.htm?id=content1&style=coolblue" frameborder="0" scrolling="no" width="100%" height="100%"></IFRAME></div>



<!--
<TR>
	<TD colspan=2 align=right>
	<INPUT type=submit value="�ύ"> 
	<INPUT type=reset value="����"> 
	<INPUT type=button value="�鿴Դ�ļ�" onclick="location.replace('view-source:'+location)"> 
	</TD>
</TR>
<TR>
	<TD>HTML���룺</TD>
	<TD><TEXTAREA cols=50 rows=5 id=myTextArea style="width:550px">�����ȡֵ����ť����һ��Ч����</TEXTAREA></TD>
</TR>
<TR>
	<TD colspan=2 align=right>
	<INPUT type=button value="ȡֵ(HTML)" onclick="myTextArea.value=eWebEditor1.getHTML()"> 
	<INPUT type=button value="ȡֵ(���ı�)" onclick="myTextArea.value=eWebEditor1.getText()"> 
	<INPUT type=button value="��ֵ" onclick="eWebEditor1.setHTML('<b>Hello My World!</b>')">
	<INPUT type=button value="��ǰλ�ò���" onclick="eWebEditor1.insertHTML('This is insertHTML function!')">
	<INPUT type=button value="β��׷��" onclick="eWebEditor1.appendHTML('This is appendHTML function!')">
	<br>
	<INPUT type=button value="����״̬" onclick="eWebEditor1.setMode('CODE')">
	<INPUT type=button value="���״̬" onclick="eWebEditor1.setMode('EDIT')">
	<INPUT type=button value="�ı�״̬" onclick="eWebEditor1.setMode('TEXT')">
	<INPUT type=button value="Ԥ��״̬" onclick="eWebEditor1.setMode('VIEW')">
	<br>
	<INPUT type=button value="Ӣ������" onclick="alert(eWebEditor1.getCount(0))">
	<INPUT type=button value="��������" onclick="alert(eWebEditor1.getCount(1))">
	<INPUT type=button value="��Ӣ������(���ļ�1)" onclick="alert(eWebEditor1.getCount(2))">
	<INPUT type=button value="��Ӣ������(���ļ�2)" onclick="alert(eWebEditor1.getCount(3))">
	</TD>
</TR>
-->
