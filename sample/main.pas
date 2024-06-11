program SimpleHttpClient;
{$H+}

uses HttpMessages in '../HttpMessages.pas';

procedure LineRead(const Line: String);
begin
    Writeln('> ', Line);
end;

var
    URL: String;
    Request: THttpRequest;
    Response: THttpResponse;
    Header: THttpHeader;
    Cookie: THttpCookie;
begin
    Write('URL> ');
    Read(URL);

    Request := THttpRequest.Create(URL);
    //Request.OnLineRead := @LineRead;
    Response := Request.Send;

    Writeln(Response.Cookie('sii_alumno'));

    (*
    Request.Header('User-Agent', 'Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0');

    Response := Request.Send;

    //Writeln('RESPONSE', #10, Response);
    // Print THttpResponse Object
    Writeln('=============RESPONSE=============');
    Writeln('HTTP Version: ', Response.HttpVersion);
    Writeln('Status Code: ', Response.StatusCode);
    Writeln('Status Text: ', Response.StatusText);
    Writeln('Headers: ', Length(Response.Headers));

    Writeln('>>>>>>>>>> HEADERS');
    for Header in Response.Headers do
        Writeln('> ', Header.Name, ': ', Header.Value);

    Writeln('>>>>>>>>>> COOKIES');
    for Cookie in Response.Cookies do
        Writeln('> ', Cookie.Key, ': ', Cookie.Value);

    Writeln('Body: ', Response.Body);*)
end.


