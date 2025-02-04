using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Collections;
using System.Threading;
using System.IO;
using System.Threading.Tasks;
using System.Collections.Generic;
using ImGuiNET;

namespace Gppbox.logic
{
    struct PlayerFrame
    {
        int ms;//time from start of game
        int buttons;//which button is pressed

    }

    public enum State
    {
        Disconnected,
        Connecting,
        Connected,
        Error,
    }

    public class Server
    {
        public State state = State.Disconnected;

        int     port = 10005;
        Socket  socket;

        public class ClientCnct{
            static int guid = 1;

            public bool     connected;
            public Server   srv;
            public int      id;
            public string   name;
            public string   msg;


            public ClientCnct(Server srv)
            {
                this.srv = srv;
                id = guid++;
                StartClient();
            }

            async void StartClient()
            {
                var buffer = new byte[2048];
                var hdl = await srv.socket.AcceptAsync();
                var received = await hdl.ReceiveAsync(buffer, SocketFlags.None);
                if (srv.state == State.Connecting)
                    srv.state = State.Connected;
                var response = Encoding.UTF8.GetString(buffer, 0, received);
                if(response.Length>0)
                {
                    msg = response;
                    Console.WriteLine(response);
                    await hdl.SendAsync(Encoding.UTF8.GetBytes("Bisous!"), 0);
                }
            }
        };

        List<ClientCnct> clients = new List<ClientCnct>();

        public void MkImgui()
        {
            if (socket == null)
            {
                ImGui.Text("Server is disconnected");
                return;
            }
            
            ImGui.Text("Server state is " + state);
            
            if (socket.IsBound)
                ImGui.Text("Socket is bound to "+port);
            if( state == State.Connecting)
            {
                ImGui.Text("No one connected, waiting for connections");
            }
            else if (state == State.Connected)
            {
                ImGui.Text("Some one connected, waiting for communication");
            }

            //test socket avail?
            foreach( var c in clients)
            {
                if (c.connected)
                {
                    ImGui.Text("client " + c.id + " is connected");
                    ImGui.Text("latest client msg " + c.msg);
                }
            }
            if (socket.LocalEndPoint != null)
            {
                ImGui.Text( "local:"+socket.LocalEndPoint.ToString() ); 
            }

            if (socket.RemoteEndPoint != null)
            {
                ImGui.Text("remote:" + socket.RemoteEndPoint.ToString());
            }
        }

        public void ClearSocket()
        {
            if (socket != null)
                if (socket.IsBound)
                {
                    socket.Close();
                }
        }

        public void update(double dt){

            switch (state)
            {
                case State.Error:
                    ClearSocket();
                    state = State.Disconnected;
                    break;

                case State.Disconnected:
                    ClearSocket();

                    socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                    try
                    {
                        state = State.Connecting;
                        var host = Dns.GetHostName();
                        IPHostEntry ipHostEntry = Dns.GetHostEntry(host);
                        int naddr = 0;
                        while (ipHostEntry.AddressList[naddr].AddressFamily != AddressFamily.InterNetwork)
                        {
                            naddr++;
                            if (naddr == ipHostEntry.AddressList.Length)
                            {
                                socket.Close();
                                socket = null;
                                return;
                            }
                        }

                        var addr = ipHostEntry.AddressList[naddr];
                        socket.Bind(new IPEndPoint(addr, 8000));
                        int maxClients = 10;
                        socket.Listen(10);
                        state = State.Connecting;
                        
                        for(int i = 0; i < maxClients; ++i)
                        {
                            clients.Add(new ClientCnct(this));
                        }

                    }
                    catch (Exception e)
                    {
                        state = State.Error;
                    }
                    break;
            }

        }

    }

}
