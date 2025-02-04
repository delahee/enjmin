using ImGUIExemple;
using ImGuiNET;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using Num = System.Numerics;

namespace Gppbox
{
    public class GameServer : Gppbox.Game
    {

        private logic.Server _server;
        private logic.Client _client;

        public GameServer()
        {
            _server = new logic.Server();   
            _client = new logic.Client();   

        }

    }
}
