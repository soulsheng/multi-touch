// ----------------------------------------------------------------------------------
// Microsoft Developer & Platform Evangelism
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
// 
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES 
// OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// ----------------------------------------------------------------------------------
// The example companies, organizations, products, domain names,
// e-mail addresses, logos, people, places, and events depicted
// herein are fictitious.  No association with any real company,
// organization, product, domain name, email address, logo, person,
// places, or events is intended or should be inferred.
// ----------------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Windows7.Multitouch.Manipulation;
using Windows7.Multitouch.WPF;
using System.Windows;

namespace MultitouchHOL
{
    /// <summary>
    /// Track a single picture
    /// </summary>
    class PictureTracker
    {
        public Picture Picture { get; set; }

        private readonly PictureTrackerManager _pictureTrackerManager;

        //Calculate the inertia start velocity
        private readonly InertiaParam _inertiaParam = new InertiaParam();

        private readonly ManipulationInertiaProcessor _processor =
            new ManipulationInertiaProcessor(ProcessorManipulations.ALL, Factory.CreateTimer());

        public PictureTracker(PictureTrackerManager pictureTrackerManager)
        {
            _pictureTrackerManager = pictureTrackerManager;

            //Start inertia velocity calculations
            _processor.ManipulationStarted += (s, e) =>
            {
                _inertiaParam.Reset();
            };

            //All completed, inform the tracker manager that the current tracker can be reused
            _processor.ManipulationCompleted += (s, e) => { _inertiaParam.Stop(); pictureTrackerManager.Completed(this); };
            _processor.ManipulationDelta += ProcessManipulationDelta;
            _processor.BeforeInertia += OnBeforeInertia;
        }

        //Update picture state
        private void ProcessManipulationDelta(object sender, ManipulationDeltaEventArgs e)
        {
            if (Picture == null)
                return;

            Picture.X += e.TranslationDelta.Width;
            Picture.Y += e.TranslationDelta.Height;

            Picture.Angle += e.RotationDelta * 180 / Math.PI;

            Picture.ScaleX *= e.ScaleDelta;
            Picture.ScaleY *= e.ScaleDelta;

            //Update inertia calculation. Take 40 percent from the previos data
            _inertiaParam.Update(e, 0.4F);
        }

        public void ProcessDown(int id, Point location)
        {
            _processor.ProcessDown((uint)id, location.ToDrawingPointF());
        }

        public void ProcessMove(int id, Point location)
        {
            _processor.ProcessMove((uint)id, location.ToDrawingPointF());
        }

        public void ProcessUp(int id, Point location)
        {
            _processor.ProcessUp((uint)id, location.ToDrawingPointF());
        }

        //Fingers removed, start inertia
        void OnBeforeInertia(object sender, BeforeInertiaEventArgs e)
        {
            //Tell the tracker manager that the user removed the fingers
            _pictureTrackerManager.InInertia(this);

            _processor.InertiaProcessor.InertiaTimerInterval = 15;
            _processor.InertiaProcessor.MaxInertiaSteps = 500;
            _processor.InertiaProcessor.InitialVelocity = _inertiaParam.InitialVelocity;
            _processor.InertiaProcessor.DesiredDisplacement = _inertiaParam.InitialVelocity.Magnitude * 250;
            _processor.InertiaProcessor.InitialAngularVelocity = _inertiaParam.InitialAngularVelocity * 20F / (float)Math.PI;
            _processor.InertiaProcessor.DesiredRotation = Math.Abs(_inertiaParam.InitialAngularVelocity * _processor.InertiaProcessor.InertiaTimerInterval * 540F / (float)Math.PI);
            _processor.InertiaProcessor.InitialExpansionVelocity = _inertiaParam.InitialExpansionVelocity * 15;
            _processor.InertiaProcessor.DesiredExpansion = Math.Abs(_inertiaParam.InitialExpansionVelocity * 4F);
        }

        //Keep track of object velocities
        private class InertiaParam
        {
            public VectorF InitialVelocity { get; set; }
            public float InitialAngularVelocity { get; set; }
            public float InitialExpansionVelocity { get; set; }
            public System.Diagnostics.Stopwatch _stopwatch = new System.Diagnostics.Stopwatch();
            public void Reset()
            {
                InitialVelocity = new VectorF(0, 0);
                InitialAngularVelocity = 0;
                InitialExpansionVelocity = 0;
                _stopwatch.Reset();
                _stopwatch.Start();
            }

            public void Stop()
            {
                _stopwatch.Stop();
            }

            //update velocities, velocity = distance/time
            public void Update(ManipulationDeltaEventArgs e, float history)
            {
                float elappsedMS = (float)_stopwatch.ElapsedMilliseconds;
                if (elappsedMS == 0)
                    elappsedMS = 1;

                InitialVelocity = InitialVelocity * history + ((VectorF)e.TranslationDelta * (1F - history)) / elappsedMS;
                InitialAngularVelocity = InitialAngularVelocity * history + (e.RotationDelta * (1F - history)) / elappsedMS;
                InitialExpansionVelocity = InitialExpansionVelocity * history + (e.ExpansionDelta * (1F - history)) / elappsedMS;
                _stopwatch.Reset();
                _stopwatch.Start();
            }
        }
    }
}