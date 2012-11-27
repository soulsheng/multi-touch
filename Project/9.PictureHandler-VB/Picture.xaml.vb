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

''' <summary>
''' Interaction logic for Picture.xaml
''' </summary>
Class Picture

    Public Sub New()
        InitializeComponent()
        DataContext = Me
    End Sub

    Public Property ImagePath() As String
        Get
            Return CStr(GetValue(ImagePathProperty))
        End Get
        Set(ByVal value As String)
            SetValue(ImagePathProperty, value)
        End Set
    End Property

    ' Using a DependencyProperty as the backing store for ImagePath.  This enables animation, styling, binding, etc...
    Public Shared ReadOnly ImagePathProperty As DependencyProperty = DependencyProperty.Register("ImagePath", GetType(String), GetType(Picture), New UIPropertyMetadata(""))


    Public Property Angle() As Double
        Get
            Return CDbl(GetValue(AngleProperty))
        End Get
        Set(ByVal value As Double)
            SetValue(AngleProperty, value)
        End Set
    End Property

    ' Using a DependencyProperty as the backing store for Angle.  This enables animation, styling, binding, etc...
    Public Shared ReadOnly AngleProperty As DependencyProperty = DependencyProperty.Register("Angle", GetType(Double), GetType(Picture), New UIPropertyMetadata(0.0))



    Public Property ScaleX() As Double
        Get
            Return CDbl(GetValue(ScaleXProperty))
        End Get
        Set(ByVal value As Double)
            SetValue(ScaleXProperty, value)
        End Set
    End Property

    ' Using a DependencyProperty as the backing store for ScaleX.  This enables animation, styling, binding, etc...
    Public Shared ReadOnly ScaleXProperty As DependencyProperty = DependencyProperty.Register("ScaleX", GetType(Double), GetType(Picture), New UIPropertyMetadata(1.0))

    Public Property ScaleY() As Double
        Get
            Return CDbl(GetValue(ScaleYProperty))
        End Get
        Set(ByVal value As Double)
            SetValue(ScaleYProperty, value)
        End Set
    End Property

    ' Using a DependencyProperty as the backing store for ScaleY.  This enables animation, styling, binding, etc...
    Public Shared ReadOnly ScaleYProperty As DependencyProperty = DependencyProperty.Register("ScaleY", GetType(Double), GetType(Picture), New UIPropertyMetadata(1.0))

    Public Property X() As Double
        Get
            Return CDbl(GetValue(XProperty))
        End Get
        Set(ByVal value As Double)
            SetValue(XProperty, value)
        End Set
    End Property

    ' Using a DependencyProperty as the backing store for X.  This enables animation, styling, binding, etc...
    Public Shared ReadOnly XProperty As DependencyProperty = DependencyProperty.Register("X", GetType(Double), GetType(Picture), New UIPropertyMetadata(0.0))


    Public Property Y() As Double
        Get
            Return CDbl(GetValue(YProperty))
        End Get
        Set(ByVal value As Double)
            SetValue(YProperty, value)
        End Set
    End Property

    ' Using a DependencyProperty as the backing store for Y.  This enables animation, styling, binding, etc...
    Public Shared ReadOnly YProperty As DependencyProperty = DependencyProperty.Register("Y", GetType(Double), GetType(Picture), New UIPropertyMetadata(0.0))
End Class