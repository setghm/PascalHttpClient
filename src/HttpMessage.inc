// HttpMessage Class =====================================
{$IFDEF INTERFACE}

Type THttpMessage = Class
  Protected 
    FHttpVersion: String;
    FHeaders: Array Of THttpHeader;
    FCookies: Array Of THttpCookie;
    FBody: String;
    FSock: TSocket;
  Public
    property HttpVersion: String Read FHttpVersion;
    property Headers: THttpHeaders Read FHeaders;
    property Cookies: THttpCookies Read FCookies;
    property Body: String Read FBody;

    Procedure SetCookie(Key, Value: String);
    Function GetCookie(Key: String): String;
    Procedure SetHeader(Name, Value: String);
    Function GetHeader(Name: String): String;
End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

Procedure THttpMessage.SetCookie(Key, Value: String);
Var 
  i: LongWord;
Begin
  If Length(FCookies) > 0 Then
    For i:=0 To High(FCookies) Do
      If FCookies[i].Key = Key Then
        Begin
          FCookies[i].Value := Value;
          Exit;
        End;

  i := Length(FCookies);
  SetLength(FCookies, i + 1);
  
  FCookies[i].Key := Key;
  FCookies[i].Value := Value;
End;

Function THttpMessage.GetCookie(Key: String): String;
Var 
  Cookie: THttpCookie;
Begin
  For Cookie In FCookies Do
    If Cookie.Key = Key Then
      Exit(Cookie.Value);
End;

Procedure THttpMessage.SetHeader(Name, Value: String);
Var 
  i: LongWord;
Begin
  If Length(FHeaders) > 0 Then
    For i:=0 To High(FHeaders) Do
      If FHeaders[i].Name = Name Then
        Begin
          FHeaders[i].Value := Value;
          Exit;
        End;

  i := Length(FHeaders);
  SetLength(FHeaders, i + 1);

  FHeaders[i].Name := Name;
  FHeaders[i].Value := Value;
End;

Function THttpMessage.GetHeader(Name: String): String;
Var 
  Header: THttpHeader;
Begin
  For Header In FHeaders Do
    If Header.Name = Name Then
      Exit(Header.Value);
End;

{$ENDIF}
