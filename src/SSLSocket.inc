// SSLSocket Class =======================================
{$IFDEF INTERFACE}

Type TSSLSocket = Class(TSocket)
  Private 
    SSLContext: PSSL_CTX;
    SSL: PSSL;
    
    Function SSLInitCtx: PSSL_CTX;
{$IFDEF DEBUG}
    Procedure SSLPrintCerts;
{$ENDIF}
  Public 
    // Creates a secure socket and attempts to open a secure connection
    // with the given host at the given port.
    //
    constructor Create(RemoteHost: String; RemotePort: Word);

    destructor Destroy;
    override;

    Function Write(Const Data: String): LongInt;
    override;

    Function Read(Var Buffer: TByteArray; BufferSize: LongWord): LongInt;
    override;

    Function ReadLn: String;
    override;
End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor TSSLSocket.Create(RemoteHost: String; RemotePort: Word);
Begin
  inherited Create(RemoteHost, RemotePort);

  SSLContext := SSLInitCtx;
  SSL := SSLNew(SSLContext);

  SSLSetFd(SSL, Sock);

  If SSLConnect(SSL) = -1 Then
    Begin
{$IFDEF DEBUG}
      WriteLn('[ERROR][SSLSocket] Error in SSLConnect()');
{$ENDIF}
      raise Exception.Create('Error connecting to host via secure connection');
    End;

{$IFDEF DEBUG}
  WriteLn('[DEBUG][SSLSocket] Using encryption: ', SSLCipherGetName(SSLGetCurrentCipher(SSL)));

  SSLPrintCerts;
{$ENDIF}
End;

destructor TSSLSocket.Destroy;
Begin
  SSLFree(SSL);

  CloseSocket(Sock);

  SSLCtxFree(SSLContext);
End;

Function TSSLSocket.Write(Const Data: String): LongInt;
Begin
  Result := SSLWrite(SSL, PChar(Data), Length(Data));

{$IFDEF DEBUG}
  If Result < 0 Then
    WriteLn('[ERROR][SSLSocket] An error occurred while sending data over the secure connection');
  Else
    WriteLn('[DEBUG][SSLSocket] ', Result, ' bytes sent over the secure connection');
{$ENDIF}
End;

Function TSSLSocket.Read(Var Buffer: TByteArray; BufferSize: LongWord): LongInt;
Begin
  Result := SSLRead(SSL, @Buffer[0], BufferSize);

{$IFDEF DEBUG}
  If Result < 0 Then
    WriteLn('[ERROR][SSLSocket] An error occurred while receiving data over the secure connection');
  Else
    WriteLn('[DEBUG][SSLSocket] ', Result, ' bytes received over the secure connection');
{$ENDIF}
End;

Function TSSLSocket.ReadLn: String;
Var 
  RawDataLine: String = '';
  CurrentChar: Char = #0;
  LastChar: Char = #0;
Begin
  Repeat
    LastChar := CurrentChar;
    SSLRead(SSL, @CurrentChar, 1);
    RawDataLine := RawDataLine + CurrentChar;
  Until (LastChar + CurrentChar) = EOL;

  Result := RawDataLine;
End;

Function TSSLSocket.SSLInitCtx: PSSL_CTX;

Var 
  Context: PSSL_CTX;
Begin
  OPENSSL_add_all_algorithms;

  SSLLoadErrorStrings;

  Context := SSLCtxNew(SSLMethodTLSV1_2);

  If Context = Nil Then
    Begin
{$IFDEF DEBUG}
      WriteLn('[ERROR][SSLSocket] SSL Context is nil');
{$ENDIF}
      raise Exception.Create('Cannot create a new SSL context');
    End;

  Result := Context;
End;

{$IFDEF DEBUG}
Procedure TSSLSocket.SSLPrintCerts;

Var 
  Cert: pX509;
  Line, TempLine: String;

Const 
  TempSize: LongWord = 512;
Begin
  Cert := SSLGetPeerCertificate(SSL);
  If Cert <> Nil Then
    Begin
      SetLength(TempLine, TempSize);
      WriteLn('[DEBUG][SSLSocket] Server certificates');

      Line := X509NameOneline(X509GetSubjectName(Cert), TempLine, TempSize);
      WriteLn('[DEBUG][SSLSocket] Subject (Line): ', Line);

      Line := X509NameOneline(X509GetIssuerName(Cert), TempLine, TempSize);
      WriteLn('[DEBUG][SSLSocket] Issuer (Line): ', Line);

      X509Free(Cert);
    End
  Else
    WriteLn('[DEBUG][SSLSocket] No client certificates configured.');
End;
{$ENDIF}

{$ENDIF}
