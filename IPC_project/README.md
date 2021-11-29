## Table of contents
* [General info](#general-info)
* [How does it work](#how-does-it-work)
* [Setup](#setup)

## General info
Program is a practice implementation of queue mechanism for passing the shared information across the components.
	
## How does it work
Project contains two threads and shared queue
* Writer thread parses InputArray and puts sentences into queue.
* When user chooses 'Q', reader thread gets one sentence from queue and prints it.
* When user chooses 'X' program ends
	
## Setup
To run this project

```
$ make
```