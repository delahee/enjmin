using ImGUIExemple;
using ImGuiNET;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System;
using Num = System.Numerics;

namespace Gppbox
{
    public class GameClient : Gppbox.Game
    {

        private logic.ServerCnct _serverCnct;
        private logic.Client _client;

        public GameClient()
        {
            _serverCnct = new logic.ServerCnct();
            _client = new logic.Client();

        }

    }
}