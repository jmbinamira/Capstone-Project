using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using TagLib;

namespace SoftwareDesign
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
            this.FormClosed += new FormClosedEventHandler(form_OnClose);
        }

        string music_path;

        public void preview_swap()
        {
            label1.Text = "";
            List<string> data = new List<string>();

            var music_file = TagLib.File.Create(music_path);

            string name = music_file.Name;
            int bitrate = music_file.Properties.AudioBitrate;
            int channels = music_file.Properties.AudioChannels;
            int samplerate = music_file.Properties.AudioSampleRate;
            int bitsample = music_file.Properties.BitsPerSample;
            string desc = music_file.Properties.Description;
            TimeSpan duration = music_file.Properties.Duration;

            string[] artists;
            string[] albumartists;
            string[] genres;
            string subtitle = music_file.Tag.Subtitle;
            string title;
            string album;
            string lyrics;
            string comment;

            if (button3.Text == "Preview New Changes")
            {
                title = textBox1.Text;
                album = textBox2.Text;
                albumartists = music_file.Tag.AlbumArtists;
                lyrics = textBox6.Text;
                comment = textBox5.Text;

                foreach (string artist in textBox4.Text.Split(','))
                {
                    data.Add(artist);
                }
                artists = data.ToArray();
                data.Clear();

                foreach (string genre in textBox3.Text.Split(','))
                {
                    data.Add(genre);
                }
                genres = data.ToArray();
                data.Clear();

                button3.Text = "Preview Original";
            }

            else
            {
                title = music_file.Tag.Title;
                subtitle = music_file.Tag.Subtitle;
                artists = music_file.Tag.Performers;
                album = music_file.Tag.Album;
                albumartists = music_file.Tag.AlbumArtists;
                genres = music_file.Tag.Genres;
                lyrics = music_file.Tag.Lyrics;
                comment = music_file.Tag.Comment;

                button3.Text = "Preview New Changes";
            }

            label1.Text += $"Duration: {duration}\n";
            label1.Text += $"Bitrate: {bitrate}\n";
            label1.Text += $"Channels: {channels}\n";
            label1.Text += $"Sample Rate: {samplerate}\n";
            label1.Text += $"Bit Sample: {bitsample}\n";
            label1.Text += $"Description: {desc}\n";

            if (title != null)
            {
                label1.Text += $"\nTitle: {title}";
            }

            if (subtitle != null)
                label1.Text += $"\nSubtitle: {subtitle}";

            if (artists.Length != 0)
            {
                label1.Text += $"\nArtist: ";
                foreach (string people in artists)
                {
                    label1.Text += $" {people},";
                }
                label1.Text = label1.Text.Remove(label1.Text.Length - 1);
            }


            if (album != null)
            {
                label1.Text += $"\nAlbum Name: {album}";
            }

            if (albumartists.Length != 0)
            {
                label1.Text += $"\nAlbum Artist: ";
                foreach (string people in albumartists)
                {
                    label1.Text += $" {people},";
                }
                label1.Text = label1.Text.Remove(label1.Text.Length - 1);
            }

            if (genres.Length != 0)
            {
                label1.Text += $"\nGenre/s: ";
                foreach (string genre in genres)
                {
                    label1.Text += $" {genre},";
                }
                label1.Text = label1.Text.Remove(label1.Text.Length - 1);
            }

            if (lyrics != null)
            {
                label1.Text += $"\nLyrics: {lyrics}";
            }

            if (comment != null)
            {
                label1.Text += $"\nComment: {comment}";
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog file = new OpenFileDialog();
            
            if (file.ShowDialog() == DialogResult.OK)
            {
                music_path = file.FileName;
                preview_swap();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (music_path != null)
            {
                preview_swap();
            }

            MessageBox.Show("Your metadata has been saved on the selected file.\n Thanks for using our program!");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (music_path != null)
            {
                preview_swap();
            }
        }
        void form_OnClose(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }
    }
}
