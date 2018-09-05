# LegoHack-POC-
A proof of concept external esp for Roblox. Using signature scans to find player bases and only thing needed to update the hack
is the offset to the viewmatrix.

## Why?
I'm not really interested in this game much why myself but I've noticed that there are very little cheats for it despite a surprisingly 
high demand for them. Most cheats written for this game are in LUA and are executed through script executers kind of like how we use 
to execute sqf hacks back in the Arma 2 scripting days. These types of hacks are easily detectable if developers know what to look for
and Roblox actively works on security for their game so what about hacks written in pure C++ where are those at?


## Roblox External ESP
As a learning project a long while back I took up the task of trying to make an external ESP for this game like an actual box esp hack with player names and correctly calculated boxes all in c++ externally no lua (not that I'm even interested in that) and no settling for chams because anyone can hook DrawIndexed, log models and call it a day.
