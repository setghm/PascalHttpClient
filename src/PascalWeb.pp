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

  EOL_DOUBLE: String = #13 + #10 + #13 + #10;

Type 
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

{$I UserAgents.inc}
{$I URL.inc}
{$I Socket.inc}
{$I SSLSocket.inc}
{$I HttpMessage.inc}
{$I HttpResponse.inc}
{$I HttpRequest.inc}
{$I HttpClientUtils.inc}

{$UNDEF INTERFACE}

Implementation
{$DEFINE IMPLEMENTATION}

{$I URL.inc}
{$I Socket.inc}
{$I SSLSocket.inc}
{$I HttpMessage.inc}
{$I HttpResponse.inc}
{$I HttpRequest.inc}
{$I HttpClientUtils.inc}

{$UNDEF IMPLEMENTATION}
Initialization
{$DEFINE INITIALIZATION}

{$I SSLSocket.inc}

{$UNDEF INITIALIZATION}
End.
