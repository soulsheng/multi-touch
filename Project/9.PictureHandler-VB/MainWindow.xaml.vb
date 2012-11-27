' ----------------------------------------------------------------------------------
' Microsoft Developer & Platform Evangelism
' 
' Copyright (c) Microsoft Corporation. All rights reserved.
' 
' THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
' EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
' OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
' ----------------------------------------------------------------------------------
' The example companies, organizations, products, domain names,
' e-mail addresses, logos, people, places, and events depicted
' herein are fictitious.  No association with any real company,
' organization, product, domain name, email address, logo, person,
' places, or events is intended or should be inferred.
' ----------------------------------------------------------------------------------

Imports System.Text
Imports System.IO
Imports Windows7.Multitouch
Imports Windows7.Multitouch.WPF

''' <summary>
''' Interaction logic for MainWindow.xaml
''' </summary>
Class MainWindow

    Private _picture As Picture
    Private _prevLocation As System.Windows.Point
    Private ReadOnly _pictureTrackerManager As PictureTrackerManager

    Public Sub New()
        InitializeComponent()
        _pictureTrackerManager = New PictureTrackerManager(_canvas)

        If Not Windows7.Multitouch.TouchHandler.DigitizerCapabilities.IsMultiTouchReady Then
            MsgBox("Multitouch is not availible")
            Environment.Exit(1)
        End If

        ' Register for stylus (touch) events
        AddHandler Me.StylusDown, AddressOf _pictureTrackerManager.ProcessDown
        AddHandler Me.StylusMove, AddressOf _pictureTrackerManager.ProcessMove
        AddHandler Me.StylusUp, AddressOf _pictureTrackerManager.ProcessUp

    End Sub

    ' Enable stylus events and load pictures.
    Private Sub Window_OnLoaded() Handles Me.Loaded
        Factory.EnableStylusEvents(Me)
        LoadPictures()
    End Sub

    ' Return collection of file/resource picture locations.
    Private Function GetPictureLocations() As String()
        Dim pictures() = Directory.GetFiles(Environment.GetFolderPath(Environment.SpecialFolder.MyPictures), "*.jpg")

        ' If there are no pictures in MyPictures.
        If pictures.Length = 0 Then pictures = New String() {"images\Pic1.jpg", "images\Pic2.jpg", "images\Pic3.jpg", "images\Pic4.jpg"}

        Return pictures
    End Function


    ' Load pictures to the canvas.
    Private Sub LoadPictures()
        Dim pictureLocations() = GetPictureLocations()
        Dim angle = 0
        Dim angleStep = 360 \ pictureLocations.Length

        For Each filePath In pictureLocations
            Try
                Dim p As New Picture()
                p.ImagePath = filePath
                p.Width = 300
                p.Angle = 180 - angle
                Dim angleRad = angle * Math.PI / 180.0
                p.X = Math.Sin(angleRad) * 300 + (_canvas.ActualWidth - 300) / 2.0
                p.Y = Math.Cos(angleRad) * 300 + (_canvas.ActualHeight - 300) / 2.0
                _canvas.Children.Add(p)

                angle += angleStep
            Catch ex As Exception
                System.Diagnostics.Trace.WriteLine("Error:" & ex.Message)
            End Try
        Next filePath
    End Sub

    ''' <summary>
    ''' Find the picture in the touch location
    ''' </summary>
    ''' <param name="pointF">touch location</param>
    ''' <returns>The picture or null if no picture exists in the touch location</returns>
    Private Function FindPicture(ByVal location As System.Windows.Point) As Picture
        Dim result = VisualTreeHelper.HitTest(_canvas, location)

        If result Is Nothing Then Return Nothing

        Dim image = TryCast(result.VisualHit, System.Windows.Controls.Image)

        If image Is Nothing Then Return Nothing

        Return TryCast(image.Parent, Picture)
    End Function

    Private Sub BringPictureToFront(ByVal picture As Picture)
        If picture Is Nothing Then Return

        Dim children = ( _
            From child In _canvas.Children _
            Where child IsNot picture _
            Order By Canvas.GetZIndex(child) _
            Select child).ToArray()

        For i = 0 To children.Length - 1
            Canvas.SetZIndex(children(i), i)
        Next i
        Canvas.SetZIndex(picture, children.Length)
    End Sub

End Class