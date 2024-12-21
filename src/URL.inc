// URL Class =============================================
{$IFDEF INTERFACE}

Type TURL = Class
  private
    FProtocol: EHttpProtocol;
    FHost: String;
    FPath: String;
    FParams: TURLParams;
    Function SavedParamsToString: String;
  public
    property Protocol: EHttpProtocol Read FProtocol;
    property Host: String Read FHost;
    property Path: String Read FPath;
    property Params: String Read SavedParamsToString;
    
    Constructor Create;
    
    Constructor Create(StringURL: String); overload;
    
    Function GetParam(Key: String): String;
    Procedure SetParam(Key, Value: String);

    Class Function ParamsToString(Parameters: TURLParams): String;
    
    Class Function ParamsFromString(Parameters: String): TURLParams;
    
    Class Function EscapeString(UnscapedString: String): String;
End;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

Constructor TURL.Create;
Begin
  FPath := '/';
  FHost := '0.0.0.0';
  FProtocol := PROTOCOL_HTTPS;
End;

Constructor TURL.Create(StringURL: String);
Var
  StringProtocol, StringParams: String;
  RegexObj: TRegExpr;
  i: LongWord;
Begin
  RegexObj := TRegExpr.Create;

  RegexObj.Expression := '(?:(https?)://)?' +
               '([A-Za-z0-9\-]+(?:\.[A-Za-z0-9\-]+)+)' +
               '(/[\w\.\-/]*)*' +
               '(?:\?((?:[\w\-]+=[\w\-]+&?)+))?';

  If RegexObj.Exec(StringURL) Then
    For i:=1 To RegexObj.SubExprMatchCount Do
      If Length(RegexObj.Match[i]) > 0 Then
        Case i Of
          1: StringProtocol := RegexObj.Match[1];
          2: FHost := RegexObj.Match[2];
          3: FPath := RegexObj.Match[3];
          4: StringParams := RegexObj.Match[4];
        End;

  If StringProtocol = 'http'
    Then FProtocol := PROTOCOL_HTTP
    Else FProtocol := PROTOCOL_HTTPS;

  FParams := TURL.ParamsFromString(StringParams);

  If Length(FPath) = 0 Then
    FPath := '/';

{$IFDEF DEBUG}
  Writeln('[DEBUG][URL] URL Protocol: ', FProtocol);
  Writeln('[DEBUG][URL] URL Host: ', FHost);
  Writeln('[DEBUG][URL] URL Path: ', FPath);
  Writeln('[DEBUG][URL] URL Params: ', StringParams);
{$ENDIF}
End;

Function TURL.SavedParamsToString: String;
Begin
  Exit(TURL.ParamsToString(FParams));
End;

Function TURL.GetParam(Key: String): String;
Var
  Param: TURLParam;
Begin
  For Param in FParams Do
    If Param.Key = Key Then
      Exit(Param.Value);
End;

Procedure TURL.SetParam(Key, Value: String);
Var
  i: LongWord;
Begin
  If Length(FParams) <> 0 Then
    For i:=0 To High(FParams) Do
      If FParams[i].Key = Key Then
        Begin
          FParams[i].Value := Value;
          Exit;
        End;

  i := Length(FParams);
  
  SetLength(FParams, i + 1);

  FParams[i].Key := Key;
  FParams[i].Value := Value;
End;

Class Function TURL.ParamsToString(Parameters: TURLParams): String;
Var
  i: LongWord;
  ParamsString: String = '';
Begin
  If Length(Parameters) > 0 Then
    For i:=0 To High(Parameters) Do
      Begin
        ParamsString := ParamsString + Parameters[i].Key + '=' + Parameters[i].Value;

        If i <> High(Parameters) Then
          ParamsString := ParamsString + '&';
      End;

  Result := TURL.EscapeString(ParamsString);
End;

Class Function TURL.ParamsFromString(Parameters: String): TURLParams;
Var
  ParamsV: TStringDynArray;
  Param: String;
  RegexObj: TRegExpr;
  URLParams: TURLParams;
  i: LongWord = 0;
Begin
  RegexObj := TRegExpr.Create;
  ParamsV := SplitString(Parameters, '&');

  For Param in ParamsV Do
    Begin
      RegexObj.Expression := '([\w\-]+)=([\w\-]+)';

      If RegexObj.Exec(Param) Then
      Begin
        SetLength(URLParams, i + 1);
        
        URLParams[i].Key := RegexObj.Match[1];
        URLParams[i].Value := RegexObj.Match[2];

        Inc(i);
      End;
    End;

  Result := URLParams;
End;

Class Function TURL.EscapeString(UnscapedString: String): String;
Var
  EscapedString: String;
Begin
  EscapedString := UnscapedString;
  EscapedString := StringReplace(EscapedString, '[', '%5B', [rfReplaceAll]);
  EscapedString := StringReplace(EscapedString, ']', '%5D', [rfReplaceAll]);

  Exit(EscapedString);
End;

{$ENDIF}
