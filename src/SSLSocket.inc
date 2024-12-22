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
    Procedure SSLPrintError;

    Class Procedure SSLInit;
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
      SSLPrintError;

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
    Begin
      WriteLn('[ERROR][SSLSocket] An error occurred while sending data over the secure connection');
      SSLPrintError;
    End
  Else
    WriteLn('[DEBUG][SSLSocket] ', Result, ' bytes sent over the secure connection');
{$ENDIF}
End;

Function TSSLSocket.Read(Var Buffer: TByteArray; BufferSize: LongWord): LongInt;
Begin
  Result := SSLRead(SSL, @Buffer[0], BufferSize);

{$IFDEF DEBUG}
  If Result < 0 Then
    Begin
      WriteLn('[ERROR][SSLSocket] An error occurred while receiving data over the secure connection');
      SSLPrintError;
    End
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
  SSLMethod: PSSL_METHOD;
Begin
  SSLMethod := SSLMethodTLSV1_2;

  If SSLMethod = Nil Then
    Begin
{$IFDEF DEBUG}
      WriteLn('[ERROR][SSLSocket] SSL Method was NULL');
{$ENDIF}
      SSLPrintError;

      raise Exception.Create('Cannot get the required SSL method, consider using another OpenSSL version: https://github.com/IndySockets/OpenSSL-Binaries');
    End;

  Context := SSLCtxNew(SSLMethod);

  If Context = Nil Then
    Begin
{$IFDEF DEBUG}
      WriteLn('[ERROR][SSLSocket] SSL Context is nil');
{$ENDIF}
      SSLPrintError;

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

Procedure TSSLSocket.SSLPrintError;
Var
  Buffer: String;
Const
  BufferSize: LongWord = 1024;
begin
  SetLength(Buffer, BufferSize);

  ErrErrorString(ErrGetError, Buffer, BufferSize);

  WriteLn(Buffer);
end;

Class Procedure TSSLSocket.SSLInit;
Begin
  SSLLibraryInit;

  OPENSSL_add_all_algorithms;
  
  SSLLoadErrorStrings;

  InitSSLInterface;

{$IFDEF DEBUG}
  If SSLMethodTLSV1_2 = Nil Then WriteLn('[DEBUG][SSLSocket] TLS v1.2 was Nil');
  If SSLMethodTLSV1_1 = Nil Then WriteLn('[DEBUG][SSLSocket] TLS v1.1 was Nil');
  If SSLMethodTLSV1 = Nil Then WriteLn('[DEBUG][SSLSocket] TLS v1 was Nil');
  If SSLMethodV3 = Nil Then WriteLn('[DEBUG][SSLSocket] SSL Method v3 was Nil');
  If SSLMethodV23 = Nil Then WriteLn('[DEBUG][SSLSocket] SSL Method v2.3 was Nil');
  If SSLMethodV2 = Nil Then WriteLn('[DEBUG][SSLSocket] SSL Method v2 was Nil');
{$ENDIF}
End;

{$ENDIF}
{$IFDEF INITIALIZATION}

TSSLSocket.SSLInit;

{$ENDIF}