// SslSocket Class =======================================
{$IFDEF INTERFACE}

type
    TSslSocket = Class(TSocket)
        private
            SslContext: PSSL_CTX;
            Ssl: PSSL;
            function SslInitCtx: PSSL_CTX;
            {$IFDEF DEBUG}
            procedure SslPrintCerts;
            {$ENDIF}
        public
            constructor Create(RemoteHost: String; RemotePort: Word);
            procedure Write(const RawData: String); override;
            function Read: String; override;
            function Readln: String; override;
    End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor TSslSocket.Create(RemoteHost: String; RemotePort: Word);
begin
    inherited Create(RemoteHost, RemotePort);

    SslContext := SslInitCtx;
    Ssl := SslNew(SslContext);

    SslSetFd(Ssl, Sock);
    if SslConnect(Ssl) = -1 then
    begin
        {$IFDEF DEBUG}
        Writeln('Error in SslConnect()');
        {$ENDIF}
        Halt(-1);
    end;

    {$IFDEF DEBUG}
    Writeln('Using encryption: ', SSLCipherGetName(SSLGetCurrentCipher(Ssl)));
    SslPrintCerts;
    {$ENDIF}
end;

procedure TSslSocket.Write(const RawData: String);
var
    Bytes: LongWord;
begin
    Bytes := SslWrite(Ssl, PChar(RawData), Length(RawData));
    {$IFDEF DEBUG}
    Writeln(Bytes, ' ssl bytes sent.');
    {$ENDIF}
end;

function TSslSocket.Read: String;
var
    RawData: String = '';
    BufStr: String = '';
    Buf: Array[0..1023] of Char;
    Bytes: LongInt;
const
    BufLen: LongWord = 1024;
begin
    //fpShutdown(Sock, 1);

    repeat
        FillChar(Buf, BufLen, 0);
        Bytes := SslRead(Ssl, @Buf[0], BufLen);
        {$IFDEF DEBUG}
        Writeln(Bytes, ' ssl bytes read.');
        {$ENDIF}
        SetString(BufStr, PChar(@Buf[0]), Bytes);
        RawData := RawData + BufStr;
    until Bytes <= 0;

    SslFree(Ssl);
    CloseSocket(Sock);
    SslCtxFree(SslContext);

    Result := RawData;
end;

function TSslSocket.Readln: String;
var
    RawDataLine: String;
    CurrentChar, LastChar: Char;
begin
    Repeat
        LastChar := CurrentChar;
        SslRead(Ssl, @CurrentChar, 1);
        RawDataLine := RawDataLine + CurrentChar;
    Until (LastChar + CurrentChar) = EOL;

    Result := RawDataLine;
end;

function TSslSocket.SslInitCtx: PSSL_CTX;
var
    Context: PSSL_CTX;
begin
    OPENSSL_add_all_algorithms;
    SslLoadErrorStrings;
    Context := SslCtxNew(SslMethodTLSV1_2);

    if Context = nil then
    begin
        {$IFDEF DEBUG}
        Writeln('SSL Context is nil');
        {$ENDIF}
        Halt(-1);
    end;

    Result := Context;
end;

{$IFDEF DEBUG}
procedure TSslSocket.SslPrintCerts;
var
    Cert: pX509;
    Line, TempLine: String;
const
    TempSize: LongWord = 512;
begin
    Cert := SslGetPeerCertificate(Ssl);
    if Cert <> nil then
    begin
        SetLength(TempLine, TempSize);
        Writeln('Server certificates');

        Line := X509NameOneline(X509GetSubjectName(Cert), TempLine, TempSize);
        Writeln('Subject (Line): ', Line);

        Line := X509NameOneline(X509GetIssuerName(Cert), TempLine, TempSize);
        Writeln('Issuer (Line): ', Line);

        X509Free(Cert);
    end
    else
        Writeln('No client certificates configured.');
end;
{$ENDIF}

{$ENDIF}
