unit figures;

interface

uses System, System.Drawing, System.Windows.Forms;

type
  circle=class
  private
    graph:graphics;
    p:point;
    radius:integer;
  public
    procedure init(x,y:integer;var g:graphics);
    procedure draw(var pen1:pen;p1:point);
    procedure drawend(var pen1:pen;p1:point;var s:solidbrush);
  end;
  
  rect=class
  private
    graph:graphics;
    alpha: real;
    xa, ya, xb, yb, xc, yc: integer;
    p:point;
  public
    procedure init(p1:point;var g:graphics;a:real);
    procedure draw(var pen1:pen;p1:point);
    procedure drawend(var pen1:pen;p1:point;var s:solidbrush);
  end;

implementation

procedure circle.init(x,y:integer;var g:graphics);
begin
  p.x:=x;
  p.y:=y;
  graph:=g;
end;

procedure circle.draw(var pen1:pen;p1:point);
begin
  radius := round((sqrt(sqr(p1.x - p.x) + sqr(p1.y - p.y))) / (sqrt(2)));
  graph.Drawellipse(pen1, p.X, p.Y - radius, radius, radius);
end;

procedure circle.drawend(var pen1:pen;p1:point;var s:solidbrush);
begin
  radius := round((sqrt(sqr(p1.x - p.x) + sqr(p1.y - p.y))) / (sqrt(2)));
  graph.Drawellipse(pen1, p.X, p.Y - radius, radius, radius);
  graph.FillEllipse(s, p.X, p.Y - radius, radius, radius);
end;

procedure rect.init(p1:point;var g:graphics;a:real);
begin
  alpha:=a;
  p:=p1;
  graph:=g;
end;

procedure rect.draw(var pen1:pen;p1:point);
begin
  xa := round((p1.y - p.y) * sin(alpha) + p.x);
  ya := round((p1.y - p.y) * cos(alpha) + p.y);
  xb := round((p1.X - p.x) * cos(alpha) + (p1.Y - p.y) * sin(alpha) + p.X);
  yb := round((p1.y - p.y) * cos(alpha) - (p1.x - p.x) * sin(alpha) + p.y);
  xc := round((p1.x - p.x) * cos(alpha) + p.x);
  yc := round(-(p1.x - p.x) * sin(alpha) + p.y);
  graph.DrawLine(pen1, p.X, p.Y, xc, yc);
  graph.DrawLine(pen1, xc, yc, xb, yb);
  graph.DrawLine(pen1, xb, yb, xa, ya);
  graph.DrawLine(pen1, xa, ya, p.X, p.Y);
end;

procedure rect.drawend(var pen1:pen;p1:point;var s:solidbrush);
var 
  points:array of point;
begin
  xa := round((p1.y - p.y) * sin(alpha) + p.x);
  ya := round((p1.y - p.y) * cos(alpha) + p.y);
  xb := round((p1.X - p.x) * cos(alpha) + (p1.Y - p.y) * sin(alpha) + p.X);
  yb := round((p1.y - p.y) * cos(alpha) - (p1.x - p.x) * sin(alpha) + p.y);
  xc := round((p1.x - p.x) * cos(alpha) + p.x);
  yc := round(-(p1.x - p.x) * sin(alpha) + p.y);
  graph.DrawLine(pen1, p.X, p.Y, xc, yc);
  graph.DrawLine(pen1, xc, yc, xb, yb);
  graph.DrawLine(pen1, xb, yb, xa, ya);
  graph.DrawLine(pen1, xa, ya, p.X, p.Y);
  SetLength(points,4);
  points[0]:=p;
  points[1].x:=xa;
  points[1].y:=ya;
  points[2].x:=xb;
  points[2].y:=yb;
  points[3].x:=xc;
  points[3].y:=yc;
  graph.FillPolygon(s,points);
end;

end.