using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using TDx.TDxInput;

namespace TestFormApp
{
    public partial class Form1 : Form
    {
        private  Sensor _sensor;
        private  Device _device=null;
        private  Keyboard _keyboard;
            
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (_device == null)
                {
                    _device = new Device(); // Class();
                    _sensor = _device.Sensor;
                    _keyboard = _device.Keyboard;
                }
                if (!_device.IsConnected)
                    _device.Connect();
            }
            catch (COMException ex)
            {
               Console.WriteLine("error:{0}",ex.ToString());

            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Console.WriteLine("{0} {1} {2}",
            (float)_sensor.Translation.X,
            (float)_sensor.Translation.Y,
            (float)_sensor.Translation.Z
            
            );

        }
    }
}
