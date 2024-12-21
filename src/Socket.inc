// Socket Class ==========================================
{$IFDEF INTERFACE}

Type TSocket = Class
  Protected 
    Sock: LongInt;
    RemoteIP: String;
    Procedure GetRemoteIP(HostName: String);
  Public 
    // Creates a new socket and opens a connection to the desired host
    // and port.
    //
    constructor Create(RemoteHost: String; RemotePort: Word);

    // Destroys the current socket and finishes the connection.
    //
    destructor Destroy;
    override;

    // Writes the given string into the buffer and returns the number of
    // bytes written.
    //
    Function Write(Const Data: String): LongInt;
    virtual;

    // Reads N bytes from the socket into the buffer and retrieves the
    // number of bytes read.
    //
    // If an error occurs, the function returns a < 0 value.
    //
    Function Read(Var Buffer: TByteArray; BufferSize: LongWord): LongInt;
    virtual;

    // Retrieves a single line terminated by '\r\n'
    //
    Function ReadLn: String;
    virtual;

    // Sets the socket to read-only mode.
    //
    // This means that the client has been sent all the needed data to server.
    //
    Procedure FinishSending;
End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor TSocket.Create(RemoteHost: String; RemotePort: Word);

Var 
  SockAddr: TSockAddr;
Begin
  GetRemoteIP(RemoteHost);

  Sock := fpSocket(AF_INET, SOCK_STREAM, 0);

  If Sock = -1 Then
    Begin
  {$IFDEF DEBUG}
      Writeln('[ERROR][Socket] Error in fpSocket()');
  {$ENDIF}
      raise Exception.Create('Error creating the socket');
    End;

  SockAddr.sin_family := AF_INET;
  SockAddr.sin_port := HToNS(RemotePort);
  SockAddr.sin_addr := StrToNetAddr(RemoteIP);

  If fpConnect(Sock, @SockAddr, SizeOf(SockAddr)) = -1 Then
    Begin
{$IFDEF DEBUG}
      Writeln('[ERROR][Socket] Error in fpConnect()');
{$ENDIF}
      raise Exception.Create('Error connecting to the remote host');
    End;
End;

destructor TSocket.Destroy;
Begin
  CloseSocket(Sock);
End;

Function TSocket.Write(Const Data: String): LongInt;
Begin
  Result := fpSend(Sock, PChar(Data), Length(Data), 0);

{$IFDEF DEBUG}
  If Result < 0 Then
    WriteLn('[ERROR][Socket] Error while writting to socket');
  Else
    Writeln('[DEBUG][Socket]', Result, ' bytes written');
{$ENDIF}
End;

Function TSocket.Read(Var Buffer: TByteArray; BufferSize: LongWord): LongInt;
Begin
  Result := fpRecv(Sock, @Buffer[0], BufferSize, 0);

{$IFDEF DEBUG}
  If Result < 0 Then
    WriteLn('[ERROR][Socket] Error while reading from socket');
  Else
    WriteLn('[DEBUG][Socket]', Result, ' bytes read');
{$ENDIF}
End;

Function TSocket.ReadLn: String;
Var 
  DataLine: String = '';
  CurrentChar: Char = #0;
  LastChar: Char = #0;
Begin
  Repeat

    LastChar := CurrentChar;

    fpRecv(Sock, @CurrentChar, 1, 0);

    DataLine := DataLine + CurrentChar;

  Until (LastChar + CurrentChar) = EOL;

  Result := DataLine;
End;

Procedure TSocket.GetRemoteIP(HostName: String);

Var 
  Host: THostResolver;
Begin
  Host := THostResolver.Create(Nil);

  If Host.NameLookup(HostName)
    Then RemoteIP := Host.AddressAsString
  Else RemoteIP := '127.0.0.1';

  Host.Free;
End;

Procedure TSocket.FinishSending;
Begin
  fpShutdown(Sock, 1);
End;

{$ENDIF}
