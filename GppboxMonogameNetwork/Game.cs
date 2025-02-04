using System;
using System.Collections.Generic;
using System.Text;
using System.Numerics;
using Num = System.Numerics;

using ImGUIExemple;
using ImGuiNET;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace Gppbox
{
    public class Game : Microsoft.Xna.Framework.Game
    {
        private Texture2D _bg;
        private GraphicsDeviceManager _graphics;
        private SpriteBatch _spriteBatch;

        private ImGUIExemple.ImGuiRenderer _imGuiRenderer;
        private Texture2D _monoGameTexture;
        private IntPtr _imGuiTexture;

        private logic.Server _server;
        private logic.Client _client;

        public Game()
        {
            _graphics = new GraphicsDeviceManager(this);
            _graphics.PreferMultiSampling = true;
            _graphics.PreferredBackBufferWidth = 1280;
            _graphics.PreferredBackBufferHeight = 720;
            _graphics.ApplyChanges();

            Content.RootDirectory = "Content";
            IsMouseVisible = true;
        }

        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            _imGuiRenderer = new ImGuiRenderer(this);
            _imGuiRenderer.RebuildFontAtlas();

            base.Initialize();
            _server = new logic.Server();
            _client = new logic.Client();
        }

        protected override void LoadContent()
        {
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            // TODO: use this.Content to load your game content here
            _bg = Content.Load<Texture2D>("bg_stars");

            // First, load the texture as a Texture2D (can also be done using the XNA/FNA content pipeline)
            _monoGameTexture = CreateTexture(GraphicsDevice, 300, 150, pixel =>
            {
                var red = (pixel % 300) / 2;
                return new Color(red, 1, 1);
            });

            _imGuiTexture = _imGuiRenderer.BindTexture(_monoGameTexture);
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Add your update logic here

            base.Update(gameTime);
        }


        // Direct port of the example at https://github.com/ocornut/imgui/blob/master/examples/sdl_opengl2_example/main.cpp
        private float f = 0.0f;

        private bool show_test_window = false;
        private Num.Vector3 clear_color = new Num.Vector3(114f / 255f, 144f / 255f, 154f / 255f);
        private byte[] _textBuffer = new byte[100];
        
       public virtual void ImGuiLayout()
        {
            // 1. Show a simple window
            // Tip: if we don't call ImGui.Begin()/ImGui.End() the widgets appears in a window automatically called "Debug"
            {
                ImGui.Text("Hello, world!");
                ImGui.SliderFloat("float", ref f, 0.0f, 1.0f, string.Empty);
                ImGui.ColorEdit3("clear color", ref clear_color);
                if (ImGui.Button("Test Window")) show_test_window = !show_test_window;
                ImGui.Text(string.Format("Application average {0:F3} ms/frame ({1:F1} FPS)", 1000f / ImGui.GetIO().Framerate, ImGui.GetIO().Framerate));
                ImGui.InputText("Text input", _textBuffer, 100);
                ImGui.Text("Texture sample");
                ImGui.Image(_imGuiTexture, new Num.Vector2(300, 150), Num.Vector2.Zero, Num.Vector2.One, Num.Vector4.One, Num.Vector4.One); // Here, the previously loaded texture is used
            }
          

            // 3. Show the ImGui test window. Most of the sample code is in ImGui.ShowTestWindow()
            if (show_test_window)
            {
                ImGui.SetNextWindowPos(new Num.Vector2(650, 20), ImGuiCond.FirstUseEver);
                ImGui.ShowDemoWindow(ref show_test_window);
            }
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            _imGuiRenderer.BeforeLayout(gameTime);

            ImGuiLayout();


            base.Draw(gameTime);

            _spriteBatch.Begin();
            _spriteBatch.Draw(_bg, new Rectangle(0, 0, _graphics.PreferredBackBufferWidth, _graphics.PreferredBackBufferHeight), new Color(0xffffffff));
            _spriteBatch.End();

            _imGuiRenderer.AfterLayout();

        }

        public static Texture2D CreateTexture(GraphicsDevice device, int width, int height, Func<int, Color> paint)
        {
            //initialize a texture
            var texture = new Texture2D(device, width, height);

            //the array holds the color for each pixel in the texture
            Color[] data = new Color[width * height];
            for (var pixel = 0; pixel < data.Length; pixel++)
            {
                //the function applies the color according to the specified pixel
                data[pixel] = paint(pixel);
            }

            //set the color
            texture.SetData(data);

            return texture;
        }
    }
}
