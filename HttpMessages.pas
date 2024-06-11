unit HttpMessages;
{$MODE OBJFPC} {$H+}
//{$DEFINE DEBUG}
interface
{$DEFINE INTERFACE}

uses OpenSSL, Sockets, Resolve, RegExpr, Types, StrUtils, SysUtils;

const
    DefaultHttpPort: Word = 80;
    DefaultHttpsPort: Word = 443;
    EOL: String = #13 + #10;
    EOLDouble: String = #13 + #10 + #13 + #10;

type
    TCallbackLineRead = Procedure (const Line: String);

    THttpHeader = Record
        Name: String;
        Value: String;
    End;

    THttpHeaders = Array of THttpHeader;

    TKeyValuePair = Record
        Key: String;
        Value: String;
    End;

    THttpCookie = TKeyValuePair;
    THttpCookies = Array of THttpCookie;

    TUrlParam = TKeyValuePair;
    TUrlParams = Array of TUrlParam;

    EHttpMethod = (HTTP_HEAD, HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_OPTIONS, HTTP_DELETE);
    EHttpProtocol = (PROTOCOL_HTTP, PROTOCOL_HTTPS);

{$I Url.inc}
{$I Socket.inc}
{$I SslSocket.inc}
{$I HttpMessage.inc}
{$I HttpResponse.inc}
{$I HttpRequest.inc}

{$UNDEF INTERFACE}
implementation
{$DEFINE IMPLEMENTATION}

{$I Url.inc}
{$I Socket.inc}
{$I SslSocket.inc}
{$I HttpMessage.inc}
{$I HttpResponse.inc}
{$I HttpRequest.inc}

{$UNDEF IMPLEMENTATION}
initialization

// Ssl Initialization
SslLibraryInit;

end.
