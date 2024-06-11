// HttpMessage Class =====================================
{$IFDEF INTERFACE}

type
    THttpMessage = Class
        protected
            FHttpVersion: String;
            FHeaders: Array of THttpHeader;
            FCookies: Array of THttpCookie;
            FBody: String;
            procedure SetCookie(Key, Value: String);
            function GetCookie(Key: String): String;
            procedure SetHeader(Name, Value: String);
            function GetHeader(Name: String): String;
        public
            property HttpVersion: String Read FHttpVersion;
            property Headers: THttpHeaders Read FHeaders;
            property Cookies: THttpCookies Read FCookies;
            property Body: String Read FBody;
    End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

procedure THttpMessage.SetCookie(Key, Value: String);
var
    i: LongWord;
begin
    if Length(FCookies) > 0 then
        for i:=0 to High(FCookies) do
            if FCookies[i].Key = Key then
            begin
                FCookies[i].Value := Value;
                Exit;
            end;

    i := Length(FCookies);
    SetLength(FCookies, i + 1);
    FCookies[i].Key := Key;
    FCookies[i].Value := Value;
end;

function THttpMessage.GetCookie(Key: String): String;
var
    Cookie: THttpCookie;
begin
    for Cookie in FCookies do
        if Cookie.Key = Key then
            Exit(Cookie.Value);
end;

procedure THttpMessage.SetHeader(Name, Value: String);
var
    i: LongWord;
begin
    if Length(FHeaders) > 0 then
        for i:=0 to High(FHeaders) do
            if FHeaders[i].Name = Name then
            begin
                FHeaders[i].Value := Value;
                Exit;
            end;

    i := Length(FHeaders);
    SetLength(FHeaders, i + 1);
    FHeaders[i].Name := Name;
    FHeaders[i].Value := Value;
end;

function THttpMessage.GetHeader(Name: String): String;
var
    Header: THttpHeader;
begin
    for Header in FHeaders do
        if Header.Name = Name then
            Exit(Header.Value);
end;

{$ENDIF}
