// HttpRequest Class =====================================
{$IFDEF INTERFACE}

type
    THttpRequest = Class(THttpMessage)
        private
            FHttpMethod: EHttpMethod;
            FUrl: TUrl;
            FOnLineRead: TCallbackLineRead;
            Sock: TSocket;
            procedure SetHttpMethod(NewHttpMethod: EHttpMethod);
            procedure SetOnLineRead(Callback: TCallbackLineRead);
            function ToRawString: String;
        public
            property Url: TUrl Read FUrl;
            property HttpMethod: EHttpMethod Read FHttpMethod Write SetHttpMethod;
            property OnLineRead: TCallbackLineRead Write SetOnLineRead;
            constructor Create(StringUrl: String; Method: EHttpMethod = HTTP_GET);
            procedure Cookie(Key, Value: String);
            procedure Header(Name, Value: String);
            procedure MessageBody(Data: String);
            function Send: THttpResponse;
    End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor THttpRequest.Create(StringUrl: String; Method: EHttpMethod = HTTP_GET);
begin
    FOnLineRead := nil;
    FHttpMethod := Method;
    FUrl := TUrl.Create(StringUrl);

    if FUrl.Protocol = PROTOCOL_HTTP
        then Sock := TSocket.Create(FUrl.Host, DefaultHttpPort)
        else Sock := TSslSocket.Create(FUrl.Host, DefaultHttpsPort);

    Header('Host', FUrl.Host);
    Header('User-Agent', 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:42.0) Gecko/20100101 Firefox/42.0');
    Header('Accept', 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8');
    Header('Accept-Language', 'en-US,es-MX;q=0.8');
    Header('Accept-Encoding', 'gzip, deflate, br');
    Header('DNT', '1');
    Header('Connection', 'keep-alive');
    Header('Upgrade-Insecure-Requests', '1');
    Header('Sec-Fetch-Dest', 'document');
    Header('Sec-Fetch-Mode', 'navigate');
    Header('Sec-Fetch-Site', 'none');
    Header('Sec-Fetch-User', '?1');
    Header('Pragma', 'no-cache');
    Header('Cache-Control', 'no-cache');
end;

procedure THttpRequest.Cookie(Key, Value: String);
begin
    SetCookie(Key, Value);
end;

procedure THttpRequest.Header(Name, Value: String);
begin
    SetHeader(Name, Value);
end;

procedure THttpRequest.MessageBody(Data: String);
begin
    FBody := Data;
end;

function THttpRequest.Send: THttpResponse;
var
    RawResponse, RawRequest: String;
begin
    RawRequest := self.ToRawString;

    {$IFDEF DEBUG}
    Writeln('======================== REQUEST ========================');
    Writeln(RawRequest);
    Writeln('=========================================================');
    {$ENDIF}

    Sock.Write(RawRequest);

    if FOnLineRead <> nil then
    begin
        Repeat
            RawResponse := Sock.Readln;
            FOnLineRead(RawResponse);
        Until RawResponse = EOL;
    end
    else RawResponse := Sock.Read;

    Result := THttpResponse.Create(RawResponse);
end;

function THttpRequest.ToRawString: String;
var
    HeaderItem: THttpHeader;
    CookieItem: THttpCookie;
    HeadString, MethodString, PathString: String;
    HeadersString: String = '';
    CookiesString: String = '';
    BodyLength: LongWord;
begin
    case HttpMethod of
        HTTP_HEAD: MethodString := 'HEAD';
        HTTP_GET: MethodString := 'GET';
        HTTP_POST: MethodString := 'POST';
        HTTP_PUT: MethodString := 'PUT';
        HTTP_OPTIONS: MethodString := 'OPTIONS';
        HTTP_DELETE: MethodString := 'DELETE';
    end;

    PathString := FUrl.Path;

    if Length(FUrl.Params) > 0 then
        PathString := PathString + '?' + FUrl.Params;

    HeadString := MethodString + ' ' + PathString + ' HTTP/1.1' + EOL;

    if Length(FCookies) > 0 then
    begin
        for CookieItem in FCookies do
            CookiesString := CookiesString + CookieItem.Key + '=' + CookieItem.Value + ';';

        Header('Cookie', CookiesString);
    end;

    if HttpMethod = HTTP_POST then
    begin
        BodyLength := Length(Body);
        Header('Content-Length', IntToStr(BodyLength));

        {$IFDEF DEBUG}
        if BodyLength = 0 then
            Writeln('Message body required but not found');
        {$ENDIF}
    end;

    if Length(FHeaders) > 0 then
        for HeaderItem in FHeaders do
            HeadersString := HeadersString + HeaderItem.Name + ': ' + HeaderItem.Value + EOL;

    Result := HeadString + HeadersString + EOL + Body;
end;

procedure THttpRequest.SetHttpMethod(NewHttpMethod: EHttpMethod);
begin
    FHttpMethod := NewHttpMethod;
end;

procedure THttpRequest.SetOnLineRead(Callback: TCallbackLineRead);
begin
    FOnLineRead := Callback;
end;


{$ENDIF}
