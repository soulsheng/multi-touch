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

using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace MultitouchHOL
{
    /// <summary>
    /// Managing the manipulation of multiple pictures in the same time
    /// </summary>
    class PictureTrackerManager
    {
        //Cache for re-use of picture trackers 
        private readonly Stack<PictureTracker> _pictureTrackers = new Stack<PictureTracker>();

        //Map between touch ids and picture trackers
        private readonly Dictionary<int, PictureTracker> _pictureTrackerMap = new Dictionary<int, PictureTracker>();
        private readonly Canvas _canvas;

        public PictureTrackerManager(Canvas canvas)
        {
            _canvas = canvas;
        }

        public void ProcessDown(object sender, StylusEventArgs args)
        {
            Point location = args.GetPosition(_canvas);
            PictureTracker pictureTracker = GetPictureTracker(args.StylusDevice.Id, location);

            if (pictureTracker == null)
                return;

            pictureTracker.ProcessDown(args.StylusDevice.Id, location);
        }

        public void ProcessUp(object sender, StylusEventArgs args)
        {
            Point location = args.GetPosition(_canvas);
            PictureTracker pictureTracker = GetPictureTracker(args.StylusDevice.Id, location);

            if (pictureTracker == null)
                return;

            pictureTracker.ProcessUp(args.StylusDevice.Id, location);
        }

        public void ProcessMove(object sender, StylusEventArgs args)
        {
            PictureTracker pictureTracker = GetPictureTracker(args.StylusDevice.Id);

            if (pictureTracker == null)
                return;

            Point location = args.GetPosition(_canvas);
            pictureTracker.ProcessMove(args.StylusDevice.Id, location);
        }

        private PictureTracker GetPictureTracker(int touchId)
        {
            PictureTracker pictureTracker = null;

            _pictureTrackerMap.TryGetValue(touchId, out pictureTracker);

            return pictureTracker;
        }

        private PictureTracker GetPictureTracker(int touchId, Point location)
        {
            PictureTracker pictureTracker;

            //See if we already track the picture with the touchId
            if (_pictureTrackerMap.TryGetValue(touchId, out pictureTracker))
                return pictureTracker;

            //Get the picture under the touch location
            Picture picture = FindPicture(location);

            if (picture == null)
                return null;

            //See if we track the picture with other ID
            pictureTracker = (from KeyValuePair<int, PictureTracker> entry in _pictureTrackerMap
                              where entry.Value.Picture == picture
                              select entry.Value).FirstOrDefault();

            //First time
            if (pictureTracker == null)
            {
                //take from stack
                if (_pictureTrackers.Count > 0)
                    pictureTracker = _pictureTrackers.Pop();
                else //create new
                    pictureTracker = new PictureTracker(this);

                pictureTracker.Picture = picture;
                BringPictureToFront(picture);
            }

            //remember the corelation between the touch id and the picture
            _pictureTrackerMap[touchId] = pictureTracker;

            return pictureTracker;
        }

        //We remove the touchID from the tracking map since the fingers are no longer touch
        //the picture
        public void InInertia(PictureTracker pictureTracker)
        {
            //remove all touch id from the map
            foreach (int id in
                (from KeyValuePair<int, PictureTracker> entry in _pictureTrackerMap
                 where entry.Value == pictureTracker
                 select entry.Key).ToList())
            {
                _pictureTrackerMap.Remove(id);
            }
        }

        //Inertia is completed, we can reuse the object
        public void Completed(PictureTracker pictureTracker)
        {
            pictureTracker.Picture = null;
            _pictureTrackers.Push(pictureTracker);
        }

        /// <summary>
        /// Find the picture in the touch location
        /// </summary>
        /// <param name="pointF">touch location</param>
        /// <returns>The picture or null if no picture exists in the touch location</returns>
        private Picture FindPicture(Point location)
        {
            HitTestResult result = VisualTreeHelper.HitTest(_canvas, location);

            if (result == null)
                return null;

            Image image = result.VisualHit as Image;

            if (image == null)
                return null;

            return image.Parent as Picture;
        }

        private void BringPictureToFront(Picture picture)
        {
            if (picture == null)
                return;

            var children = (from UIElement child in _canvas.Children
                            where child != picture
                            orderby Canvas.GetZIndex(child)
                            select child).ToArray();

            for (int i = 0; i < children.Length; ++i)
            {
                Canvas.SetZIndex(children[i], i);
            }
            Canvas.SetZIndex(picture, children.Length);
        }
    }
}