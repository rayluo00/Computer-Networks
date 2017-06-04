IGMP Multicasting Maze Program

Author: Raymond Weiming Luo

A multiplayer maze game that utilizes IGMP multicasting to send out
the status of the maze and the player's location once a player has
made a move. The player may choose to use the following commands:
    up   : Move up
    down : Move down
    left : Move left
    right: Move right

Users can only move to open spaces and will start at 'S' represented
as the starting place in the maze. The objectvie is to reach to 'E'
represented as the ending place in the maze. Each client and server
will be on their own terminal session.

Instructions to start program:
    1. Compile program using Makefile command, 'make'.
    2. Run server: 
            ./svr [IP ADDRESS OF LOCAL NETWORK] [GROUP IGMP PORT] [CLIENT 1 TCP PORT] [CLIENT 2 TCP PORT]
    3. Run client 1:
            ./clnt [IP ADDRESS OF LOCAL NETWORK] [GROUP IGMP PORT] [CLIENT 1 TCP PORT]
    4. Run client 2:
            ./clnt [IP ADDRESS OF LOCAL NETWORK] [GROUP IGMP PORT] [CLIENT 2 TCP PORT]
    5. Either client 1 or client 2 can write commands to stdin.
