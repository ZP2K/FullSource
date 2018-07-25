Imports System.Windows.Forms
Imports System.Resources
Imports System.Drawing
Public Class frmSelText

    Private Sub btnOK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOK.Click
        Dim tstart As Integer = 0
        Dim tlen As Integer = 0
        With Module1.frmMain.txtShow
            '�����Ч��
            If txtLabel.Text = "" Then
                MsgBox("����������", Microsoft.VisualBasic.MsgBoxStyle.Critical)
                Exit Sub
            End If


            Dim i As Integer
            Dim tmps As String = ""
            For i = 1 To Len(txtLabel.Text)
                tmps &= charReplace(Mid(txtLabel.Text, i, 1))
            Next
            If tmps <> txtLabel.Text Then
                i = MsgBox("�������ֲ����Ϲ淶��ִ���Զ�������", Microsoft.VisualBasic.MsgBoxStyle.YesNo + Microsoft.VisualBasic.MsgBoxStyle.Exclamation, "ѯ��")
                If i = vbYes Then
                    txtLabel.Text = tmps
                Else
                    Exit Sub
                End If
            End If

            '����ͼƬ
            Dim tString As String
            If cbClose.Checked = True Then
                tString = Module1.frmMain.res2.Text
            Else
                tString = Module1.frmMain.res3.Text
            End If

            '����������hash��
            hiddenData.Add(hiddenData.Count + 1, txtLabel.Text)


            '�۲��Ƿ���Ҫ�Զ��ӻس���ȥ
            If .SelectionStart > 0 Then
                tstart = .SelectionStart
                tlen = .SelectionLength
                .SelectionStart -= 1
                .SelectionLength = 1
                If .SelectedText <> vbLf Then
                    .SelectionStart += 1
                    .SelectionLength = 0
                    .SelectedText = vbLf
                    tstart += 1
                End If
                .SelectionStart = tstart
                .SelectionLength = tlen
            End If

            '׷��ͼƬ
            tString = Split(tString, "\pich1000")(0) & "\pich" & (1000 + hiddenData.Count) & Split(tString, "\pich1000")(1)
            .SelectedRtf = tString
            .SelectionLength = 0

            'ѡ������ѡ��׷�ӵ�����,���滻

            Dim tbr As Integer
            tstart = .SelectionStart
            .SelectionStart += 1
            While .SelectionBackColor = Color.Yellow
                tbr = .SelectionStart
                .SelectionStart += 1
                If .SelectionStart = tbr Then
                    tbr = -1
                    Exit While
                End If
            End While

            tlen = .SelectionStart - tstart
            If tbr = -1 Then tlen += 1

            .SelectionStart = tstart
            tlen -= 1
            If tlen < 0 Then tlen = 0
            .SelectionLength = tlen

            .SelectionBackColor = Color.Yellow
            .SelectedText = txtLabel.Text
            .SelectionBackColor = Color.FromName("window")


            '�۲��Ƿ���Ҫ��׷�ӻس���ȥ
            .SelectionLength = 1
            If .SelectedText <> vbLf Then
                .SelectionLength = 0
                .SelectedText = vbLf
            End If


            Me.Hide()
        End With
    End Sub

    Private Sub cmdCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdCancel.Click
        Me.Hide()
    End Sub

    Private Sub PanelEx1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PanelEx1.Click

    End Sub

    Private Sub txtLabel_KeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles txtLabel.KeyDown
        If e.KeyCode = 13 Then
            Call btnOK_Click(sender, e)
        End If
    End Sub

    Private Sub txtLabel_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles txtLabel.TextChanged

    End Sub
End Class