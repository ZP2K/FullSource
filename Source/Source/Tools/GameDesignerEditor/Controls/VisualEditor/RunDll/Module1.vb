Imports System.Data


Module Module1

    Sub Main()
        Dim t As New VisualEditor.Doit
        Dim conn As New System.Data.SqlClient.SqlConnection

        'Dim tga As New tgaDialog.iconSelector


        'tga.show(2, conn, "a")

        conn.ConnectionString = "Server = jx3web; DataBase = s3design_rtm; Uid = s3design; Password = ddd12345678"
        MsgBox(t.show("����Ŀ�꣨�����Ҫ˵����\n<$asdfasdfcddd>������Ŀdsfg\n<$asdf>\nsdfasdf", conn, "D:\Kingsoft\sword3-products\trunk\client"), 0)



    End Sub

End Module
