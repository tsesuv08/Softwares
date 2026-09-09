Imports System.Windows.Forms.VisualStyles.VisualStyleElement
Imports System.Windows.Forms.VisualStyles.VisualStyleElement.Button

Public Class Form1
    Private amount As Integer

    Private X As Integer
    Private Y As Integer

    Private Const RMrg As Integer = 120
    Private Const TMrg As Integer = 20

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        PictureBox1.Left = Me.Width / 2
        PictureBox1.Top = Me.Height / 2

        TextBox1.Text = ":("
        TextBox2.Text = 10
    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        If RadioButton1.Checked Then
            PictureBox1.Left -= amount
            If (PictureBox1.Left < 5) Then
                PictureBox1.Left = X - 10
            End If
        ElseIf RadioButton2.Checked Then
            PictureBox1.Left += amount
            If (X - 5 < PictureBox1.Left) Then
                PictureBox1.Left = 10
            End If
        ElseIf RadioButton3.Checked Then
            PictureBox1.Top -= amount
            If (PictureBox1.Top < 5) Then
                PictureBox1.Top = Y - 10
            End If
        ElseIf RadioButton4.Checked Then
            PictureBox1.Top += amount
            If (Y - 5 < PictureBox1.Top) Then
                PictureBox1.Top = 10
            End If
        End If

        TextBox1_TextChanged(sender, e)
    End Sub

    Private Sub PictureBox1_Click(sender As Object, e As EventArgs) Handles PictureBox1.Click
    End Sub

    Private Sub TextBox1_TextChanged(sender As Object, e As EventArgs) Handles TextBox1.TextChanged
        TextBox1.Text = $"X: {PictureBox1.Left}, Y: {PictureBox1.Top}"
    End Sub

    Private Sub TextBox2_TextChanged(sender As Object, e As EventArgs) Handles TextBox2.TextChanged
        Dim tmp As Integer
        tmp = Convert.ToInt32(TextBox2.Text)
        If (0 < tmp) Then
            amount = tmp
        End If
    End Sub

    Private Sub RadioButton1_CheckedChanged(sender As Object, e As EventArgs) Handles RadioButton1.CheckedChanged
    End Sub

    Private Sub RadioButton2_CheckedChanged(sender As Object, e As EventArgs) Handles RadioButton2.CheckedChanged
    End Sub

    Private Sub RadioButton3_CheckedChanged(sender As Object, e As EventArgs) Handles RadioButton3.CheckedChanged
    End Sub

    Private Sub RadioButton4_CheckedChanged(sender As Object, e As EventArgs) Handles RadioButton4.CheckedChanged
    End Sub

    Private Sub UpdateFormState() Handles MyBase.LocationChanged, MyBase.SizeChanged
        TextBox1.Left = Me.ClientSize.Width - RMrg
        TextBox1.Top = TMrg

        X = Me.Size.Width
        Y = Me.Size.Height
    End Sub
End Class
