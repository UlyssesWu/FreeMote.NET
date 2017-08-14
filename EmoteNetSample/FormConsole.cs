using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using FreeMote;

namespace NekoHacks
{
    public partial class FormConsole : Form
    {
        private EmotePlayer _emote;
        private Dictionary<string, Control> _argTracks = new Dictionary<string, Control>();
        private bool _haveWind = false;
        private float _windStart = 0.0f,_windEnd = 1.0f,_windSpeed = 0.0f,_powerMin=0.0f,_powerMax=1.0f;

        public FormConsole(EmotePlayer emotePlayer,string info = "")
        {
            _emote = emotePlayer;
            InitializeComponent();
            if (!string.IsNullOrWhiteSpace(info))
            {
                this.Text = $"NekoConsole - {info}";
            }
            _haveWind = chkWind.Checked;
        }

        private void FormConsole_Load(object sender, EventArgs e)
        {
            ArgTrackBar temp = new ArgTrackBar("Bust",ConvertToInt(_emote.GetBustScale()));
            temp.OnValueChanged += SetBustValue;
            _argTracks.TryAdd("Bust", temp);

            temp = new ArgTrackBar("Hair",ConvertToInt(_emote.GetHairScale()));
            temp.OnValueChanged += SetHairValue;
            _argTracks.TryAdd("Hair", temp);

            temp = new ArgTrackBar("Wind Direction");
            temp.OnValueChanged += SetWindDirect;
            _argTracks.TryAdd("Wind", temp);

            temp = new ArgTrackBar("Wind Speed");
            temp.OnValueChanged += SetWindSpeed;
            _argTracks.TryAdd("WindSpeed", temp);

            UpdatePanel();
        }

        private void UpdatePanel()
        {
            foreach (var argTrackBar in _argTracks)
            {
                if (!flpArg.Controls.Contains(argTrackBar.Value))
                {
                    flpArg.Controls.Add(argTrackBar.Value);
                    argTrackBar.Value.Dock = DockStyle.Top;
                }
            }
        }

        private void UpdateWind()
        {
            if (_haveWind)
            {
                _emote.StartWind(_windStart, _windEnd, _windSpeed, _powerMin, _powerMax);
            }
        }

        private void SetWindSpeed(object sender, ArgTrackBar.TrackEventArgs eventArgs)
        {
            _windSpeed = ConvertPercent(eventArgs.Value);
            UpdateWind();
        }

        private void SetWindDirect(object sender, ArgTrackBar.TrackEventArgs eventArgs)
        {
            if (eventArgs.Value < 50)
            {
                _windStart = ConvertPercent(eventArgs.Value);
                _windEnd = 1.0f;
            }
            else
            {
                _windStart = ConvertPercent(eventArgs.Value);
                _windEnd = 0.0f;
            }
            UpdateWind();
        }

        private void SetHairValue(object sender, ArgTrackBar.TrackEventArgs eventArgs)
        {
            _emote.SetHairScale(ConvertPercent(eventArgs.Value));
        }

        private void SetBustValue(object sender, ArgTrackBar.TrackEventArgs eventArgs)
        {
            _emote.SetBustScale(ConvertPercent(eventArgs.Value));
        }

        private static float ConvertPercent(int value)
        {
            return ((float) value)/100.0f;
        }

        private static int ConvertToInt(float value)
        {
            return (int) (value*100);
        }

        private void chkWind_CheckedChanged(object sender, EventArgs e)
        {
            if (chkWind.Checked)
            {
                _haveWind = true;
                _emote.StartWind(_windStart,_windEnd,_windSpeed,_powerMin,_powerMax);
            }
            else
            {
                _haveWind = false;
                _emote.StopWind();
            }
        }

        private void btnGet_Click(object sender, EventArgs e)
        {
            var count = _emote.CountVariables();
            for (uint i = 0; i < count; i++)
            {
                Console.WriteLine(_emote.GetVariableLabelAt(i));
                ArgTrackBar temp = new ArgTrackBar(_emote.GetVariableLabelAt(i),
                    ConvertToInt(_emote.GetVariable(_emote.GetVariableLabelAt(i))),-4097, 4097);
                temp.OnValueChanged += SetVariable;
                _argTracks.TryAdd(_emote.GetVariableLabelAt(i), temp);
            }
            UpdatePanel();
        }
        
        private void SetVariable(object sender, ArgTrackBar.TrackEventArgs eventArgs)
        {
            _emote.SetVariable(eventArgs.Name,ConvertPercent(eventArgs.Value),0,0);
        }

        private void btnTime_Click(object sender, EventArgs e)
        {
            Console.WriteLine("Playing");
            var count = _emote.CountPlayingTimelines();
            for (uint i = 0; i < count; i++)
            {
                Console.WriteLine(_emote.GetPlayingTimelineLabelAt(i));
                Button btn = new Button()
                {
                    Name = _emote.GetPlayingTimelineLabelAt(i),
                    Text = _emote.GetPlayingTimelineLabelAt(i),
                    Width = 200
                };
                btn.Click += btn_Click;
                _argTracks.TryAdd(_emote.GetPlayingTimelineLabelAt(i), btn);
            }
            Console.WriteLine("Diff");
            count = _emote.CountDiffTimelines();
            for (uint i = 0; i < count; i++)
            {
                Console.WriteLine(_emote.GetDiffTimelineLabelAt(i));
                Button btn = new Button()
                {
                    Name = _emote.GetDiffTimelineLabelAt(i),
                    Text = _emote.GetDiffTimelineLabelAt(i),
                    Width = 200
                };
                btn.Click += btn_Click;
                _argTracks.TryAdd(_emote.GetDiffTimelineLabelAt(i), btn);
            }
            Console.WriteLine("Main");
            count = _emote.CountMainTimelines();
            for (uint i = 0; i < count; i++)
            {
                Console.WriteLine(_emote.GetMainTimelineLabelAt(i));
                Button btn = new Button()
                {
                    Name = _emote.GetMainTimelineLabelAt(i),
                    Text = _emote.GetMainTimelineLabelAt(i),
                    Width = 200
                };
                btn.Click += btn_Click;
                _argTracks.TryAdd(_emote.GetMainTimelineLabelAt(i), btn);
            }
            UpdatePanel();
        }

        void btn_Click(object sender, EventArgs e)
        {
            _emote.PlayTimeline(((Button)sender).Text,TimelinePlayFlags.NONE);
        }

        private void FormConsole_FormClosing(object sender, FormClosingEventArgs e)
        {
        }
    }
}
