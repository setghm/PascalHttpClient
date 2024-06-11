// Url Class =============================================
{$IFDEF INTERFACE}

type
    TUrl = Class
        private
            FProtocol: EHttpProtocol;
            FHost: String;
            FPath: String;
            FParams: TUrlParams;
            function SavedParamsToString: String;
        public
            property Protocol: EHttpProtocol Read FProtocol;
            property Host: String Read FHost;
            property Path: String Read FPath;
            property Params: String Read SavedParamsToString;
            constructor Create;
            constructor Create(StringUrl: String); overload;
            function GetParam(Key: String): String;
            procedure SetParam(Key, Value: String);
            class function ParamsToString(Parameters: TUrlParams): String;
            class function ParamsFromString(Parameters: String): TUrlParams;
            class function EscapeString(UnscapedString: String): String;
    End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

constructor TUrl.Create;
begin
    FPath := '/';
    FHost := '0.0.0.0';
    FProtocol := PROTOCOL_HTTPS;
end;

constructor TUrl.Create(StringUrl: String);
var
    StringProtocol, StringParams: String;
    RegexObj: TRegExpr;
    i: LongWord;
begin
    RegexObj := TRegExpr.Create;
    RegexObj.Expression := '(?:(https?)://)?' +
                           '([A-Za-z0-9\-]+(?:\.[A-Za-z0-9\-]+)+)' +
                           '(/[\w\.\-/]*)*' +
                           '(?:\?((?:[\w\-]+=[\w\-]+&?)+))?';

    if RegexObj.Exec(StringUrl) then
        for i:=1 to RegexObj.SubExprMatchCount do
            if Length(RegexObj.Match[i]) > 0 then
                case i of
                    1: StringProtocol := RegexObj.Match[1];
                    2: FHost := RegexObj.Match[2];
                    3: FPath := RegexObj.Match[3];
                    4: StringParams := RegexObj.Match[4];
                end;

    if StringProtocol = 'http'
        then FProtocol := PROTOCOL_HTTP
        else FProtocol := PROTOCOL_HTTPS;

    FParams := TUrl.ParamsFromString(StringParams);

    if Length(FPath) = 0 then
        FPath := '/';

    {$IFDEF DEBUG}
    Writeln('URL Protocol: ', FProtocol);
    Writeln('URL Host: ', FHost);
    Writeln('URL Path: ', FPath);
    Writeln('URL Params: ', StringParams);
    {$ENDIF}
end;

function TUrl.SavedParamsToString: String;
begin
    Exit(TUrl.ParamsToString(FParams));
end;

function TUrl.GetParam(Key: String): String;
var
    Param: TUrlParam;
begin
    for Param in FParams do
        if Param.Key = Key then
            Exit(Param.Value);
end;

procedure TUrl.SetParam(Key, Value: String);
var
    i: LongWord;
begin
    if Length(FParams) <> 0 then
        for i:=0 to High(FParams) do
            if FParams[i].Key = Key then
            begin
                FParams[i].Value := Value;
                Exit;
            end;

    i := Length(FParams);
    SetLength(FParams, i + 1);
    FParams[i].Key := Key;
    FParams[i].Value := Value;
end;

class function TUrl.ParamsToString(Parameters: TUrlParams): String;
var
    i: LongWord;
    ParamsString: String = '';
begin
    if Length(Parameters) > 0 then
        for i:=0 to High(Parameters) do
        begin
            ParamsString := ParamsString + Parameters[i].Key + '=' + Parameters[i].Value;
            if i <> High(Parameters) then
                ParamsString := ParamsString + '&';
        end;

    Result := TUrl.EscapeString(ParamsString);
end;

class function TUrl.ParamsFromString(Parameters: String): TUrlParams;
var
    ParamsV: TStringDynArray;
    Param: String;
    RegexObj: TRegExpr;
    UrlParams: TUrlParams;
    i: LongWord = 0;
begin
    RegexObj := TRegExpr.Create;
    ParamsV := SplitString(Parameters, '&');

    for Param in ParamsV do
    begin
        RegexObj.Expression := '([\w\-]+)=([\w\-]+)';
        if RegexObj.Exec(Param) then
        begin
            SetLength(UrlParams, i + 1);
            UrlParams[i].Key := RegexObj.Match[1];
            UrlParams[i].Value := RegexObj.Match[2];
            inc(i);
        end;
    end;

    Result := UrlParams;
end;

class function TUrl.EscapeString(UnscapedString: String): String;
var
    EscapedString: String;
begin
    EscapedString := UnscapedString;
    EscapedString := StringReplace(EscapedString, '[', '%5B', [rfReplaceAll]);
    EscapedString := StringReplace(EscapedString, ']', '%5D', [rfReplaceAll]);
    Exit(EscapedString);
end;

{$ENDIF}
