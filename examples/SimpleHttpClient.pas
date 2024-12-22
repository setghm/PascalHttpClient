
Program SimpleHttpClient;
{$H+}

{$UNITPATH ../build}

Uses PascalWeb;

Var 
  URL: String;
  Request: THttpRequest;
  Response: THttpResponse;
  Header: THttpHeader;
  Cookie: THttpCookie;
Begin
  Write('URL> ');
  Read(URL);

  Request := THttpRequest.Create(URL);
  Request.Header('User-Agent', 'Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0');

  Response := Request.Send;

  Writeln('============= RESPONSE =============');
  Writeln('HTTP Version: ', Response.HttpVersion);
  Writeln('Status Code: ', Response.StatusCode);
  Writeln('Status Text: ', Response.StatusText);
  Writeln('Headers: ', Length(Response.Headers));

  Writeln('>>>>>>>>>> HEADERS');
  For Header In Response.Headers Do
    Writeln('> ', Header.Name, ': ', Header.Value);

  Writeln('>>>>>>>>>> COOKIES');
  For Cookie In Response.Cookies Do
    Writeln('> ', Cookie.Key, ': ', Cookie.Value);

  Writeln('>>>>>>>>>>> BODY');
  Writeln(Response.ReadContentAsString);

End.
