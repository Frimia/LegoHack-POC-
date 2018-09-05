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

  So the result I couldn't find a solid entity list maybe I'm missing something but I'm not sure nothing like I've seen from the usual games I reverse. So instead of trying to find a pointer to the entity list that will shift every few days when Roblox updates to break hacks. 
  
  Instead I started scanning for player bases in all of the pages allocated to the roblox process, in order to make that feasible I read entire pages at a times and scanned it for the player base sig I created keep in mind the code was very sloppy and still is. I haven't worked on it for a while and it may be completely broken but when I had it working a while ago the scans were extremely fast depending on the game.
