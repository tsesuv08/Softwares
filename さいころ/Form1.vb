Public Class Form1
    Private flag As Boolean = False

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        flag = False
        Label1.Text = "IDK :("
    End Sub

    Private Sub dice()
        Dim i = rdice()
        PictureBox1.Visible = False
        PictureBox2.Visible = False
        PictureBox3.Visible = False
        PictureBox4.Visible = False
        PictureBox5.Visible = False
        PictureBox6.Visible = False
        If i = 1 Then PictureBox1.Visible = True
        If i = 2 Then PictureBox2.Visible = True
        If i = 3 Then PictureBox3.Visible = True
        If i = 4 Then PictureBox4.Visible = True
        If i = 5 Then PictureBox5.Visible = True
        If i = 6 Then PictureBox6.Visible = True
    End Sub

    Private Function rdice()
        Dim s As New Random
        Dim i As Integer
        i = s.Next(1, 7)

        Return i
    End Function

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        dice()
    End Sub

    Private Async Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        flag = True

        While flag
            dice()
            Await Task.Delay(1)
        End While

        flag = True
    End Sub

    Private Sub Button3_Click(sender As Object, e As EventArgs) Handles Button3.Click
        flag = False
    End Sub

    Private Sub Button4_Click(sender As Object, e As EventArgs) Handles Button4.Click
        Dim d1 = rdice()
        Dim d2 = rdice()
        Label1.Text = $"{d1} + {d2} = {d1 + d2}, {d1} x {d2} = {d1 * d2}"
    End Sub
End Class
