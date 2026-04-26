g++ src/*.cpp -Iinclude -o bin/virtualDisk && .bin/virtualDisk


NOTE FOR TOMMOROW
- fix found bugs
  1. when running cd ./ on root it displays double roots | FIXED
  2. Add validation to touch currently it produces a segmentation fault if a file without an extension name is provided | FIXED
- make ls handle listing of folders too | FIXED
- work on input validation for ls, cd and mkdir before proceeding on working to other commands | DONE
  and handling edge case scenarios | IN PROGRESS
- work on the next line of commands - cp, mv and echo
- fix the display interface when running ls