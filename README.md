# Calculator - a calculator with variable precision and complex numbers support.

## What does variable precision mean?
Well, try getting a regular calculator and tell it to calculate, for example, the square root of 2. Most calculators will just say `1.414213562`. A better calculator will give you more decimal places. This calculator will give you as many decimal places, as you wish - limited only by available memory, and, probably more important - by time of calculation :p Just use the `setprec()` function to set the desired precision and calculate whatever you want.

## Compiling
Compiles fine with G++:
g++ -o calc *.cpp
or, in Windows:
g++ -o calc.exe *.cpp

Should compile on VC++ too.

## Usage
Type `help` to see a list of available functions. `list_vars` lists defined variables.
To calculate something, just type an expression like `sqrt(2+5*(8-exp(2)))`.
