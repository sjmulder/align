#!/bin/sh
echo "Romeo & Juliet" | ./center -w20 # ASCII
echo "Romeo & Juliët" | ./center -w20 # UTF-8 NFC (one char)
echo "Romeo & Juliët" | ./center -w20 # UTF-8 NFD (two chars)
