// HttpRequest Class =====================================
{$IFDEF INTERFACE}

Type THttpRequest = Class(THttpMessage)
  Private 
    FHttpMethod: EHttpMethod;
    FUrl: TUrl;
    Procedure SetHttpMethod(NewHttpMethod: EHttpMethod);
    Function ToString: String;
  Public 
    property Url: TUrl Read FUrl;
    property HttpMethod: EHttpMethod Read FHttpMethod Write SetHttpMethod;
    constructor Create(StringUrl: String; Method: EHttpMethod = HTTP_GET);
    Procedure Cookie(Key, Value: String);
    Procedure Header(Name, Value: String);
    Procedure MessageBody(Data: String);
    Function Send: THttpResponse;
End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor THttpRequest.Create(StringUrl: String; Method: EHttpMethod = HTTP_GET);
Begin
  FHttpMethod := Method;
  FUrl := TUrl.Create(StringUrl);

  If FUrl.Protocol = PROTOCOL_HTTP
    Then FSock := TSocket.Create(FUrl.Host, DEFAULT_HTTP_PORT)
  Else FSock := TSSLSocket.Create(FUrl.Host, DEFAULT_HTTPS_PORT);

  Header('Host', FUrl.Host);
  Header('User-Agent', UA_PASCAL_WEB);
  Header('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8');
  Header('Accept-Language', 'en-US,es-MX;q=0.8');
  Header('Accept-Encoding', 'gzip, deflate, br');
  Header('DNT', '1');
  Header('Connection', 'close');
  Header('Upgrade-Insecure-Requests', '1');
  Header('Sec-Fetch-Dest', 'document');
  Header('Sec-Fetch-Mode', 'navigate');
  Header('Sec-Fetch-Site', 'none');
  Header('Sec-Fetch-User', '?1');
  Header('Pragma', 'no-cache');
  Header('Cache-Control', 'no-cache');
End;

Procedure THttpRequest.Cookie(Key, Value: String);
Begin
  SetCookie(Key, Value);
End;

Procedure THttpRequest.Header(Name, Value: String);
Begin
  SetHeader(Name, Value);
End;

Procedure THttpRequest.MessageBody(Data: String);
Begin
  FBody := Data;
End;

Function THttpRequest.Send: THttpResponse;
Var 
  StringRequest: String;
Begin
  StringRequest := ToString;

{$IFDEF DEBUG}
  Writeln('======================== REQUEST ========================');
  Writeln(StringRequest);
  Writeln('=========================================================');
{$ENDIF}

  FSock.Write(StringRequest);

  // Notify the server that all the data has been sent.
  FSock.FinishSending;

  Result := THttpResponse.Create(FSock);
End;

Function THttpRequest.ToString: String;
Var 
  HeaderItem: THttpHeader;
  CookieItem: THttpCookie;
  HeadString, MethodString, PathString: String;
  HeadersString: String = '';
  CookiesString: String = '';
  BodyLength: LongWord;
Begin
  Case HttpMethod Of 
    HTTP_HEAD: MethodString := 'HEAD';
    HTTP_GET: MethodString := 'GET';
    HTTP_POST: MethodString := 'POST';
    HTTP_PUT: MethodString := 'PUT';
    HTTP_OPTIONS: MethodString := 'OPTIONS';
    HTTP_DELETE: MethodString := 'DELETE';
  End;

  PathString := FUrl.Path;

  If Length(FUrl.Params) > 0 Then
    PathString := PathString + '?' + FUrl.Params;

  HeadString := MethodString + ' ' + PathString + ' HTTP/1.1' + EOL;

  If Length(FCookies) > 0 Then
    Begin
      For CookieItem In FCookies Do
        CookiesString := CookiesString + CookieItem.Key + '=' + CookieItem.Value + ';';

      Header('Cookie', CookiesString);
    End;

  If HttpMethod = HTTP_POST Then
    Begin
      BodyLength := Length(Body);
      Header('Content-Length', IntToStr(BodyLength));

        {$IFDEF DEBUG}
      If BodyLength = 0 Then
        Writeln('Message body required but not found');
        {$ENDIF}
    End;

  If Length(FHeaders) > 0 Then
    For HeaderItem In FHeaders Do
      HeadersString := HeadersString + HeaderItem.Name + ': ' + HeaderItem.Value + EOL;

  Result := HeadString + HeadersString + EOL + Body;
End;

Procedure THttpRequest.SetHttpMethod(NewHttpMethod: EHttpMethod);
Begin
  FHttpMethod := NewHttpMethod;
End;

{$ENDIF}
