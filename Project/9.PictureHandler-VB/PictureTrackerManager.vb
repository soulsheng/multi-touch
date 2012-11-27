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

Imports System.Windows
Imports System.Windows.Controls

Class PictureTrackerManager
    ' Cache for re-use of picture trackers
    Private ReadOnly _pictureTrackers As New Stack(Of PictureTracker)()
    ' Map between touch ids and picture trackers
    Private ReadOnly _pictureTrackerMap As New Dictionary(Of Integer, PictureTracker)
    Private ReadOnly _canvas As Canvas
    Public Sub New(ByVal canvas As Canvas)
        _canvas = canvas
    End Sub
    Public Sub ProcessDown(ByVal sender As Object, ByVal args As StylusEventArgs)
        Dim location = args.GetPosition(_canvas)
        Dim pictureTracker = GetPictureTracker(args.StylusDevice.Id, location)

        If pictureTracker Is Nothing Then Return

        pictureTracker.ProcessDown(args.StylusDevice.Id, location)
    End Sub
    Public Sub ProcessUp(ByVal sender As Object, ByVal args As StylusEventArgs)
        Dim location = args.GetPosition(_canvas)
        Dim pictureTracker = GetPictureTracker(args.StylusDevice.Id)
        If pictureTracker Is Nothing Then Return

        pictureTracker.ProcessUp(args.StylusDevice.Id, location)
        _pictureTrackerMap.Remove(args.StylusDevice.Id)
    End Sub
    Public Sub ProcessMove(ByVal sender As Object, ByVal args As StylusEventArgs)
        Dim pictureTracker = GetPictureTracker(args.StylusDevice.Id)
        If pictureTracker Is Nothing Then Return

        Dim location = args.GetPosition(_canvas)
        pictureTracker.ProcessMove(args.StylusDevice.Id, location)
    End Sub
    Private Function GetPictureTracker(ByVal touchId As Integer) As PictureTracker
        Dim pictureTracker As PictureTracker = Nothing
        _pictureTrackerMap.TryGetValue(touchId, pictureTracker)
        Return pictureTracker
    End Function
    Private Function GetPictureTracker(ByVal touchId As Integer, ByVal location As Point) As PictureTracker
        Dim pictureTracker As PictureTracker = Nothing

        ' See if we already track the picture with the touchId
        If _pictureTrackerMap.TryGetValue(touchId, pictureTracker) Then Return pictureTracker

        ' Get the picture under the touch location
        Dim picture = FindPicture(location)
        If picture Is Nothing Then Return Nothing


        ' See if we track the picture with other ID
        pictureTracker = (From entry In _pictureTrackerMap _
                          Where entry.Value.Picture Is picture _
                          Select entry.Value).FirstOrDefault()

        ' First time
        If pictureTracker Is Nothing Then
            ' take from stack
            If _pictureTrackers.Count > 0 Then
                pictureTracker = _pictureTrackers.Pop()
            Else ' create new
                pictureTracker = New PictureTracker(Me)
            End If

            pictureTracker.Picture = picture
            BringPictureToFront(picture)
        End If
        ' remember the corelation between the touch id and the picture
        _pictureTrackerMap(touchId) = pictureTracker
        Return pictureTracker
    End Function
    ''' <summary>
    ''' Find the picture in the touch location
    ''' </summary>
    ''' <param name="pointF">touch location</param>
    ''' <returns>The picture or null if no picture exists in the touch
    ''' location</returns>
    Private Function FindPicture(ByVal location As Point) As Picture
        Dim result = VisualTreeHelper.HitTest(_canvas, location)
        If result Is Nothing Then Return Nothing

        Dim image = TryCast(result.VisualHit, Image)
        If image Is Nothing Then Return Nothing

        Return TryCast(image.Parent, Picture)
    End Function
    Private Sub BringPictureToFront(ByVal picture As Picture)
        If picture Is Nothing Then Return

        Dim children = (From child In _canvas.Children _
                        Where child IsNot picture _
                        Order By Canvas.GetZIndex(child) _
                        Select child).ToArray()

        For i = 0 To children.Length - 1
            Canvas.SetZIndex(children(i), i)
        Next i
        Canvas.SetZIndex(picture, children.Length)
    End Sub
    ' Manipulation is completed, we can reuse the object
    Public Sub Completed(ByVal pictureTracker As PictureTracker)
        pictureTracker.Picture = Nothing
        _pictureTrackers.Push(pictureTracker)
    End Sub
    ' We remove the touchID from the tracking map since the fingers are 
    ' no longer touching the picture
    Public Sub InInertia(ByVal pictureTracker As PictureTracker)
        ' remove all touch id from the map
        For Each id In (From entry In _pictureTrackerMap _
                                   Where entry.Value Is pictureTracker _
                                   Select entry.Key).ToList()

            _pictureTrackerMap.Remove(id)
        Next id
    End Sub
End Class