Public Class frmImg
    Private Sub cmdCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdCancel.Click
        Me.Hide()
    End Sub

    Private Sub btnOK_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOK.Click
        '�����Ч��
        If Not IsNumeric(txtID.Text) Then
            MsgBox("��������ЧID")
            Exit Sub
        End If

        '����ͼƬ
        Dim tString As String

        '���ݼ��а�
        Dim str As String = ""
        str = System.Windows.Forms.Clipboard.GetText()

        '����rtf
        tString = isr.tga2rtf(txtID.Text, conn, RootDir & "\ui\Image\icon")

        '�ָ����а�
        Try
            System.Windows.Forms.Clipboard.SetText(str)
        Catch ex As Exception

        End Try



        If tString = "" Then
            tString = Module1.frmMain.res1.Text
        Else
            tString = Split(tString, "wmetafile8\picw")(0) & "wmetafile8\picw1001" & "\pich1000\picwgoal" & Split(tString, "\picwgoal")(1)
        End If


        '����������hash��
        hiddenData.Add(hiddenData.Count + 1, txtID.Text)

        '�۲��Ƿ���Ҫ�Զ��ӻس���ȥ
        With Module1.frmMain.txtShow
            If .SelectionStart > 0 Then
                Dim tstart As Integer = 0
                Dim tlen As Integer = 0
                tstart = .SelectionStart
                tlen = .SelectionLength
                .SelectionStart -= 1
                .SelectionLength = 1
                If .SelectionBackColor = Drawing.Color.Yellow Then
                    .SelectionStart += 1
                    .SelectionLength = 0
                    .SelectedText = vbLf
                    tstart += 1
                End If
                .SelectionStart = tstart
                .SelectionLength = tlen
            End If
        End With

        '׷��ͼƬ
        tString = Split(tString, "\pich1000")(0) & "\pich" & (1000 + hiddenData.Count) & Split(tString, "\pich1000")(1)
        Module1.frmMain.txtShow.SelectedRtf = tString
        Me.Hide()
    End Sub

    Private Sub txtID_KeyDown(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles txtID.KeyDown
        If e.KeyCode = 13 Then
            Call btnOK_Click(sender, e)
        End If
    End Sub

    Private Sub txtSelect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles txtSelect.Click
        Dim t As IconSelector.Implement = Module1.isDiag
        Dim r As Integer
        conn.Open()
        If txtID.Text = "" Then
            t.IconID = -1
        Else
            t.IconID = txtID.Text
        End If
        If bisInit = False Then
            bisInit = True
            t.Init(conn, (RootDir & "\ui\Image\icon"))
        End If
        r = t.Show()
        If r <> -1 Then txtID.Text = r
        conn.Close()
    End Sub

    Private Sub PanelEx1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PanelEx1.Click

    End Sub

    Private Sub frmImg_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load

    End Sub

    Private Sub frmImg_Resize(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Resize
        txtID.Focus()
    End Sub
End Class