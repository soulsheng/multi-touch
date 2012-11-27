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

''' <summary>
''' Track a single picture.
''' </summary>
Imports System.Windows
Imports Windows7.Multitouch.Manipulation
Imports Windows7.Multitouch.WPF

Class PictureTracker

    Private _picture As Picture
    Public Property Picture() As Picture
        Get
            Return _picture
        End Get
        Set(ByVal value As Picture)
            _picture = value
        End Set
    End Property

    Private ReadOnly _pictureTrackerManager As PictureTrackerManager

    'Calculate the Inertia start velocity
    Private ReadOnly _inertiaParam As New InertiaParam()
    Private WithEvents _processor As New ManipulationInertiaProcessor(ProcessorManipulations.ALL, Factory.CreateTimer())

    Public Sub New(ByVal pictureTrackerManager As PictureTrackerManager)
        _pictureTrackerManager = pictureTrackerManager
    End Sub
    Private Sub Processor_OnManipulationStarted() Handles _processor.ManipulationStarted
        _inertiaParam.Reset()
    End Sub
    Private Sub Processor_OnManipulationCompleted() Handles _processor.ManipulationCompleted
        _inertiaParam.Stop()
        _pictureTrackerManager.Completed(Me)
    End Sub
    'Update picture state
    Private Sub ProcessManipulationDelta(ByVal sender As Object, ByVal e As ManipulationDeltaEventArgs) Handles _processor.ManipulationDelta
        If Picture Is Nothing Then Return

        Picture.X += e.TranslationDelta.Width
        Picture.Y += e.TranslationDelta.Height
        Picture.Angle += e.RotationDelta * 180 / Math.PI
        Picture.ScaleX *= e.ScaleDelta
        Picture.ScaleY *= e.ScaleDelta
    End Sub
    Public Sub ProcessDown(ByVal id As Integer, ByVal location As Point)
        _processor.ProcessDown(CUInt(id), location.ToDrawingPointF())
    End Sub
    Public Sub ProcessMove(ByVal id As Integer, ByVal location As Point)
        _processor.ProcessMove(CUInt(id), location.ToDrawingPointF())
    End Sub
    Public Sub ProcessUp(ByVal id As Integer, ByVal location As Point)
        _processor.ProcessUp(CUInt(id), location.ToDrawingPointF())
    End Sub
    
    ' Fingers removed, start inertia
    Private Sub OnBeforeInertia(ByVal sender As Object, ByVal e As BeforeInertiaEventArgs) Handles _processor.BeforeInertia
        'Tell the tracker manager that the user removed the fingers
        _pictureTrackerManager.InInertia(Me)
        _processor.InertiaProcessor.InertiaTimerInterval = 15
        _processor.InertiaProcessor.MaxInertiaSteps = 500
        _processor.InertiaProcessor.InitialVelocity = _inertiaParam.InitialVelocity
        _processor.InertiaProcessor.DesiredDisplacement = _inertiaParam.InitialVelocity.Magnitude * 250
        _processor.InertiaProcessor.InitialAngularVelocity = _inertiaParam.InitialAngularVelocity * 20.0F / CSng(Math.PI)
        _processor.InertiaProcessor.DesiredRotation = Math.Abs(_inertiaParam.InitialAngularVelocity * _processor.InertiaProcessor.InertiaTimerInterval * 540.0F / CSng(Math.PI))
        _processor.InertiaProcessor.InitialExpansionVelocity = _inertiaParam.InitialExpansionVelocity * 15
        _processor.InertiaProcessor.DesiredExpansion = Math.Abs(_inertiaParam.InitialExpansionVelocity * 4.0F)
    End Sub


    ' Keep track of object velocities.
    Private Class InertiaParam
        Private _initialVelocity As VectorF
        Public Property InitialVelocity() As VectorF
            Get
                Return _initialVelocity
            End Get
            Set(ByVal value As VectorF)
                _initialVelocity = value
            End Set
        End Property

        Private _initialAngularVelocity As Single
        Public Property InitialAngularVelocity() As Single
            Get
                Return _initialAngularVelocity
            End Get
            Set(ByVal value As Single)
                _initialAngularVelocity = value
            End Set
        End Property

        Private _initialExpansionVelocity As Single
        Public Property InitialExpansionVelocity() As Single
            Get
                Return _initialExpansionVelocity
            End Get
            Set(ByVal value As Single)
                _initialExpansionVelocity = value
            End Set
        End Property

        Public _stopwatch As New System.Diagnostics.Stopwatch()
        Public Sub Reset()
            InitialVelocity = New VectorF(0, 0)
            InitialAngularVelocity = 0
            InitialExpansionVelocity = 0
            _stopwatch.Reset()
            _stopwatch.Start()
        End Sub
        Public Sub [Stop]()
            _stopwatch.Stop()
        End Sub
        'update velocities, velocity = distance/time
        Public Sub Update(ByVal e As ManipulationDeltaEventArgs, ByVal history As Single)
            Dim elappsedMS = CSng(_stopwatch.ElapsedMilliseconds)
            If elappsedMS = 0 Then elappsedMS = 1

            InitialVelocity = InitialVelocity * history + (CType(e.TranslationDelta, VectorF) * (1.0F - history)) / elappsedMS
            InitialAngularVelocity = InitialAngularVelocity * history + (e.RotationDelta * (1.0F - history)) / elappsedMS
            InitialExpansionVelocity = InitialExpansionVelocity * history + (e.ExpansionDelta * (1.0F - history)) / elappsedMS
            _stopwatch.Reset()
            _stopwatch.Start()
        End Sub
    End Class
End Class