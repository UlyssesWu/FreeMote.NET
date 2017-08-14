using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NekoHacks
{
    public partial class ArgTrackBar : UserControl
    {
        public class TrackEventArgs:EventArgs
        {
            public TrackEventArgs(int value,string name = "")
            {
                Value = value;
                Name = name;
            }
            public int Value = 0;
            public string Name = "";
        }
        public delegate void ValueChangedHandler(object sender,TrackEventArgs eventArgs);
        public event ValueChangedHandler OnValueChanged = null;
        //public event 
        public ArgTrackBar(string name,int value = 0,int min = 0,int max = 100)
        {
            InitializeComponent();
            this.Name = name;
            lblName.Text = name;
            lblValue.Text = tracker.Value.ToString();
            
            tracker.Maximum = max;
            tracker.Minimum = min;
            if (value >= min && value <= max)
            {
                tracker.Value = value;
            }
        }

        public void SetValue(int value)
        {
            tracker.Invoke(new MethodInvoker(() => { tracker.Value = value; }));
        }

        private void tracker_ValueChanged(object sender, EventArgs e)
        {
            lblValue.Text = tracker.Value.ToString();
            if (OnValueChanged != null)
            {
                OnValueChanged(this, new TrackEventArgs(tracker.Value,this.Name));
            }
        }
    }
}
