/*
 *  Project AZUSA © 2015-2017 ( https://github.com/Project-AZUSA )
 *  Emote.NET Sample - NekoSpirit
 *  LICENSE:CC 4.0 BY-NC-SA
 *  AUTHOR:	Ulysses (wdwxy12345@gmail.com)
 */

using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading;
using System.Windows.Forms;
using FreeMote;
using SharpDX;
using SharpDX.Direct3D9;
using SharpDX.Mathematics.Interop;
using SharpDX.Windows;
using Point = System.Drawing.Point;
using Rectangle = System.Drawing.Rectangle;

namespace NekoHacks
{
    class Program
    {
        const double Refresh = 1.0 / 60.0;
        const int Width = 900;
        const int Height = 600;

        private static RenderForm _form;
        private static Emote _e;
        private static EmotePlayer _player;
        private static EmotePlayer _player2;
        private static PreciseTimer _timer = new PreciseTimer();
        private static double _elaspedTime;
        private static Device _device;
        private static Rectangle _screenRect;

        private static int _lastX, _lastY;
        private static bool _leftMouseDown;
        private static bool _rightMouseDown;

        static unsafe void Main(string[] args)
        {
            Application.EnableVisualStyles();

            _screenRect = Screen.PrimaryScreen.Bounds;
            _form = new RenderForm("AZUSA E-mote Sample - NekoSpirit");
            _form.AutoScaleMode = AutoScaleMode.None;
            //_form.FormBorderStyle = FormBorderStyle.None;
            _form.ClientSize = new Size(Width, Height);
            _form.Resize += FormOnResize;
            FormOnResize(_form, null);
            //form.TopMost = true;
            _form.StartPosition = FormStartPosition.Manual;
            _form.Location = new Point(_screenRect.Width - _form.Width, _screenRect.Height - _form.Height);
            //form.AllowTransparency = true;            
            
            _e = new Emote(_form.Handle, Width, Height, false);

            _e.EmoteInit();
            _e.Device.TextureFilter = GrayTextureFilter;

            if (File.Exists(args[0]))
            {
                _player = _e.CreatePlayer("Chara1", args[0]);
            }
            else
            {
                _player = _e.CreatePlayer("Chara1", "emote_test.pure.psb");
            }
            _e.Device.UseTextureFilter = false; //turn on texture filter!
            //e.Device.SetMaskRegionClipping(true); //Set this to true will increase CPU usage but decrease GPU usage.
            //_player2 = _e.CreatePlayer("Chara2", "chocola-pure.psb");

            //I don't know how to use this method. I guess this method may combine two models (only if they are complementary).
            //virtual void  CreatePlayer (emote_uint32_t emoteObjectNum, const emote_uint8_t **emoteObjectImage, const emote_uint32_t *emoteObjectSize, class IEmotePlayer **player)=0 
            //_player2 = e.CreatePlayer("Chocola", new[] {"dx_e-mote3.0ショコラ制服b_鈴あり.psb", "dx_e-mote3.0バニラ私服b_鈴あり.psb"});

            _player.Show();
            //_player2.Show();

            var faceTable = new Dictionary<string, float>
            {
                {"face_eye_UD", -30f},
                {"face_eye_LR", 0.0f},
                {"face_eye_open", 5},
                {"face_eyebrow", 20f},
                {"face_mouth", 20},
                {"face_talk", 0.5f},
                {"face_tears", 0.0f},
                {"face_cheek", 0.0f}
            };

            _device = new Device(new IntPtr(_e.D3Device));
            _player.SetScale(0.4f, 0, 0);
            _player.SetCoord(300, 50, 0, 0);
            _player.StartWind(0f, 1f, 0.8f, 0.5f, 0.8f);
            _player.SetSmoothing(true);

            //for (uint i = 0; i < _player.CountVariables(); i++)
            //{
            //    for (uint j = 0; j < _player.CountVariableFrameAt(i); j++)
            //    {
            //        Console.WriteLine($"{_player.GetVariableFrameLabelAt(i, j)}:{_player.GetVariableFrameValueAt(i,j)}");
            //    }
            //}

            //_player2.SetScale(0.4f, 0, 0);
            //_player2.SetCoord(-100, 50, 0, 0);
            //_player2.SetSmoothing(true);

            //_player2.SetVariables(faceTable, 2000f, 1f); //-Why are you so angry? -Why am I monochrome?

            //_player.PlayTimeline("変な動き", TimelinePlayFlags.TIMELINE_PLAY_PARALLEL);
            //_player2.PlayTimeline("差分用_waiting_loop", TimelinePlayFlags.TIMELINE_PLAY_PARALLEL);
            //ctrlform.TopMost = true;
            Thread th = new Thread(() =>
            {
                //var ctrlform2 = new FormConsole(_player2, "Chara2");
                //ctrlform2.Closed +=
                //    (sender, eventArgs) =>
                //    {
                //        _e.DeletePlayer(_player2); //Try to close this form, the memory will be released
                //    };
                //ctrlform2.Show();
                var ctrlform = new FormConsole(_player, "Chara1");
                //ctrlform.Show();
                Application.Run(ctrlform);
            });
            th.Start();
            //Control
            _form.MouseClick += form_MouseClick;
            _form.MouseDown += form_MouseDown;
            _form.MouseMove += form_MouseMove;
            _form.MouseUp += form_MouseUp;
            _form.MouseWheel += (sender, eventArgs) => _player.OffsetScale(1 + ConvertDelta(eventArgs.Delta));
            _form.Load += (sender, eventArgs) => { _form.Activate(); };
            
            RenderLoop.Run(_form, Render);
        }

        private static Point ConvertToEmotePosition(Point p, Size clientSize)
        {
            var centerX = clientSize.Width / 2;
            var centerY = clientSize.Height / 2;
            return new Point(p.X - centerX, p.Y - centerY);
        }

        private static void form_MouseClick(object sender, MouseEventArgs args)
        {
            //var p = ConvertToEmotePosition(args.Location, _form.ClientSize);
        }

        //For efficiency, you have to use unsafe method for filter
        private static unsafe void GrayTextureFilter(byte* image, uint imageSize)
        {
            while (imageSize > 0)
            {
                //BGRA
                byte gray = (byte)(0.298912f * image[2] + 0.586611f * image[1] + 0.114478f * image[0]);
                image[0] = image[1] = image[2] = gray;
                //image[3] = (byte)(image[3] * 0.5f); //Alpha
                image += 4;
                imageSize -= 4;
            }
            //int p = 0;
            //while (p + 4 < imageSize)
            //{
            //    byte gray = (byte) (0.298912f*image[p + 2] + 0.586611f*image[p + 1] + 0.114478f*image[p]);
            //    image[p] = image[p + 1] = image[p + 2] = gray;
            //    p += 4;
            //}
        }


        private static void Render()
        {
            _elaspedTime += _timer.GetElaspedTime();
            if (_elaspedTime < Refresh)
            {
                //e.Pass();
                Thread.Sleep(5);
                return;
            }

            _e.Update((float)(_elaspedTime * 1000.0));

            _device.Clear(ClearFlags.Target, new RawColorBGRA(0,0,0,0), 1.0f, 0);

            _device.BeginScene();
            //_device.UpdateSurface(_device.GetBackBuffer(0,0),new Surface(new IntPtr(e.D3DSurface)));
            _e.Draw();
            _device.EndScene();
            try
            {
                _device.Present();
            }
            catch (SharpDXException exception)
            {
                if (exception.ResultCode == ResultCode.DeviceLost)
                {
                    Console.WriteLine("Device Lost Detected");
                    _e.OnDeviceLost();
                    Result r;
                    while ((r = _device.TestCooperativeLevel()) == ResultCode.DeviceLost)
                    {
                        Thread.Sleep(5);
                    }
                    r = _device.TestCooperativeLevel();
                    if (r == ResultCode.DeviceNotReset)
                    {
                        _e.D3DReset();
                        _e.OnDeviceReset();
                        //e.D3DInitRenderState();
                    }
                    else
                    {
                        Console.WriteLine(r);
                    }
                    //r = _device.TestCooperativeLevel();
                }
                else
                {
                    throw;
                }
            }

            _elaspedTime = 0;
        }


        private static void FormOnResize(object sender, EventArgs eventArgs)
        {
            var form = (Form)sender;
            int[] margins = { 0, 0, form.Width, form.Height };

            // Extend aero glass style to whole form
            //Util.DwmExtendFrameIntoClientArea(form.Handle, ref margins);

        }

        static void form_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                _leftMouseDown = false;
            }
            if (e.Button == MouseButtons.Right)
            {
                _rightMouseDown = true;
            }
        }

        static void form_MouseMove(object sender, MouseEventArgs args)
        {
            if (_leftMouseDown)
            {
                _player.OffsetCoord(args.X - _lastX, args.Y - _lastY);
                _lastX = args.X;
                _lastY = args.Y;
                //form.Location.Offset(ex.X - lastX, ex.Y - lastY);
            }
            //var p = ConvertToEmotePosition(args.Location, _form.ClientSize);
        }

        static void form_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                _leftMouseDown = true;
                _lastX = e.X;
                _lastY = e.Y;
            }
            if (e.Button == MouseButtons.Right)
            {
                _rightMouseDown = true;
            }
        }

        private static float ConvertDelta(int delta)
        {
            return delta / 120.0f / 50.0f;
        }
    }
}