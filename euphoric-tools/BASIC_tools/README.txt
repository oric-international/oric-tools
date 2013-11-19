BASIC Tools :

- txt2bas converts a text file containing the source of a BASIC program to an
oric tape,
- bas2txt outputs to the screen the BASIC program that is contained in an Oric tape


Example :

Enter the following two lines in a file named hello.txt :

10 PRINT"HELLO"
20 END

Then convert the hello.txt file to an Oric tape :

txt2bas hello.txt hello.tap

You get the hello.tap tape, that you can load on Euphoric or any other emulator.

Now if you have a BASIC program in a .tap tape (like the hello.tap you just
built above),
you can ouput its listing to the screen with :

bas2txt hello.tap

NB: if you want to save the ouput to a file, just type :
bas2txt hello.tap > hello2.txt


Options:

txt2bas has a version option :
-v0 uses the Oric-1 tokens (ROM 1.0)
-v1 uses the Atmos tokens (ROM 1.1)
-v2 uses the BASIC Evolution tokens (my Basic Evolution ROM)

Just type txt2bas without any file argument to have a remainder about the
program's usage.

