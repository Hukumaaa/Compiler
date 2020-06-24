{ Test Program  }

var
value: integer;
j: integer;
i: integer;


{ write Function Test }
write(999);


{ Arithmetic Expressions Test }
value := 100 + 54 * (4 / 2); { = 208 };
write(value);


{ For Test }
for i := 0 to 10 do
   write(i);



{ While Test }
while value <> 100 do
    value := value - 1;
    write(value);



{ If Test }
j := 100;
if j <> 10 then
    write(j);



{ If-Else Test }
if j = 10 then
    write(1)
else
    write(0);



{ Error Test }
{ Uncomment this to get an error }


{  Semicolon test: }
{ value := 100 }

{ Undeclared variable: }
{ num := 100; }

{ Redeclared a variable: }
{ value: INTEGER; }


{ END at the end }
end