# This game is now on Steam!

#### But first: A little history.

Back in 2015 I had just finished reading two very influential books: [SFML Game Development](https://www.amazon.co.uk/SFML-Game-Development-Jan-Haller/dp/1849696845) by Jan Haller, Henrik Vogelius Hansson and Artur Moreira, and [Game Programming Patterns](http://gameprogrammingpatterns.com/) by Robert Nystrom. It was thanks to these books that I finally managed to coalesce some code in such a way that I had finally completed a project from start to end without having coded myself into a knotty mess. That game was [Pseuthe](https://github.com/fallahn/pseuthe) which, while not a particularly enthralling game to play, was a milestone for me personally. It was the first time I felt I had some reasonably designed code and it buoyed my enthusiasm for hobby game development. [Crush](https://github.com/fallahn/crush) quickly followed, using mostly the same programming layout and recycling a lot of the code files from Pseuthe. It only came to a halt when I decided I'd like to add multiplayer networking and quickly discovered that not only was it not a trivial thing to do, it generally required the game being built from the ground up around the networking mechanic.

Unperturbed, however, I set about creating this repository, based on the vague notion of being able to create mini 'programs' to steer a lawn mower around a garden, and share those programs online via a network play mode. This would be the third project which shared a large subset of code between Crush and Pseuthe. It became a pivotal point as this was when I decided to move the shared code into its own library and [xygine](https://github.com/fallahn/xygine) was born. As it turns out this 'game' (RoboMower) never got much further than a user interface, along with some experiments in networking based on [Glenn Fiedler's articles](https://gafferongames.com/). xygine, however, was destined for much bigger things...

So pleased was I with finally having my own game programming framework in xygine that many experiments followed, some of which can be found in [their own repository](https://github.com/fallahn/xyExperimental). At one point I even played with adding a [3D renderer](https://trederia.blogspot.com/2016/06/xygine-feature-3d-model-component.html) to it, which eventually paved the way for [DoodleBob](https://github.com/fallahn/xyExperimental/tree/master/DoodleChum), a project I worked on mostly during the winter holidays of 2016-2017. It was to become my second completed project - though much like Pseuthe it lacked any riveting gameplay. If you'd like to try it, it can be downloaded [here](https://gamejolt.com/games/doodlebob/256272). DoodleBob highlighted two major flaws with xygine. Firstly the [data locality](http://gameprogrammingpatterns.com/data-locality.html) in my entity-like component system was horrible, and measurably damaging performance. Secondly the mix of OpenGL for the 3D part with [SFML](https://www.sfml-dev.org/) (the library behind the cross-platform parts of xygine) was incompatible with macOS. It was at about this point I had also read a lot about ECS design and was eager to try implementing it myself.

To attempt to better understand these problems, as well as investigate mobile platform development (something which SFML still doesn't support very well) I turned to [SDL2](https://www.libsdl.org/download-2.0.php), and created a 3D mobile-centric game framework named [Crogine](https://github.com/fallahn/crogine). As an experiment this was quite successful, and I created a near-complete demo game that ran well on my clunky old Android phone. Unfortunately the toll of creating 3D assets for such projects became too much and my enthusiasm began to wane. Crogine is a very capable library and potentially still open to further development - but the real gain came from the ECS I had implemented. I soon found myself branching xygine, gutting most of its existing systems, and inserting the new ECS. This was done under the working title 'xyginext' as the binaries of the new version were incompatible with the old one, and therefore required a different binary name. Once the branch was merged, however, the library was again unified under the 'xygine' moniker.

While this major refactoring was taking place it also required a huge amount of rewriting of the documentation. As part of this process I felt an example was as good an explanation as any amount of wiki pages. The [xygine demo](https://github.com/fallahn/xygine/tree/master/Demo) was created, which not only demonstrated how to use xygine but also finally realised my goal of a networked multiplayer game (as well as implementing some of the crate-crushing physics of Crush). Winter 2017-2018 soon rolled around, and during the holidays I decided for my next project I would flesh out the demo into a fully fledged game. This is what became [Castle Clamber](https://store.steampowered.com/app/770050/Castle_Clamber/), the first game I released on Steam. A Steam release had never originally been the goal of Castle Clamber, but the idea of publishing on a large platform and finding out how the Steam client integration worked at a technical level intrigued me. So much so I felt it was worth the submission fee, just to find out.

And, as an experiment, it was certainly interesting. It hasn't made me wealthy, and I never expected it would, but it does carry a certain satisfaction and an air of accomplishment.

After this I worked on a much larger project, a multiplayer, pirate themed, treasure hunting game. I spent the best part of a year on it and got quite far into development, but the scope of it was beginning to creep out of the reach of a single developer such as myself. I eventually burned out on it and it remains incomplete and unpublished - but one day it may get ressurected, just as this very repository shows.

Because, come winter 2018-2019, when I was looking for a holiday project I was reminded that RoboMower was a thing. It was a thing that was no more than an idea and a slightly broken UI, but I was intrigued to find out just how well an old idea could be executed using a thoroughly revised library, xygine, which had been spawned from this very repository all those years before. With all the things I had learned in the interim, just how quickly could I iterate on the idea and create a playable game? As it turns out the core gameplay was done in a matter of weeks, and much of the hardest work was poured into the creation of the (badly drawn) pixel art assets. But hey, I learned something there too. Most immensely satisfying is the overwhelming feeling of achievement from having spent years learning, revising, learning some more and improving myself to be able to go back and realise a project that I had started and abandoned years ago.

And that is how, one year after Castle Clamber, [Mow Problem](https://store.steampowered.com/app/1037140/Mow_Problem/) came to be on Steam. It looks different and there are subtle differences to the original gameplay idea, but it is, ultimately, the final incarnation of RoboMower. I changed the name merely because I felt RoboMower didn't really roll off the tongue. Oh and MowBots are apparently [already a thing](https://mowbotfranchise.com/).

If you read this far then thank you for taking the time to find out a little about my experiences with hobby game development. If instead you scrolled here looking for the download link then you've gone too far. It's in the previous paragraph.


-----------------------------------------------------------------------


RoboMower
---------

Programmable lawn mowing fun!

Using a simple [LOGO](https://en.wikipedia.org/wiki/Logo_(programming_language)) style programming language, program your lawn
mower to cut the grass in the most effecient way possible.

Built using [xygine](https://github.com/fallahn/xygine)

-----------------------------------------------------------------------

Matt Marchant 2015 - 2016  
http://trederia.blogspot.com  

Robomower - Zlib license.  

This software is provided 'as-is', without any express or  
implied warranty. In no event will the authors be held  
liable for any damages arising from the use of this software.  

Permission is granted to anyone to use this software for any purpose,  
including commercial applications, and to alter it and redistribute  
it freely, subject to the following restrictions:  

1. The origin of this software must not be misrepresented;  
you must not claim that you wrote the original software.  
If you use this software in a product, an acknowledgment  
in the product documentation would be appreciated but  
is not required.  

2. Altered source versions must be plainly marked as such,  
and must not be misrepresented as being the original software.  

3. This notice may not be removed or altered from any  
source distribution.  

-----------------------------------------------------------------------
