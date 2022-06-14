using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using HIDLib;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        cHIDLib miHID = new cHIDLib();

        Timer init = new Timer();   // timer de inicialización
        Timer timer = new Timer();  // declaro un timer para temporizar la graficación
        Timer tusb = new Timer();   // timer de adquisición de datos de USB
        
        public Form1()
        {
            InitializeComponent();

            //inicializacion de los timers

            init.Tick += new EventHandler(init_Tick);
            init.Interval = (10) * (5);
            init.Enabled = true;
            init.Start();

            timer.Tick += new EventHandler(timer_Tick);
            timer.Interval = (10) * (2);
            timer.Enabled = false;

            tusb.Tick += new EventHandler(tusb_Tick);
            tusb.Interval = (100) * (5);
            tusb.Enabled = false;
                       
        }

        public static class Var
        {
            
            public static int[] X = new int[61];    //vector eje temporal
            public static int[] Y = new int[61];    //vector de valores de la señal para graficar (ver double D[])
            public static int C = 0, T=0, A=100; //C es para actualizar valores de pulsaciones y spO2 // F es el divisor de la frecuencia // A es el valor de la amplitud
            public static double F = 100;
            public static float SP = 0;             //SP es el valor de spO2(es flotante por los decimales)
            public static double[] D = new double[61];  //vector de valores calculados de la señal, segun la ecuacion
                      
        }

        void init_Tick(object sender, EventArgs e)  //búsqueda automatica de dispositivo al iniciar
        {
            if (Var.T < 105)
            {
                if (Var.T < 100)
                    BuscarDev();
                else
                    Var.T++;
            }            
        }

        void timer_Tick(object sender, EventArgs e)
        {
            Valores();    //generacion de valores para graficar
            Dibujar();  //dibuja la señal
                        
            if (Var.C == 25)       //los numeros en el display de spO2 y pulso se actualizan con menor frecuencia para que se lean mejor
            {
                textBox1.Clear();
                textBox1.Text += Var.SP.ToString();
                textBox2.Clear();
                textBox2.Text += Convert.ToInt16(9000/Var.F);
                
                Var.C = 0;
            }
            else
                Var.C++;

        }

        void tusb_Tick(object sender, EventArgs e)  //búsqueda automatica de dispositivo al iniciar
        {
            Datos();
        }

        void BuscarDev()    // busca un dispositivo acoplado
        {
            short vid = short.Parse("1111", System.Globalization.NumberStyles.HexNumber);
            short pid = short.Parse("2222", System.Globalization.NumberStyles.HexNumber);

            label3.Text = "Buscando...";
            bool encontrado = miHID.hidBuscar(pid, vid);

            if (encontrado)
            {
                label3.Text = "Encontrado.";
                timer.Start();
                tusb.Start();
                init.Stop();
            }       // si encuentra el dispositivo inicia el timer de trabajo y detiene el de inicialización
            else
                label3.Text = "No encontrado.";
        }

        void Dibujar()
        {
            Bitmap b;           //creo un bitmap

            b = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            pictureBox1.Image = (Image)b;
            Graphics g;
            g = Graphics.FromImage(b);      //inicializo el bitmap
           
            Pen rojo = new Pen(Color.Red, 3);
            Pen nada = new Pen(Color.Transparent);      //dos lapices, uno rojo y otro sin color para no graficar una linea de una punta a la otra
            
            for (int i = 0; i < 58; i++)            //lazo que dibuja los puntos en la gráfica
            {
                if (Var.X[i] >= 300)
                    g.DrawLine(nada, new Point(Var.X[i], 150-Var.Y[i]), new Point(Var.X[i+1], 150-Var.Y[i+1]));
                else
                    g.DrawLine(rojo, new Point(Var.X[i], 150-Var.Y[i]), new Point(Var.X[i + 1], 150-Var.Y[i + 1]));
                           
            }
         }

        void Valores()        //los valores de los puntos necesarios para hacer el grafico
        {
            for (int i = 0; i < 60; i++)
            {
                Var.X[i] = Var.X[i + 1];
                Var.Y[i] = Var.Y[i + 1];
                
                if (Var.X[i + 1] >= 300)    //desplazamiento del eje de tiempo
                    Var.X[i + 1] = 0;
                else
                    Var.X[i + 1] = Var.X[i + 1] + 5;
                        
                //valores de la señal para graficar
                    Var.D[i + 1] = (0.05 * (Math.Sin((2 * Math.PI * 3 / Var.F) * Var.X[i + 1])) +
                                        0.6 * (Math.Sin((2 * Math.PI / Var.F) * Var.X[i + 1])) +
                                        0.25 * (Math.Sin(((2 * Math.PI * 2 / Var.F) * Var.X[i + 1]) + 45))) * Var.A + 50;
                    Var.Y[i + 1] = (int)(Var.D[i + 1]);               
            }
        }

        void Datos()
        {
            byte[] Trama;
            do
                {
                    Trama = miHID.hidLeer();
                    Application.DoEvents();
                } while (Trama == null);

                foreach (byte b in Trama)
                {
                    textBox3.Text += " " + b;
                }
        
                Var.F =  9000 / Convert.ToInt16(Trama[3]) ;
//                Var.A = Trama[1] ;
                Var.SP = Trama[2];

              
            
        }
        
        private void pictureBox1_Click(object sender, EventArgs e)
        {
            
        }
        
        private void label1_Click(object sender, EventArgs e)
        {
            
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
          
        }

        private void button3_Click(object sender, EventArgs e)
        {
            BuscarDev();  // búsqueda manual del dispositivo si no se encuentra al finalizar el tiempo inicial
        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            
        }

       
    }
}
