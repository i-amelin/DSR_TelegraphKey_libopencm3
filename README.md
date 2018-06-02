# Telegraph Key

Simulator of a telegraph key that encodes a message using Morse code. It's like a one-key keyboard :)
The message enters by pressing the User Button connected to PE0.

### Building

1. Build the libopencm3 library:
```
cd libopencm3 && make
```

2. Build the workshop:
```
cd ws_telegraph_key && make bin
```

3. If you are using st-flash, run the following command:
```
st-flash write telegraph_key.bin 0x8000000
```

Now you are ready to use the telegraph key!

### Using

0. Prepare yourself: change your keyboard layout to EN, learn the Morse code (https://en.wikipedia.org/wiki/Morse_code), and open any text editor or console.
1. Connect the device to the computer.
2. In 2 seconds, press the User Button one time to enter a dot; three times to enter a dash; don't press the User Button to send encoded symbol to the computer.

For example, try to enter the following:

dash dot dot

dot dot dot

dot dash dot