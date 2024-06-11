// Socket Class ==========================================
{$IFDEF INTERFACE}

type
    TSocket = Class
        protected
            Sock: LongInt;
            RemoteIP: String;
            procedure GetRemoteIP(HostName: String);
        public
            constructor Create(RemoteHost: String; RemotePort: Word);
            procedure Write(const RawData: String); virtual;
            function Read: String; virtual;
            function Readln: String; virtual;
    End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor TSocket.Create(RemoteHost: String; RemotePort: Word);
var
    SockAddr: TSockAddr;
begin
    GetRemoteIP(RemoteHost);

    Sock := fpSocket(AF_INET, SOCK_STREAM, 0);

    if Sock = -1 then
    begin
        {$IFDEF DEBUG}
        Writeln('Error in fpSocket()');
        {$ENDIF}
        Halt(-1);
    end;

    SockAddr.sin_family := AF_INET;
    SockAddr.sin_port := HToNS(RemotePort);
    SockAddr.sin_addr := StrToNetAddr(RemoteIP);

    if fpConnect(Sock, @SockAddr, SizeOf(SockAddr)) = -1 then
    begin
        {$IFDEF DEBUG}
        Writeln('Error in fpConnect()');
        {$ENDIF}
        Halt(-1);
    end;
end;

procedure TSocket.Write(const RawData: String);
var
    Bytes: LongInt;
begin
    Bytes := fpSend(Sock, PChar(RawData), Length(RawData), 0);
    {$IFDEF DEBUG}
    Writeln(Bytes, ' bytes sent.');
    {$ENDIF}
end;

function TSocket.Read: String;
var
    RawData: String = '';
    BufStr: String = '';
    Buf: Array[0..1023] of Char;
    Bytes: LongInt;
const
    BufLen: LongWord = 1024;
begin
    //fpShutdown(Sock, 1);

    Repeat
        FillChar(Buf, BufLen, 0);
        Bytes := fpRecv(Sock, @Buf[0], BufLen, 0);
        {$IFDEF DEBUG}
        Writeln(Bytes, ' bytes read.');
        {$ENDIF}
        SetString(BufStr, PChar(@Buf[0]), Bytes);
        RawData := RawData + BufStr;
    Until Bytes <= 0;

    CloseSocket(Sock);

    Result := RawData;
end;

function TSocket.Readln: String;
var
    RawDataLine: String;
    CurrentChar, LastChar: Char;
begin
    Repeat
        LastChar := CurrentChar;
        fpRecv(Sock, @CurrentChar, 1, 0);
        RawDataLine := RawDataLine + CurrentChar;
    Until (LastChar + CurrentChar) = EOL;

    Result := RawDataLine;
end;

procedure TSocket.GetRemoteIP(HostName: String);
var
    Host: THostResolver;
begin
    Host := THostResolver.Create(nil);

    if Host.NameLookup(HostName)
        then RemoteIP := Host.AddressAsString
        else RemoteIP := '127.0.0.1';

    Host.Free;
end;

{$ENDIF}
