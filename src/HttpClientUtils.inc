// Util functions to perform fast http requests.
{$IFDEF INTERFACE}

Function HttpGet(URL: String): THttpResponse;

Function HttpPost(URL: String; Payload: String): THttpResponse;

Function HttpPut(URL: String; Payload: String): THttpResponse;

Function HttpDelete(URL: String): THttpResponse;

{$ENDIF}
{$IFDEF IMPLEMENTATION}

Function HttpGet(URL: String): THttpResponse;
var
  Request: THttpRequest;
begin
  Request := THttpRequest.Create(URL);

  Result := Request.Send;
end;

Function HttpPost(URL: String; Payload: String): THttpResponse;
var
  Request: THttpRequest;
begin
  Request := THttpRequest.Create(URL);

  Request.HttpMethod := HTTP_POST;

  Request.Header('Content-Length', IntToStr(Length(Payload)));

  Result := Request.Send;
end;

Function HttpPut(URL: String; Payload: String): THttpResponse;
var
  Request: THttpRequest;
begin
  Request := THttpRequest.Create(URL);
  
  Request.HttpMethod := HTTP_PUT;

  Request.Header('Content-Length', IntToStr(Length(Payload)));

  Result := Request.Send;
end;

Function HttpDelete(URL: String): THttpResponse;
var
  Request: THttpRequest;
begin
  Request := THttpRequest.Create(URL);
  
  Request.HttpMethod := HTTP_DELETE;

  Result := Request.Send;
end;

{$ENDIF}