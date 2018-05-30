README FOR MIM.EXE V3.00 Beta    (C) 2005
Mercury Instant Messenger
By: Dennis Dunn (MasterHD)
_______________________________________________
 CONTENTS:
 1 - What is MIM?
 2 - Requirements
 3 - Installation
 4 - Setting up a connection
 5 - Sending Messages
 6 - Saving IP Addresses
 7 - Finding your computer's IP address
 8 - Pop-up Menu Functions
 9 - Sending Files
 10 - Playing Sounds
 11 - Reference
 12 - Tips
 13 - Troubleshooting
 14 - Advanced Features
 15 - Contact Info
_______________________________________________

1 - WHAT IS MIM?
This program was designed to allow 2 or more computer's with network 
cards or modems (using TCP-IP), to chat with each other by sending text
messages.

-----------------------------------------------------------------------
2 - REQUIREMENTS
This program needs to have TCP-IP running in order to connect computers
together. If a network card is installed, TCP-IP is probably already
working (See section 7 for help with TCP-IP).

	*If using a Modem, you may need to connect to the internet
		before MIM can be used.
	*If using a network card, it must have TCP-IP running.
	*An IP address is required.
	*An ISP(Internet Serveice Provider) is needed for internet
		computers using DHCP.
	
	*System:
		O/S: 		Windows 95/98/2000/Me/XP.
		Speed:		66MHz
		RAM: 		16MB
		Resolution: 	640x480
		Modem: 		14.4K Modem
		(or)NIC: 	10Mbps+

-----------------------------------------------------------------------
3 - INSTALLATION

The first time MIM is opened it will create new files to save your
	settings. You will only be asked to specify the installed
	location of the MIM.exe file.

-----------------------------------------------------------------------
4 - SETTING UP A CONNECTION 
A computer can do one of two different jobs. It can be a SERVER (The
one that creates the chat room and waits for others to join in)or it
can be a "CLIENT" (the one that joins a chat room that has already been
created by a server).

	Here is a step-by-step tutorial to set up a connection:
	
	1. The First step is to select which job your computer will do,
		Server or Client. This is done by clicking the
		"Connection Settings..." button at the top of the
		window. Here you can select your screen name, port, address,
		sounds, etc.
	
	2. The Second step is to enter the IP Address of the Server 
		(See Section 6 on how to save these addresses). An IP
		Address consists of 4 numbers from 0-255 separated by a
		period (EX: 192.168.0.1) or it can be a DNS assigned name.
		To find out what your computer's IP address is, see
		section 7. The Server does not need to enter an IP address.

	3. The third step is to enter the Port Number. This can be any
		integer from 1 to about 20000 (The default is 4000
		because no other programs should be using this port).
		This number specifies which port the application will
		listen on, like a frequency on a radio. A conflict occurs
		if another application is using the same port number.
		
		**NOTE: Always be sure to enter the Port Number of
		the server you are connecting to**
	
	4. The "Connection Timeout" is the number of seconds that the
		client will wait for the server to accept the connection
		request. Usually if a time out occurs, the server cannot
		be found. Be sure the IP address and Port are correct.
	
	5. The next step is to close the "Connection Settings" window
		and press the "Server Options" button. Here you can adjust
		the settings for a chat room that you create (Server).
	
	6. Chatroom Name and Admin Name are the names of the room and admin.
		They can both be up to 29 characters long.
	
	7. The server can also set the maximum number of clients
		allowed on the server (1 - 20).
	
	8. The Port number is the port which the server will be listening
		on for others to join.

	9. Enter a password if you want your room to be protected from
		anonymous users.

	10. The last field is the "Screen Users" box. Checking this will
		notify the admin every time a new user wants to join. The admin
		has the option of accepting or rejecting them.
	
	11. Press OK. Now select File, Save Settings. You are ready to
		begin chatting. To create a chatroom select Server as the type
		and press "Listen". Everyone else should select Client and press "Connect"
		to join that server.
	
-----------------------------------------------------------------------
5 - SENDING MESSAGES
Once a connection has been successfully established, users can send
messages back and forth to each other via the Server. Just type the 
message in the 'Message' box and press 'Send' or the enter key. A
message can be up to 399 characters long.

-----------------------------------------------------------------------
6 - SAVING IP ADDRESSES
A helpful feature that this program offers is the ability to save IP
Addresses. Press the "Connection Settings..." button. This will open up
a new Window listing all of the saved IP Addresses. select an address
to set it as the one that will be used. All addresses will be saved to
the file named 'IP' in the same folder as the program. The address
"loopback" is placed in the list by default (see Troubleshooting
section 11.1 for more info on the loopback address). Remember that
a server which is NOT using a static IP address may not have the same IP
address for more than 24 hours.

NOTE: To save the changes that you have made to any settings, select
File/Save Settings  or  File/Exit. Pressing the close button (the "X"
at the top right corner) will not save any changes.

-----------------------------------------------------------------------
7 - FINDING YOUR COMPUTER'S IP ADDRESS

	MIM should be able to show you your IP address. If it displays
	incorrectly, you can try these steps:
	
	Windows 95/98:
	1. Right-click the "Network Neighborhood" icon on the desktop.
		(If it is not there: Click Start, point to Settings,
		click Control Panel, and then double-click the Network
		icon)
	2. Select the TCP-IP protocol from the list of available
		connections.
		**If you don't have TCP-IP installed, follow these steps:
			a. In the "Network" dialog window, press the "Add"
				button.
			b. Select "Protocol" from the list, and then press
				"Add".
			c. The "Select Network Protocol" window appears.
				Select "Microsoft" from the left-hand list.
			d. In the list on the right, select "TCP/IP" and press
				OK.
			e. Press OK again to close the "Network" window. You
				may have to restart your computer and insert
				the Windows CD.

	3. After selecting TCP-IP from the list, press the "Properties" button.
	4. Select the "IP Address" tab (Skip the Win XP directions, see below
		for further instructions).

	Windows XP:
	1. Click the Start menu.
	2. Select Settings.
	3. Open up Control Panel.
	4. Double-click the "Network Connections" icon.
	5. Right-Click the "Area Network" icon and choose "properties".
	6. Under the "General" tab select the "TCP-IP" protocol from the
		list of available connections.
		**If you don't have TCP-IP installed, follow these steps:
			a. In the "Network" dialog window, press the "Add"
				button.
			b. Select "Protocol" from the list, and then press
				"Add".
			c. "The Select Network Protocol" window appears.
				Select "Microsoft" from the left list.
			d. In the list on the right, select "TCP/IP" and press OK.
			e. Press OK again to close the "Network" window. You may
				have to restart your computer and insert the Windows CD.

	7. After selecting TCP-IP from the list, press the "Properties" button.

	Everyone should now see the "TCP/IP Properties" window
	
	LAN:
	You should now be able to see if your computer has an IP
	address and what it is. If there is no static IP address you
	can specify your own. Usually home computers default to "192.168.0.1"
	as their address.
	
	WAN:
	If you are on an internet network, your computer might not be using a
	static IP address. Most WAN's use DHCP, where a server assigns your
	computer an IP address which you keep for only 24 hours. You must find
	your IP address another way.
	
	Other methods:
	A. Press Start, Run, and type "winipcfg", press OK. Look for IP address.

	B. Start the DOS command prompt and type "ipconfig". Look for address.

	C. Right-Click the network connection on the taskbar and select "status".
		Click the "Support" tab to see the IP address.

	*NOTE: Every computer on the same network must have a unique IP
	address. Also, networked computers may need to be in the same
	"workgroup". To change your workgroup name:

	Windows 95/98
	1. Click Start, point to Settings, click Control Panel, and then
		double-click the Network icon.
	2. Under the "Identification" tab change the workspace name.

	Windows XP
	1. Click Start, point to Settings, click Control Panel, and then
		double-click System.
	2. On the "Computer Name" tab, click "Change".

	Windows uses workgroups to separate private networks that are
	within a leger network.
	
-----------------------------------------------------------------------
8 - Pop-up Menu Functions

The User List Box located to the right of the Message Box displays all
of the users currently connected to the server.
This allows the Server Admin to manually remove users from the chat room
using the "Kick" button. The "Secure Message" button allows the admin to
send just 1 user a secure message.

The Client has the option of ignoring others by selecting the user and
pressing "Ignore User". By pressing it again that user will be unignored.
The icon image next to their names shows whether they are being ignored or
not.

Right-clicking within the user list will show a pop-up menu. The menu
has the common functions for right-clicking a user name: View Profile,
Ignore, Send File, Kick, and Message; and also some other functions: away,
available, and disconnect.

If the icon next to a user name changes to a yellow triangle with an
exclamation mark inside, then that user has either lost connection with
the server or is having connection problems.

The main chatroom message control has the follow url, google, and
dictionary features. Select some text, right click, and select the
option from the menu. The google and dictionary options automatically
enter the selected text into the website and display the results of the
search.

-----------------------------------------------------------------------
9 - Sending Files

Files can be sent between any clients connected to the server and the
admin. Right click the user and press "Send File." Then select the
file(s) and press OK. Or click a user then select multiple files and
drag-and-drop files to the list. Compressing multiple files into ZIP or
RAR before sending them is recommended (and faster).
By default the sender will "listen" for the receiver to accept
the transfer and connect. However, if your computer cannot accept
incoming requests (because of a firewall or router that is blocking the
ports and you cannot resolve the problem) then uncheck the box "Able
to accept calls" in Connection Settings. This will tell the receiver to
listen and accept your call. There is currently no support for streaming
the files through the MIM server, only Peer-To-Peer connections work.
You can have up to 30 simultaneously file streams. Each open transfer
will use a seperate port (starting at the MIM Server connection port 
and incrementing once for each transfer. Ex. If MIM is using port 4000,
the next 3 file transfers will use ports: 4001, 4002, and 4003).

-----------------------------------------------------------------------
10 - Playing Sounds

A new feature on V2.30 is the ability to play sounds when a message is
received. Press "Connection Settings" and enter the name of the file.
Allowable file types are Wave and Midi. If the file is in the same
folder as the MIM.exe program you only need to enter the name and
extension, otherwise you must enter the entire path of the file.

-----------------------------------------------------------------------
11 - Quick Reference

Here is quick explanation of each setting:

Connection Settings:
	1. Server/Client - Choose whether you are Server or Client.
	2. Server Port - Port number of server that you are connecting to.
	3. Screen Name - Your name in the room.
	4. Connection Timeout - Time (in seconds) until connection Timeout.
	5. IP Address - IP Address of server you are connecting to.
	6. Play Sound - Sound that is played whenever a message is received.
	7. Able to accept calls - Your computer's ports are not blocked.

Server Options:
	1. Chatroom Name - Name of chatroom(29 characters).
	2. Admin Name - Name of administrator(29 characters).
	3. Max Users - Maximum number of users allowed to connect.		
	4. Port - port which the server is listening on.
	5. Password - Password required in order to join server.
	6. Screen Users - Ask admin each time, if a new user can join.
	7. Censor Words - Blocks out bad words with asterisks (****)
-----------------------------------------------------------------------
12 - TIPS
Here are a few easy tips for new users:
	1. Make sure that the IP address is correct if you are the
		client. (see section 3.2 "Setting up a connection", for
		more details on the IP address).
	2. The port address entered by the client must be the same as
		the address that the Server is listening on.
	3. Make sure that the modem or connection device has TCP-IP or
		is connected to the internet.
	
-----------------------------------------------------------------------
13 - TROUBLESHOOTING
	1. An easy way to test this program or find a problem, is to
		simulate a connection between two computers, but by only
		using one computer. This can be done by opening the MIM
		program twice so that two applications are showing at
		the same time. Then set one as the server and the other as
		the client. Make sure that both of them have the same Port
		number (EX: 4000) and use "loopback" as the IP address.
		Next click "Listen" for the server and "Connect" for the
		client. You should now be able to send messages back and
		forth between the two applications on just one computer.
	
	2. If the Server cannot "listen" for others then:
		*The socket is in use by another application, try
			another number.
		*The device has not been configured in Windows yet.
		*No connection device exists.
		*A malfunctioning connection device exists.
		*TCP/IP is not installed.

	3. It should only take a few seconds for the client to connect
		to the server. If it takes too long an error message will
		be displayed or the connection will timeout.
		Possible causes:
		*Wrong IP address,
		*Wrong Port number,
		*Server is not listening for clients.

	4. If the "Port may already be in use" error occurs, change the
		port number and try again.		

-----------------------------------------------------------------------
14 - Advanced Features
	Type /COMMANDS to see a list of all the available commands. Then type
	/HELP [command] to see how to use a specific command.

	Parameters:
	[ID] A number from 1-20 representing the UserID of the client.
			Use 0 for admin.
	
	Here are some of the most common actions:

	*Users (Only work for admin or mods)
		/kick [ID]
			Kicks the user specified.
		/ban [ID]
			Bans the user's IP address
		/vote "question" "ans1" "ans2" "ans3" "ans4"
			Displays a vote for all clients.
			Clients type: /vote [#] to choose one.
		/tally
			Tallies all the votes even if not everyone has voted yet.
		/user [ID] [command]
			Enters command into the clients console and runs it.
			This example will let a mod run a vote on the server:
			/user 0 /vote "This will work?" yes no maybe


	*Moderator (remote admin)
		This will allow a client to run admin commands on the server.
		The server can set the MODPASS variable with the console or Server
		Options window. Client will type /MOD [password] to request mod
		status. If password is correct client can type:
			/USER 0 [command]
		The [command] string will be entered and run on the server.

	*Sending Files
	The commands are self explanitory. MIM will save the paths of the files
	into a list and you can use the console to manage them. Dragging and
	dropping files will alter the saved list.
		/addfile [filename]
		/sendfile [ID]
		/clearfile
		/listfile

-----------------------------------------------------------------------
15 - CONTACT INFO
	1. You can E-Mail me at master1hd@gmail.com if you have any questions.
	2. Any comments or suggestions would also be appreciated.

-----------------------------------------------------------------------

