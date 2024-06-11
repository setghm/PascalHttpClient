// HttpResponse Class ====================================
{$IFDEF INTERFACE}

type
    THttpResponse = Class(THttpMessage)
        private
            FStatusCode: Word;
            FStatusText: String;
            procedure ParseString(const RawData: String);
            procedure ParseCookieString;
        public
            property StatusCode: Word Read FStatusCode;
            property StatusText: String Read FStatusText;
            constructor Create(const RawData: String);
            function Cookie(Key: String): String;
            function Header(Name: String): String;
    End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor THttpResponse.Create(const RawData: String);
begin
(**
    {$IFDEF DEBUG}
    Writeln('======================== RESPONSE =======================');
    Writeln(RawData);
    Writeln('=========================================================');
    {$ENDIF}
**)
    ParseString(RawData);
end;

function THttpResponse.Cookie(Key: String): String;
begin
    Result := GetCookie(Key);
end;

function THttpResponse.Header(Name: String): String;
begin
    Result := GetHeader(Name);
end;

procedure THttpResponse.ParseString(const RawData: String);
var
    HeadString, HeaderString: String;
    HeadersV: TStringDynArray;
    RegexObj: TRegExpr;
    i: LongWord;
begin
    i := Pos(EOLDouble, RawData);

    HeadString := Copy(RawData, 0, i);
    FBody := Trim(Copy(RawData, i, Length(RawData)));

    {$IFDEF DEBUG}
    Writeln(HeadString);

    if Length(HeadString) = 0 then
    begin
        Writeln('Something went wrong: Missing message head');
    end;

    if Length(FBody) = 0
        then Writeln('No Body provided');
    {$ENDIF}

    // Parse status line
    RegexObj := TRegExpr.Create;
    RegexObj.Expression := 'HTTP/(\d+(?:\.\d+)*) (\d+) ([\w ]+)\r\n';
    if RegexObj.Exec(HeadString) then
        for i:=1 to RegexObj.SubExprMatchCount do
            case i of
                1: FHttpVersion := RegexObj.Match[i];
                2: FStatusCode := StrToInt(RegexObj.Match[i]);
                3: FStatusText := RegexObj.Match[i];
            end;

    // Parse Headers
    i := Pos(EOL, HeadString);
    HeadString := Trim(Copy(HeadString, i, Length(HeadString)));
    HeadersV := SplitString(HeadString, EOL);
    RegexObj.Expression := '([A-Za-z0-9\-]+): ?([\S ]+)';

    for HeaderString in HeadersV do
        if RegexObj.Exec(HeaderString) then
            SetHeader(RegexObj.Match[1], RegexObj.Match[2]);

    // Parse Cookies
    ParseCookieString;
end;

procedure THttpResponse.ParseCookieString;
var
    SetCookieValue, CookiePair: String;
    CookiePairs: TStringDynArray;
    RegexObj: TRegExpr;
begin
    SetCookieValue := Header('Set-Cookie');

    if Length(SetCookieValue) = 0 then
    begin
        {$IFDEF DEBUG}
            Writeln('No cookies supplied');
        {$ENDIF}
        Exit;
    end;

    RegexObj := TRegExpr.Create;
    RegexObj.Expression := '(\S+)=(\S+);?';

    CookiePairs := SplitString(SetCookieValue, ';');

    for CookiePair in CookiePairs do
        if RegexObj.Exec(CookiePair) then
            SetCookie(RegexObj.Match[1], RegexObj.Match[2])
end;

{$ENDIF}
