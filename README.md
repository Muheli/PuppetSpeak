# PuppetSpeak

Puppet Speak is an extremely lightweight symbolic expression parser that I wrote to serialize and deserialize commands sent between remote processes over a socket connection. The use is very similar to JSON but optimized to be lighter and easier to port between languages.

I created it for use in the BrainPool animation and AI controller which needed a way for a remote process to send control commands to game engine scripting systems using a variety of open and proprietary scripting languages. The C++ version of the parser is shown in “PpsCore.cpp” and uses less than 300 lines of code. A Python version of the parser was implemented in less than 100 lines. A version for Perch, the Shark3D scripting language was under 200 lines.

We published Puppet Speak as open source at “http://puppetspeak.sourceforge.net/” so that game programmers could easily connect any game to the BrainPool controller and because there are a wide variety of other possible uses.