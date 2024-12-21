Unit PascalWeb;
{$MODE OBJFPC} {$H+}
//{$DEFINE DEBUG}

Interface
{$DEFINE INTERFACE}

Uses OpenSSL, Sockets, Resolve, RegExpr, Types, StrUtils, SysUtils;

Const 
  DEFAULT_HTTP_PORT: Word = 80;
  
  DEFAULT_HTTPS_PORT: Word = 443;
  
  EOL: String = #13 + #10;

  EOLDouble: String = #13 + #10 + #13 + #10;

Type 
  TCallbackLineRead = Procedure (Const Line: String);

  THttpHeader = Record
    Name: String;
    Value: String;
  End;

  THttpHeaders = Array Of THttpHeader;

  TKeyValuePair = Record
    Key: String;
    Value: String;
  End;

  THttpCookie = TKeyValuePair;
  THttpCookies = Array Of THttpCookie;

  TURLParam = TKeyValuePair;
  TURLParams = Array Of TURLParam;

  TByteArray = Array Of Byte;

  EHttpMethod = (HTTP_HEAD, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_OPTIONS, HTTP_DELETE);
  EHttpProtocol = (PROTOCOL_HTTP, PROTOCOL_HTTPS);

{$I URL.inc}
{$I Socket.inc}
{$I SSLSocket.inc}
{$I HttpMessage.inc}
{$I HttpResponse.inc}
{$I HttpRequest.inc}
{$I UserAgents.inc}

{$UNDEF INTERFACE}

Implementation
{$DEFINE IMPLEMENTATION}

{$I URL.inc}
{$I Socket.inc}
{$I SSLSocket.inc}
{$I HttpMessage.inc}
{$I HttpResponse.inc}
{$I HttpRequest.inc}

{$UNDEF IMPLEMENTATION}
initialization

// SSL Initialization
SSLLibraryInit;

End.
