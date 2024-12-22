program GetExternalAddress;

{$UNITPATH ../build}

Uses PascalWeb;

var
  Response: THttpResponse;
begin
  Response := HttpGet('https://ifconfig.me/ip');

  WriteLn(Response.ReadContentAsString);
end.