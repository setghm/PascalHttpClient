// HttpResponse Class ====================================
{$IFDEF INTERFACE}

Type THttpResponse = Class(THttpMessage)
  Private 
    FStatusCode: Word;
    FStatusText: String;
    
    Procedure ParseString(Const RawData: String);
    Procedure ParseCookieString;
  Public 
    // The status code of this response.
    //
    property StatusCode: Word Read FStatusCode;

    // The status text of this response.
    //
    property StatusText: String Read FStatusText;

    // Creates a new response from the specified Socket.
    //
    // To correctly parse the HTTP response, the socket must be
    // in read-only mode and no bytes must have been read.
    //
    constructor Create(Sock: TSocket);

    // Gets the value of the specified cookie.
    // 
    // If the cookie doesn't exists, an empty string is returned.
    //
    Function Cookie(Key: String): String;

    // Gets the value of the specified header.
    // 
    // If the header doesn't exists, an empty string is returned.
    //
    Function Header(Name: String): String;

    //  Reads N bytes from the incoming data.
    //  
    //  The content is a stream, so each byte can be read once.
    //
    Function ReadContentChunk(BytesToRead: LongWord): String;

    //  Reads the whole content into the specified file.
    //
    //  Returns true if the content was read, false otherwise.
    //
    Function ReadContentToFile(FileName: String): Boolean;

    // Reads the whole content and returns it as a string representation.
    //
    // If there are no content, an empty string is returned.
    //
    Function ReadContentAsString: String;
End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor THttpResponse.Create(Sock: TSocket);
Var
  Data: String = '';
  DataLine: String;
Begin
  FSock := Sock;

  Repeat
    DataLine := FSock.ReadLn;
    Data := Data + DataLine;
  Until (DataLine = EOL);

  ParseString(Data);
End;

Function THttpResponse.Cookie(Key: String): String;
Begin
  Result := GetCookie(Key);
End;

Function THttpResponse.Header(Name: String): String;
Begin
  Result := GetHeader(Name);
End;

Procedure THttpResponse.ParseString(Const RawData: String);
Var 
  HeadString, HeaderString: String;
  HeadersV: TStringDynArray;
  RegexObj: TRegExpr;
  i: LongWord;
Begin
  i := Pos(EOLDouble, RawData);

  HeadString := Copy(RawData, 0, i);
  FBody := Trim(Copy(RawData, i, Length(RawData)));

  {$IFDEF DEBUG}
  Writeln(HeadString);

  If Length(HeadString) = 0 Then
    Begin
      Writeln('Malformed response: Missing message head');
    End;

  If Length(FBody) = 0
    Then Writeln('No Body provided');
  {$ENDIF}

  // Parse status line
  RegexObj := TRegExpr.Create;
  RegexObj.Expression := 'HTTP/(\d+(?:\.\d+)*) (\d+) ([\w ]+)\r\n';

  If RegexObj.Exec(HeadString) Then
    For i:=1 To RegexObj.SubExprMatchCount Do
      Case i Of 
        1: FHttpVersion := RegexObj.Match[i];
        2: FStatusCode := StrToInt(RegexObj.Match[i]);
        3: FStatusText := RegexObj.Match[i];
      End;

  // Parse Headers
  i := Pos(EOL, HeadString);
  HeadString := Trim(Copy(HeadString, i, Length(HeadString)));
  HeadersV := SplitString(HeadString, EOL);
  RegexObj.Expression := '([A-Za-z0-9\-]+): ?([\S ]+)';

  For HeaderString In HeadersV Do
    If RegexObj.Exec(HeaderString) Then
      SetHeader(RegexObj.Match[1], RegexObj.Match[2]);

  // Parse Cookies
  ParseCookieString;
End;

Procedure THttpResponse.ParseCookieString;
Var 
  SetCookieValue, CookiePair: String;
  CookiePairs: TStringDynArray;
  RegexObj: TRegExpr;
Begin
  SetCookieValue := Header('Set-Cookie');

  If Length(SetCookieValue) = 0 Then
    Begin
        {$IFDEF DEBUG}
      Writeln('No cookies supplied');
        {$ENDIF}
      Exit;
    End;

  RegexObj := TRegExpr.Create;
  RegexObj.Expression := '(\S+)=(\S+);?';

  CookiePairs := SplitString(SetCookieValue, ';');

  For CookiePair In CookiePairs Do
    If RegexObj.Exec(CookiePair) Then
      SetCookie(RegexObj.Match[1], RegexObj.Match[2]);
End;

Function THttpResponse.ReadContentChunk(BytesToRead: LongWord): String;
Var 
  BufferStr: String = '';
  Buffer: TByteArray;
  BytesRead: LongInt;
Begin
  BytesRead := FSock.Read(Buffer, BytesToRead);

  If BytesRead > 0 Then
    SetString(BufferStr, PChar(@Buffer[0]), BytesRead);

  Result := BufferStr;
End;

Function THttpResponse.ReadContentToFile(FileName: String): Boolean;
Var 
  DataFile: File Of Byte;
  Buffer: Array Of Byte;
  BytesRead: LongWord;
Const
  BufferSize: LongWord = 1024;
Begin
  SetLength(Buffer, BufferSize);

  Assign(DataFile, FileName);

  Try
    Rewrite(DataFile);

    Repeat
      BytesRead := FSock.Read(Buffer, BufferSize);

      BlockWrite(DataFile, Buffer[0], BytesRead);

    Until (BytesRead <> BufferSize);

    Close(DataFile);

    Result := True;
  Except
    On E: Exception Do
      Exit(False);
  End;
  
End;

Function THttpResponse.ReadContentAsString: String;
Var
  ReadBuffer: TByteArray;
  StringBuffer: String = '';
  BytesRead: LongInt;
Const
  ReadBufferSize: LongWord = 512;
begin
  SetLength(ReadBuffer, ReadBufferSize);

  Repeat
    FillChar(ReadBuffer[0], ReadBufferSize, #0);

    BytesRead := FSock.Read(ReadBuffer, ReadBufferSize);

    If BytesRead > 0 Then
      StringBuffer := StringBuffer + String(PAnsiChar(@ReadBuffer[0]));

  Until BytesRead <= 0;

  Result := StringBuffer;
end;

{$ENDIF}
