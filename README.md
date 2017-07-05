# Egyptian Ratscrew
## Introduction
The code in this repository is an implementation of the game [Egyptian Ratscrew](https://en.wikipedia.org/wiki/Egyptian_Ratscrew) as was taught to me by my daughter. 

Unlike other games she's taught me, this one had an interesting element of being quick on the draw; you have to be able to react quickly to the card that's played, (called [Slapping](https://en.wikipedia.org/wiki/Egyptian_Ratscrew#Slapping) by Wikipedia), remembering a number of disparate rules and hope you can get to the pile before the other player(s).

I had not worked with C++11's threading functionality before; I had always used either pthreads or Windows' threading API (::CreateThread() or _beginthread() anyone???), so I thought this would be a nice way to learn how threading had been implemented within C++11 itself.

Also, what I was really interested in was how to implement a guard or barrier, basically having threads all wait for a single signal, then proceeding at the same time and seeing who would win, acknowledging that the number of players/threads performance would be influenced by the number of simutaneous threads the computer running the program has. 
## Goals
I plan on refining the program and rework my C++98-style code using more >= C++11-provided techniques so that I learn, or at least having passing familarity with, the C++ standard as it continues to evolve.

## Parts of the Program
* `main.cpp`
* `Card.cpp/hpp`
* `Player.cpp/hpp`
* `Rules.cpp/hpp`
* `Game.cpp/hpp`
* `Logger.cpp/hpp`

### `main.cpp`
This is where the program starts, creates a game thread where the game object itself is created and the thread returns when the game is over.
### `Card.cpp/hpp`
Implementation of a card in the game. I use C++11-style enums because I wanted to learn how to use them.

The method `getSuiteSymbol()` uses playing card glyphs which the Windows debug window does not seem to like, so the method `getSimpleDescription()` uses an `#ifdef _WIN32` to get the long description (`getDescription()`) if compiling under Windows. This hasn't been a problem under Linux and macOS (and I'm sure it's not a problem under Windows either, I just didn't spend the time to investigate it). 
### `Player.cpp/hpp`
This is very much the heart of the game, as everything revolves around the player. The `playGame()` method is run as a thread by the *Game* object. The *Player* class is a friend of *Game*, which I wondered was a good or bad idea, and decided that it was ultimately acceptable as the there is nothing about the game that ultimately should be hidden from the players. 

#### Um, this class has some pretzel logic going on here...
Right, yes, I know. According to my daughter, and how she was taught, and what Wikipedia says, and what other versions of the game other folks have told me about, the 'rules' seem to vary a lot, but the "face card test" seemed to be common among all of them. So I made the decision to implement what would possibly be a variable set of rules into a *Rules* class (see `Rules.cpp/hpp below`) and implement the face card test as part of the core logic. I admit to not spending more than an hour or two trying to get the face card test working, and I feel strongly enough about it that I'm planning on revisiting the issue and trying to make the whole Player loop flow better and more understandably after other projects get done.
### `Rules.cpp/hpp`
The Rules class encapsulates all the possible variations of the game as a set of functions loaded into a `vector`. I did this deliberately because I thought it would be easy to add rules as simple functions (I casually looked into having them implemented as separate files or in a text file, but I realized I was dangerously close to trying to write a dynamic rules engine).
### `Game.cpp/hpp`
The Game class creates the players, has the discard pile, and is ultimately responsible for coordinating the players access to the pile of discards. `Game.hpp` also has the `Barrier` class which is what the Player objects wait on.
### `Logger.cpp/hpp`
Thinking I might want to create a bot of some sort, I wrote a separate Logging class that could redirect the output to anything (just goes to `std::cout` in this version).

## Building
The code is pure C++11 with no external dependencies (this was key; I wanted to learn C++11's features and not resort to third-party libraries like Boost).

I have tested it with Linux, macOS, and Windows (using VS 2015) and the code should build with no problems.
